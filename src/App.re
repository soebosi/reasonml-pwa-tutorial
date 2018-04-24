type state = {
  item: ItemPageReducer.state,
  topPage: TopPageReducer.state,
  url: ReasonReact.Router.url,
};

[@bs.deriving accessors]
type childAction =
  | ItemPage(ItemPageReducer.action)
  | TopPage(TopPageReducer.action);

[@bs.deriving accessors]
type action =
  | DispatchChildAction(childAction)
  | ChangePage(ReasonReact.Router.url);

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState: () => {
    {
      item: ItemPageReducer.initialState(),
      topPage: TopPageReducer.initialState(),
      url: {path: [""], hash: "", search: ""},
    };
  },
  reducer: (action, state) => {
    let newState = switch(action) {
    | DispatchChildAction(action) => switch(action) {
      | ItemPage(action) => {...state, item: ItemPageReducer.reducer(action, state.item)}
      | TopPage(action) => {...state, topPage: TopPageReducer.reducer(action, state.topPage)}
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
          itemPageState=self.state.item
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
