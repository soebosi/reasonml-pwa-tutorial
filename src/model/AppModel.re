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

let observe = (stream, send) => {
  TopPageObserver.observe(
    filterMap(
      a =>
        switch (a) {
        | TopPageAction(a) => Some(a)
        | _ => None
        },
      stream,
    ),
    x =>
    send(TopPageAction(x))
  )
  |. ignore;
  ItemPageObserver.observe(
    filterMap(
      a =>
        switch (a) {
        | ItemPageAction(a) => Some(a)
        | _ => None
        },
      stream,
    ),
    x =>
    send(ItemPageAction(x))
  )
  |. ignore;
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
