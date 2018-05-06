open Belt;

open MostEx;

[@bs.deriving accessors]
type action =
  | ItemPageAction(ItemPageModel.action)
  | TopPageAction(TopPageModel.action)
  | ChangeUrl(ReasonReact.Router.url);

let getTopPageAction = a =>
  switch (a) {
  | TopPageAction(a) => Some(a)
  | _ => None
  };

let getItemPageAction = a =>
  switch (a) {
  | ItemPageAction(a) => Some(a)
  | _ => None
  };

[@bs.deriving accessors]
type childState =
  | ItemPageState(ItemPageModel.state)
  | TopPageState(TopPageModel.state);

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
  let reducer: (action, childState) => childState;
  let initialState: unit => childState;
  let epic: Most.stream(action) => Most.stream(action);
};

let models: array((module AdaptedModel)) = [|
  (module
   ModelAdaptor.Make(
     {
       type adaptedState = childState;
       type adaptedAction = action;
       include TopPageModel;
       let createState = topPageState;
       let getState = getTopPageState;
       let createAction = topPageAction;
       let getAction = getTopPageAction;
     },
   )),
  (module
   ModelAdaptor.Make(
     {
       type adaptedState = childState;
       type adaptedAction = action;
       include ItemPageModel;
       let createState = itemPageState;
       let getState = getItemPageState;
       let createAction = itemPageAction;
       let getAction = getItemPageAction;
     },
   )),
|];

type state = {
  childStates: array(childState),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  childStates:
    Array.mapU(models, (. (module M): (module AdaptedModel)) =>
      M.initialState()
    ),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | ChangeUrl(url) => {...state, url}
    | _ =>
      let childStates =
        Array.mapWithIndexU(models, (. i, (module M): (module AdaptedModel)) =>
          M.reducer(action, Option.getExn(state.childStates[i]))
        );
      {...state, childStates};
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  Array.mapU(models, (. (module M): (module AdaptedModel)) => M.epic(stream))
  |. Most.mergeArray;
