open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState: initialStore,
  reducer,
  didMount: ({send, onUnmount}) => {
    open ReasonReact.Router;
    let watcherID = watchUrl(send << changeUrl);
    onUnmount(() => unwatchUrl(watcherID));
    send @@ changeUrl @@ dangerouslyGetInitialUrl();
    Most.(
      Subject.asStream(actionSubject) |> epic |> observe(send) |> ignore
    );
  },
  render: ({send, state}) => {
    let stateID = Router.getPageStateID(state.url);
    let pageState = Belt.Map.get(state.pageStates, stateID);
    <div>
      (
        switch (stateID, pageState) {
        | (TopPage, Some(TopPageState(state))) =>
          let send = send << (a => PageAction(`TopPageAction(a)));
          <TopPage send state />;
        | (ItemPage(_), Some(ItemPageState(state))) =>
          let send = send << (a => PageAction(`ItemPageAction(a)));
          <ItemPage send state />;
        | (ErrorPage, _) => <ErrorPage />
        | (_, _) => s_("Now Loading")
        }
      )
    </div>;
  },
};
