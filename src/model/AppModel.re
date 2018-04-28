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

let reducer = (action, state) =>
  switch (action) {
  | DispatchChildAction(action) =>
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
  ReasonReact.Update(newState);
};
