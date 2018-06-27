open Belt;

open MostEx;

type action = [
  PageModel.adaptedAction
  | `ChangeUrl(ReasonReact.Router.url)
  | `InitialPageState(PageModel.id, PageModel.adaptedAction)
];

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

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | `ChangeUrl(url) => {...state, url}
    | `InitialPageState(id, action) =>
      let model = PageModelMap.getModel(id);
      let pageStates =
        Map.updateU(state.pageStates, id, (. state) =>
          switch (state, model) {
          | (None, Some((module M))) =>
            Some(M.reducer(action, M.initialState()))
          | (s, _) => s
          }
        );
      {...state, pageStates};
    | _ =>
      let id = Router.getStateID(state.url);
      let model = PageModelMap.getModel(id);
      let pageStates =
        Map.updateU(state.pageStates, id, (. state) =>
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

let getChangeUrl = ((a, s)) =>
  switch (a) {
  | `ChangeUrl(url) => Some((Router.getStateID(url), s))
  | _ => None
  };

let actionEpic = stream =>
  PageModelMap.models
  |. Array.mapU((. (module M): (module PageModel.T)) => M.epic(stream))
  |. Array.concat([|stream |> Most.keepMap(getChangeUrl) |> Router.epic|])
  |. Most.mergeArray;
