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
          <ItemPage
            send=(send << (a => `ItemPageAction(a)))
            itemPageState=(switch(Map.getExn(state.pageStates, ItemPage)) {
              | ItemPageState(s) => s
              | _ => raise(Unreachable)
            })
            name
          />
        }
        | [] => {
          <TopPage
            send=(send << (a => `TopPageAction(a)))
            topPageState=(switch(Map.getExn(state.pageStates, TopPage)) {
              | TopPageState(s) => s
              | _ => raise(Unreachable)
            })
          />
        }
        }
      )
    </div>;
  },
};
