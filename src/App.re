open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState,
  reducer,
  didMount: ({send, onUnmount}) => {
    let watcherID = ReasonReact.Router.watchUrl(send @@@ changeUrl);
    onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    let actionStream = Most.Subject.asStream(actionSubject);
    AppObserver.observe(actionStream, send);
  },
  render: ({send, state}) => {
    let sendChildAction = (actionCreator, action) =>
      send @@ actionCreator(action);
    <div>
      (
        switch(state.url.path) {
        | ["items", name] =>
          <ItemPage
            send=sendChildAction(itemPageAction)
            itemPageState=state.itemPage
            name
          />
        | _ =>
          <TopPage
            send=sendChildAction(topPageAction)
            topPageState=state.topPage
          />
        }
      )
    </div>;
  },
};
