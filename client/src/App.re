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
    let id = Router.getModelId(state.url);
    let pageState = Belt.Map.get(state.pageStates, id);
    <div>
      (
        switch (id, pageState) {
        | (TopPage, Some(TopPageState(s))) =>
          <TopPage send=(send << (a => `TopPageAction(a))) topPageState=s />
        | (ItemPage(name), Some(ItemPageState(s))) =>
          <ItemPage
            send=(send << (a => `ItemPageAction(a)))
            itemPageState=s
            name
          />
        | (ErrorPage, _) => <ErrorPage />
        | (_, _) => s_("Now Loading")
        }
      )
    </div>;
  },
};
