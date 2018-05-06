open MostEx;

module type Model = {
  type state;
  type adaptedState;
  let getState: adaptedState => option(state);
  let createState: state => adaptedState;
  type action;
  type adaptedAction;
  let getAction: adaptedAction => option(action);
  let createAction: action => adaptedAction;
  let initialState: unit => state;
  let reducer: (action, state) => state;
  let epic: Most.stream(action) => Most.stream(action);
};

module Make = (M: Model) => {
  type state = M.state;
  type action = M.action;
  let initialState = () => M.createState(M.initialState());
  let reducer = (action, state) =>
    switch (M.getAction(action), M.getState(state)) {
    | (Some(a), Some(s)) => M.reducer(a, s) |. M.createState
    | _ => state
    };
  let epic = stream =>
    Most.(stream |> keepMap(M.getAction) |> M.epic |> map(M.createAction));
};
