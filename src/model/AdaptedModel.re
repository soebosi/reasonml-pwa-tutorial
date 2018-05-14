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
  let adaptState: state => adaptedState;
  let getAction: [> adaptedAction] => option(action);
  let adaptAction: action => [> adaptedAction];
};

module type T = {
  let initialState: unit => adaptedState;
  let reducer: ([> adaptedAction], adaptedState) => adaptedState;
  let epic: Most.stream([> adaptedAction]) => Most.stream([> adaptedAction]);
};

module Make = (M: Model) => {
  let initialState = () => M.adaptState(M.initialState());
  let reducer = (action: [> adaptedAction], state) =>
    switch (M.getAction(action), M.getState(state)) {
    | (Some(a), Some(s)) => M.reducer(a, s) |. M.adaptState
    | _ => state
    };
  let epic = stream : Most.stream([> adaptedAction]) =>
    Most.(stream |> keepMap(M.getAction) |> M.epic |> map(M.adaptAction));
};
