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
type childAction =
  | ItemPageAction(ItemPageModel.action)
  | TopPageAction(TopPageModel.action);

[@bs.deriving accessors]
type action =
  | DispatchChildAction(childAction)
  | ChangeUrl(ReasonReact.Router.url);

let reducer = (action, state) =>
  switch (action) {
  | DispatchChildAction(action) =>
    switch (action) {
    | ItemPageAction(action) =>
      ItemPageModel.reducer(
        action,
        state.itemPage,
        itemPage => {...state, itemPage},
        dispatchChildAction,
      )
    | TopPageAction(action) =>
      TopPageModel.reducer(
        action,
        state.topPage,
        topPage => {...state, topPage},
        dispatchChildAction,
      )
    }
  | ChangeUrl(url) => ReasonReact.Update({...state, url})
  };
