[@bs.deriving jsConverter]
type key =
  | TopPage
  | ItemPage;

module PageCmp =
  Belt.Id.MakeComparable({
    type t = key;
    let cmp = (a, b) => keyToJs(a) - keyToJs(b);
  });

type id = PageCmp.identity;

let m: Belt.Map.t(PageCmp.t, (module AdaptedModel.T), id) =
  Belt.Map.make(~id=(module PageCmp));

let make = () =>
  Belt.Map.(
    m
    |. set(
         TopPage,
         (module
          AdaptedModel.Make({
            open AdaptedModel;
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
    |. set(
         ItemPage,
         (module
          AdaptedModel.Make({
            open AdaptedModel;
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
  );
