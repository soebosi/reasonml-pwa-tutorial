open Util;
open Belt;
exception Unreachable;

let getPage = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | ["items", name] => (
      (send, state: AppModel.state) => {
        let pageState = Map.getExn(state.pageStates, ItemPage);
        <ItemPage
          send=(send << (a => `ItemPageAction(a)))
          itemPageState=(
            switch (pageState) {
            | ItemPageState(s) => s
            | _ => raise(Unreachable)
            }
          )
          name
        />;
      }
    )
  | [] => (
      (send, state) => {
        let pageState = Map.getExn(state.pageStates, TopPage);
        <TopPage
          send=(send << (a => `TopPageAction(a)))
          topPageState=(
            switch (pageState) {
            | TopPageState(s) => s
            | _ => raise(Unreachable)
            }
          )
        />;
      }
    )
  | _ => ((send, state) => <ErrorPage />)
  };
