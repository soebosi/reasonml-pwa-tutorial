open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState,
  reducer,
  didMount: self => {
    let watcherID = ReasonReact.Router.watchUrl(self.send @@@ changeUrl);
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    let actionStream = Most.Subject.asStream(actionSubject);
    AppObserver.observe(actionStream, self.send);
  },
  render: self => {
    let sendChildAction = (actionCreator, action) =>
      self.send @@ actionCreator(action);
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
