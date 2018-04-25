open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState,
  reducer,
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
