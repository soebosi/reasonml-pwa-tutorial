open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState,
  reducer,
  didMount: self => {
    let watcherID = ReasonReact.Router.watchUrl(url => changeUrl(url) |. self.send);
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    AppObserver.observe(Most.Subject.asStream(actionSubject), self.send);
  },
  render: self => {
    let sendChildAction = (actionCreator, action) =>
      actionCreator(action)
      |. self.send;
    <div>
      (
        switch(self.state.url.path) {
        | ["items", name] =>
          <ItemPage
            send=sendChildAction(itemPageAction)
            itemPageState=self.state.itemPage
            name
          />
        | _ =>
          <TopPage
            send=sendChildAction(topPageAction)
            topPageState=self.state.topPage
          />
        }
      )
    </div>;
  },
};
