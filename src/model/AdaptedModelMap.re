open AdaptedModel;

[@bs.deriving jsConverter]
type key =
  | TopPage
  | ItemPage;

module PageCmp =
  Belt.Id.MakeComparable(
    {
      type t = key;
      let cmp = (a, b) => keyToJs(a) - keyToJs(b);
    },
  );

type id = PageCmp.identity;

let m: Belt.Map.t(PageCmp.t, (module T), id) =
  Belt.Map.make(~id=(module PageCmp));

let make = () =>
  Belt.Map.(
    m
    |. set(
         TopPage,
         (module
          Make(
            {
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
            },
          )),
       )
    |. set(
         ItemPage,
         (module
          Make(
            {
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
            },
          )),
       )
  );
