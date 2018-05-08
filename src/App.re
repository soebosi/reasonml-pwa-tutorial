open Belt;

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
    {
      open Most;
      Subject.asStream(actionSubject)
      |> actionEpic
      |> observe(send)
      |> ignore;
    };
  },
  render: ({send, state}) => {
    <div>
      (
        switch(state.url.path) {
        | ["items", name] =>
          <ItemPage
            send=(send << itemPageAction)
            itemPageState=Option.getExn(getItemPageState(Option.getExn(state.pageStates[1])))
            name
          />
        | _ =>
          <TopPage
            send=(send << topPageAction)
            topPageState=Option.getExn(getTopPageState(Option.getExn(state.pageStates[0])))
          />
        }
      )
    </div>;
  },
};
