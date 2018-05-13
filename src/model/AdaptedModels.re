open AdaptedModel;

open Belt;

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

type t = Map.t(PageCmp.t, (module T), PageCmp.identity);

type id = PageCmp.identity;

let m: t = Map.make(~id=(module PageCmp));

let make = () =>
  Map.(
    m
    |. set(
         TopPage,
         (module
          Make(
            {
              include TopPageModel;
              let createState = topPageState;
              let getState = a =>
                switch (a) {
                | TopPageState(a) => Some(a)
                | _ => None
                };
              let createAction = a => `TopPageAction(a);
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
              let createState = itemPageState;
              let getState = a =>
                switch (a) {
                | ItemPageState(a) => Some(a)
                | _ => None
                };
              let createAction = a => `ItemPageAction(a);
              let getAction = a =>
                switch (a) {
                | `ItemPageAction(a) => Some(a)
                | _ => None
                };
            },
          )),
       )
  );
