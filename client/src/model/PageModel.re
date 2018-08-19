open MostEx;

[@bs.deriving accessors]
type id =
  | TopPage
  | ItemPage(string)
  | ErrorPage;

[@bs.deriving accessors]
type adaptedAction =
  | TopPageAction(TopPageModel.action)
  | ItemPageAction(ItemPageModel.action);

[@bs.deriving accessors]
type adaptedState =
  | TopPageState(TopPageModel.state)
  | ItemPageState(ItemPageModel.state);

module type Model = {
  type action;
  type state;
  let initialState: unit => state;
  let reducer: (action, state) => state;
  let epic: Most.stream((action, state)) => Most.stream(action);
  let getState: adaptedState => option(state);
  let adaptState: state => adaptedState;
  let getAction: adaptedAction => option(action);
  let adaptAction: action => adaptedAction;
};

module type T = {
  let initialState: unit => adaptedState;
  let reducer: (adaptedAction, adaptedState) => adaptedState;
  let epic:
    Most.stream((adaptedAction, adaptedState)) => Most.stream(adaptedAction);
};

module Make = (M: Model) : T => {
  let initialState = () => M.(initialState() |. adaptState);
  let reducer = (action: adaptedAction, state) =>
    switch (M.(getAction(action), getState(state))) {
    | (Some(a), Some(s)) => M.(reducer(a, s) |. adaptState)
    | _ => state
    };
  let epic = stream =>
    Most.(
      stream
      |> keepMap(((action, state)) =>
           switch (M.(getAction(action), getState(state))) {
           | (Some(a), Some(s)) => Some((a, s))
           | _ => None
           }
         )
      |> M.epic
      |> map(M.adaptAction)
    );
};
