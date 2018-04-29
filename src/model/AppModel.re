open Util;

type state = {
  itemPage: ItemPageModel.state,
  topPage: TopPageModel.state,
  url: ReasonReact.Router.url,
};

let initialState = () => {
  itemPage: ItemPageModel.initialState(),
  topPage: TopPageModel.initialState(),
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

let observe = (stream, send) => {
  stream
  |> filterMap(getTopPageAction)
  |> TopPageObserver.observe(action => send(TopPageAction(action)));
  stream
  |> filterMap(getItemPageAction)
  |> ItemPageObserver.observe(action => send(ItemPageAction(action)));
};

let actionSubject = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | ItemPageAction(action) => {
        ...state,
        itemPage: ItemPageModel.reducer(action, state.itemPage),
      }
    | TopPageAction(action) => {
        ...state,
        topPage: TopPageModel.reducer(action, state.topPage),
      }
    | ChangeUrl(url) => {...state, url}
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |> ignore,
  );
};
