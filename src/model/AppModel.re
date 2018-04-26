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
  | ItemPage(ItemPageModel.action)
  | TopPage(TopPageModel.action);

[@bs.deriving accessors]
type action =
  | DispatchChildAction(childAction)
  | ChangeUrl(ReasonReact.Router.url);

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | DispatchChildAction(action) =>
      switch (action) {
      | ItemPage(action) => {
          ...state,
          itemPage: ItemPageModel.reducer(action, state.itemPage),
        }
      | TopPage(action) => {
          ...state,
          topPage: TopPageModel.reducer(action, state.topPage),
        }
      }
    | ChangeUrl(url) => {...state, url}
    };
  ReasonReact.Update(newState);
};
