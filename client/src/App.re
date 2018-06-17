open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState,
  reducer,
  didMount: ({send, onUnmount}) => {
    open ReasonReact.Router;
    let watcherID = watchUrl(send << changeUrl);
    onUnmount(() => unwatchUrl(watcherID));
    send @@ changeUrl @@ dangerouslyGetInitialUrl();
    Most.(
      Subject.asStream(actionSubject)
      |> actionEpic
      |> observe(send)
      |> ignore
    );
  },
  render: ({send, state}) => {
    let id = Router.getStateID(state.url);
    let pageState = Belt.Map.get(state.pageStates, id);
    <div>
      (
        switch (id, pageState) {
        | (TopPage, Some(TopPageState(state))) =>
          let send = send << (a => `TopPageAction(a));
          <TopPage send state />;
        | (ItemPage(name), Some(ItemPageState(state))) =>
          let send = send << (a => `ItemPageAction(a));
          <ItemPage send state name />;
        | (ErrorPage, _) => <ErrorPage />
        | (_, _) => s_("Now Loading")
        }
      )
    </div>;
  },
};
