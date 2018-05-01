open Util;

type state = {
  itemPageState: ItemPageModel.state,
  topPageState: TopPageModel.state,
  url: ReasonReact.Router.url,
};

let initialState = () => {
  itemPageState: ItemPageModel.initialState(),
  topPageState: TopPageModel.initialState(),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

[@bs.deriving accessors]
type action =
  | ItemPageAction(ItemPageModel.action)
  | TopPageAction(TopPageModel.action)
  | ChangeUrl(ReasonReact.Router.url);

let getTopPageAction = a =>
  switch (a) {
  | TopPageAction(a) => Some(a)
  | _ => None
  };

let getItemPageAction = a =>
  switch (a) {
  | ItemPageAction(a) => Some(a)
  | _ => None
  };

let actionSubject = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | ItemPageAction(action) => {
        ...state,
        itemPageState: ItemPageModel.reducer(action, state.itemPageState),
      }
    | TopPageAction(action) => {
        ...state,
        topPageState: TopPageModel.reducer(action, state.topPageState),
      }
    | ChangeUrl(url) => {...state, url}
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  Most.(
    mergeArray([|
      stream
      |> filterMap(getTopPageAction)
      |> TopPageModel.epic
      |> map(topPageAction),
      stream
      |> filterMap(getItemPageAction)
      |> ItemPageModel.epic
      |> map(itemPageAction),
    |])
  );
