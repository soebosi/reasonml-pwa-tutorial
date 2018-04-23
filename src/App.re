type state = {
  item: ItemPageReducer.state,
  topPage: TopPageReducer.state,
  url: ReasonReact.Router.url,
};

[@bs.deriving accessors]
type action =
  | Item(ItemPageReducer.action)
  | Top(TopPageReducer.action)
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
    | Item(itemAction) => {...state, item: ItemPageReducer.reducer(itemAction, state.item)}
    | Top(topPageAction) => {...state, topPage: TopPageReducer.reducer(topPageAction, state.topPage)}
    | ChangePage(url) => {...state, url}
    };
    ReasonReact.Update(newState);
  },
  subscriptions: self => [
    Sub(
      () => {
        ReasonReact.Router.watchUrl(url =>
          url |. changePage |. self.send
        );
      },
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: self => 
    <div>
    (
      switch(self.state.url.path) {
      | ["items", name] =>
        <ItemPage
          dispatch=((action) => self.send(Item(action)))
          itemPageState=self.state.item
          name
        />
      | _ =>
        <TopPage
          dispatch=((action) => self.send(Top(action)))
          topPageState=self.state.topPage
        />
      }
    )
    </div>,
};
