open Util;

open AppModel;

exception Unreachable;

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
    /*
     Most.(
       Subject.asStream(actionSubject)
       |> actionEpic
       |> observe(send)
       |> ignore
     );
     */
  },
  render: ({send, state}) => {
    let key = Router.getKey(state.url);
    let pageState = Belt.Map.get(state.pageStates, key);
    switch (pageState) {
    | Some(pageState) =>
      <div>
        (
          switch (key) {
          | TopPage =>
            <TopPage
              send=(send << (a => `TopPageAction(a)))
              topPageState=(
                switch (pageState) {
                | TopPageState(s) => s
                | _ => raise(Unreachable)
                }
              )
            />
          | ItemPage(name) =>
            <ItemPage
              send=(send << (a => `ItemPageAction(a)))
              itemPageState=(
                switch (pageState) {
                | ItemPageState(s) => s
                | _ => raise(Unreachable)
                }
              )
              name
            />
          | ErrorPage => <ErrorPage />
          }
        )
      </div>
    | None => <div> (s_("hoge")) </div>
    };
  },
};
