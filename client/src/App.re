open Util;

open AppModel;

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState: initialStore,
  reducer,
  didMount: ({send, onUnmount}) => {
    let watcherID = ReasonReact.Router.watchUrl(send << changeUrl);
    onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    send @@ changeUrl @@ ReasonReact.Router.dangerouslyGetInitialUrl();
    Most.(
      Subject.asStream(actionSubject)
      |> epic
      |> observe(send << pageAction)
      |> ignore
    );
  },
  render: ({send, state}) => {
    let stateID = Router.getPageStateID(state.url);
    let pageState = Belt.Map.get(state.pageStates, stateID);
    <div>
      (
        switch (stateID, pageState) {
        | (TopPage, Some(TopPageState(state))) =>
          let send = send << pageAction << PageModel.topPageAction;
          <TopPage send state />;
        | (ItemPage(_), Some(ItemPageState(state))) =>
          let send = send << pageAction << PageModel.itemPageAction;
          <ItemPage send state />;
        | (ErrorPage, _) => <ErrorPage />
        | (_, _) => s_("Now Loading")
        }
      )
    </div>;
  },
};
