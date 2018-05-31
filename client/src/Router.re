open Util;
open Belt;
exception Unreachable;

[@bs.deriving abstract]
type pageMap = {
  page: ReasonReact.reactElement,
  model: (module PageModel.T),
};

let getPageMap = (send, state: AppModel.state) =>
  switch (state.url.path) {
  | ["items", name] =>
    pageMap(
      ~page=
        <ItemPage
          send=(send << (a => `ItemPageAction(a)))
          itemPageState=(
            switch (Map.getExn(state.pageStates, ItemPage)) {
            | ItemPageState(s) => s
            | _ => raise(Unreachable)
            }
          )
          name
        />,
      ~model=
        (module
         PageModel.Make({
           open PageModel;
           include ItemPageModel;
           let adaptState = itemPageState;
           let getState = a =>
             switch (a) {
             | ItemPageState(a) => Some(a)
             | _ => None
             };
           let adaptAction = a => `ItemPageAction(a);
           let getAction = a =>
             switch (a) {
             | `ItemPageAction(a) => Some(a)
             | _ => None
             };
         })),
    )
  | [] =>
    pageMap(
      ~page=
        <TopPage
          send=(send << (a => `TopPageAction(a)))
          topPageState=(
            switch (Map.getExn(state.pageStates, TopPage)) {
            | TopPageState(s) => s
            | _ => raise(Unreachable)
            }
          )
        />,
      ~model=
        (module
         PageModel.Make({
           open PageModel;
           include TopPageModel;
           let adaptState = topPageState;
           let getState = a =>
             switch (a) {
             | TopPageState(a) => Some(a)
             | _ => None
             };
           let adaptAction = a => `TopPageAction(a);
           let getAction = a =>
             switch (a) {
             | `TopPageAction(a) => Some(a)
             | _ => None
             };
         })),
    )
  | _ =>
    pageMap(
      ~page=<ErrorPage />,
      ~model=
        (module
         PageModel.Make({
           open PageModel;
           include TopPageModel;
           let adaptState = topPageState;
           let getState = a =>
             switch (a) {
             | TopPageState(a) => Some(a)
             | _ => None
             };
           let adaptAction = a => `TopPageAction(a);
           let getAction = a =>
             switch (a) {
             | `TopPageAction(a) => Some(a)
             | _ => None
             };
         })),
    )
  };
