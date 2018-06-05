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

let actionSubject: Most.Subject.t(action) = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | `ChangeUrl(url) =>
      let id = Router.getModelId @@ url;
      let pageStates =
        state.pageStates
        |. Map.updateU(id, (. value) =>
             switch (value) {
             | Some(v) => Some(v)
             | None =>
               let (module M) = PageModelMap.getModel(id);
               Some(M.initialState());
             }
           );
      {url, pageStates};
    | _ =>
      let id = Router.getModelId @@ state.url;
      let pageStates =
        state.pageStates
        |. Map.updateU(id, (. value) =>
             switch (value) {
             | Some(v) =>
               let (module M) = PageModelMap.getModel(id);
               Some(M.reducer(action, v));
             | None => None
             }
           );
      {...state, pageStates};
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  PageModelMap.models
  |. Array.mapU((. (module M): (module PageModel.T)) => M.epic(stream))
  |. Most.mergeArray;
