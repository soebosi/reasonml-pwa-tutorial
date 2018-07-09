open MostEx;

type action = [
  PageModel.adaptedAction
  | `ChangeUrl(ReasonReact.Router.url)
  | `InitializePageState(PageModel.adaptedAction)
];

let changeUrl = a => `ChangeUrl(a);

module PageCmp =
  Belt.Id.MakeComparable({
    type t = PageModel.id;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type state = {
  pageStates:
    Belt.Map.t(PageModel.id, PageModel.adaptedState, PageCmp.identity),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates: Belt.Map.make(~id=(module PageCmp)),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t((action, option(PageModel.adaptedState))) =
  Most.Subject.make();

let optionMap2 = (a, b, f) =>
  switch (a, b) {
  | (Some(a), Some(b)) => Some(f(a, b))
  | (_, _) => None
  };

let reducer = (action, state) => {
  let id = Router.getPageStateID(state.url);
  let model = PageModelMap.getModel(id);
  let newState =
    switch (action) {
    | `ChangeUrl(url) => {...state, url}
    | `InitializePageState(action) => {
        ...state,
        pageStates:
          Belt.Map.updateU(state.pageStates, id, (. _) =>
            Belt.Option.map(model, ((module M)) =>
              M.reducer(action, M.initialState())
            )
          ),
      }
    | _ => {
        ...state,
        pageStates:
          Belt.Map.update(
            state.pageStates,
            id,
            optionMap2(model, _, ((module M), s) => M.reducer(action, s)),
          ),
      }
    };
  let id = Router.getPageStateID(newState.url);
  let pageState = Belt.Map.get(state.pageStates, id);
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next((action, pageState), actionSubject) |. ignore,
  );
};

let getStateIDFromChangeUrl = a =>
  switch (a) {
  | `ChangeUrl(url) => Some(Router.getPageStateID(url))
  | _ => None
  };

let getActionIfPageStateIsEmpty = ((a, s)) =>
  switch (s) {
  | None => Some(a)
  | _ => None
  };

let epic = stream =>
  PageModelMap.models
  |. Belt.Array.mapU((. (module M): (module PageModel.T)) => M.epic(stream))
  |. Belt.Array.concat([|
       stream
       |> Most.keepMap(getActionIfPageStateIsEmpty)
       |> Most.keepMap(getStateIDFromChangeUrl)
       |> Router.epic
       |> Most.map(a => `InitializePageState(a)),
     |])
  |. Most.mergeArray;
