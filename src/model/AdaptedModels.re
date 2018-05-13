open AdaptedModel;

open Belt;

let e: Map.String.t((module T)) = Map.String.empty;

let make = () =>
  Map.String.(
    e
    |. set(
         "TopPage",
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
         "ItemPage",
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
