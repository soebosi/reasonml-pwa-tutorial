type state = {
  itemPage: ItemPageModel.state,
  topPage: TopPageModel.state,
  url: ReasonReact.Router.url,
};

[@bs.deriving accessors]
type childAction =
  | ItemPage(ItemPageModel.action)
  | TopPage(TopPageModel.action);

[@bs.deriving accessors]
type action =
  | DispatchChildAction(childAction)
  | ChangePage(ReasonReact.Router.url);

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState: () => {
    {
      itemPage: ItemPageModel.initialState(),
      topPage: TopPageModel.initialState(),
      url: {path: [""], hash: "", search: ""},
    };
  },
  reducer: (action, state) => {
    let newState = switch(action) {
    | DispatchChildAction(action) => switch(action) {
      | ItemPage(action) => {...state, itemPage: ItemPageModel.reducer(action, state.itemPage)}
      | TopPage(action) => {...state, topPage: TopPageModel.reducer(action, state.topPage)}
      }
    | ChangePage(url) => {...state, url}
    };
    ReasonReact.Update(newState);
  },
  subscriptions: self => [
    Sub(
      () => {
        ReasonReact.Router.watchUrl(url => url |. changePage |. self.send);
      },
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: self => {
    let sendChildAction = (constructor, action) =>
      constructor(action)
      |. dispatchChildAction
      |. self.send;
    <div>
    (
      switch(self.state.url.path) {
      | ["items", name] =>
        <ItemPage
          dispatch=sendChildAction(itemPage)
          itemPageState=self.state.itemPage
          name
        />
      | _ =>
        <TopPage
          dispatch=sendChildAction(topPage)
          topPageState=self.state.topPage
        />
      }
    )
    </div>;
  },
};
