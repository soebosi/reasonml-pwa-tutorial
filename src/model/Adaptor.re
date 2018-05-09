open Belt;

open MostEx;

type adaptedAction = [
  | `ItemPageAction(ItemPageModel.action)
  | `TopPageAction(TopPageModel.action)
];

[@bs.deriving accessors]
type adaptedState =
  | ItemPageState(ItemPageModel.state)
  | TopPageState(TopPageModel.state);

module type Model = {
  type action;
  type state;
  let initialState: unit => state;
  let reducer: (action, state) => state;
  let epic: Most.stream(action) => Most.stream(action);
  let getState: adaptedState => option(state);
  let createState: state => adaptedState;
  let getAction: [> adaptedAction] => option(action);
  let createAction: action => [> adaptedAction];
};

let itemPageAction = a => `ItemPageAction(a);

let topPageAction = a => `TopPageAction(a);

let getTopPageAction = a =>
  switch (a) {
  | `TopPageAction(a) => Some(a)
  | _ => None
  };

let getItemPageAction = a =>
  switch (a) {
  | `ItemPageAction(a) => Some(a)
  | _ => None
  };

let getTopPageState = a =>
  switch (a) {
  | TopPageState(a) => Some(a)
  | _ => None
  };

let getItemPageState = a =>
  switch (a) {
  | ItemPageState(a) => Some(a)
  | _ => None
  };

module type AdaptedModel = {
  let reducer: ([> adaptedAction], adaptedState) => adaptedState;
  let initialState: unit => adaptedState;
  let epic: Most.stream([> adaptedAction]) => Most.stream([> adaptedAction]);
};

module Make = (M: Model) => {
  type state = M.state;
  type action = M.action;
  let initialState = () => M.createState(M.initialState());
  let reducer = (action: [> adaptedAction], state) =>
    switch (M.getAction(action), M.getState(state)) {
    | (Some(a), Some(s)) => M.reducer(a, s) |. M.createState
    | _ => state
    };
  let epic = stream : Most.stream([> adaptedAction]) =>
    Most.(stream |> keepMap(M.getAction) |> M.epic |> map(M.createAction));
};

let e: Map.String.t((module AdaptedModel)) = Map.String.empty;

let models =
  Map.String.(
    e
    |. set(
         "TopPage",
         (module
          Make(
            {
              include TopPageModel;
              let createState = topPageState;
              let getState = getTopPageState;
              let createAction = topPageAction;
              let getAction = getTopPageAction;
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
              let getState = getItemPageState;
              let createAction = itemPageAction;
              let getAction = getItemPageAction;
            },
          )),
       )
  );
