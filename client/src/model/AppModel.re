open Belt;

open MostEx;

type action = [ PageModel.adaptedAction | `ChangeUrl(ReasonReact.Router.url)];

let changeUrl = a => `ChangeUrl(a);

module PageCmp =
  Id.MakeComparable({
    type t = PageModel.id;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type state = {
  pageStates: Map.t(PageModel.id, PageModel.adaptedState, PageCmp.identity),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates: Map.make(~id=(module PageCmp)),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t((action, option(PageModel.adaptedState))) =
  Most.Subject.make();

let updatePageState = (pageStates, url, updater) => {
  let id = Router.getStateID(url);
  let model = PageModelMap.getModel(id);
  Map.updateU(pageStates, id, (. state) => updater(state, model));
};

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | `ChangeUrl(url) =>
      let pageStates =
        updatePageState(state.pageStates, url, (state, model) =>
          switch (state, model) {
          | (None, Some((module M))) => Some(M.initialState())
          | (s, _) => s
          }
        );
      {url, pageStates};
    | _ =>
      let pageStates =
        updatePageState(state.pageStates, state.url, (state, model) =>
          switch (state, model) {
          | (Some(s), Some((module M))) => Some(M.reducer(action, s))
          | (_, _) => None
          }
        );
      {...state, pageStates};
    };
  let id = Router.getStateID(newState.url);
  let pageState = Map.get(state.pageStates, id);
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next((action, pageState), actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  PageModelMap.models
  |. Array.mapU((. (module M): (module PageModel.T)) => M.epic(stream))
  |. Array.concat([||])
  |. Most.mergeArray;
