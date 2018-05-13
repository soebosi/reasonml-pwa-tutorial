open Belt;

open Util;

open AppModel;

exception Unreachable;

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
        | ["items", name] => {
          let itemPageState = switch(Map.getExn(state.pageStates, ItemPage)) {
            | ItemPageState(s) => s
            | _ => raise(Unreachable)
          };
          <ItemPage
            send=(send << (a => `ItemPageAction(a)))
            itemPageState
            name
          />
        }
        | _ => {
          let topPageState = switch(Map.getExn(state.pageStates, TopPage)) {
            | TopPageState(s) => s
            | _ => raise(Unreachable)
          };
          <TopPage
            send=(send << (a => `TopPageAction(a)))
            topPageState
          />
        }
        }
      )
    </div>;
  },
};
