open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState,
  reducer,
  didMount: ({send, onUnmount}) => {
    {
      open ReasonReact.Router;
      let watcherID = watchUrl(send << changeUrl);
      onUnmount(() => unwatchUrl(watcherID));
    };
    let actionStream = Most.Subject.asStream(actionSubject);
    AppObserver.observe(actionStream, send);
  },
  render: ({send, state}) => {
    <div>
      (
        switch(state.url.path) {
        | ["items", name] =>
          <ItemPage
            send=(send << itemPageAction)
            itemPageState=state.itemPageState
            name
          />
        | _ =>
          <TopPage
            send=(send << topPageAction)
            topPageState=state.topPageState
          />
        }
      )
    </div>;
  },
};
