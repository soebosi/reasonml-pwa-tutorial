open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState,
  reducer,
  didMount: self => {
    let watcherID = ReasonReact.Router.watchUrl(url => changeUrl(url) |. self.send);
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },
  render: self => {
    let sendChildAction = (actionCreator, action) =>
      actionCreator(action)
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
