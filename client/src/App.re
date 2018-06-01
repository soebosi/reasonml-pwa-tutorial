open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState,
  reducer,
  didMount: ({send, onUnmount}) => {
    {
      open ReasonReact.Router;
      let watcherID = watchUrl(send << changeUrl);
      onUnmount(() => unwatchUrl(watcherID));
      send @@ changeUrl @@ dangerouslyGetInitialUrl();
    };
    Most.(
      Subject.asStream(actionSubject)
      |> actionEpic
      |> observe(send)
      |> ignore
    );
  },
  render: ({send, state}) => {
    let page = Router.getPage(state.url);
    <div> (page(send, state)) </div>;
  },
};
