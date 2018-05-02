open MostEx;

type state = {
  itemPageState: ItemPageModel.state,
  topPageState: TopPageModel.state,
  url: ReasonReact.Router.url,
};

let initialState = () => {
  itemPageState: ItemPageModel.initialState(),
  topPageState: TopPageModel.initialState(),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

[@bs.deriving accessors]
type action =
  | ItemPageAction(ItemPageModel.action)
  | TopPageAction(TopPageModel.action)
  | ChangeUrl(ReasonReact.Router.url);

let actionSubject = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | ItemPageAction(action) => {
        ...state,
        itemPageState: ItemPageModel.reducer(action, state.itemPageState),
      }
    | TopPageAction(action) => {
        ...state,
        topPageState: TopPageModel.reducer(action, state.topPageState),
      }
    | ChangeUrl(url) => {...state, url}
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream => {
  open Most;
  let topPageActionSubject = Most.Subject.make();
  let itemPageActionSubject = Most.Subject.make();
  stream
  |> observe(action =>
       switch (action) {
       | TopPageAction(action) =>
         Subject.next(action, topPageActionSubject) |. ignore
       | ItemPageAction(action) =>
         Subject.next(action, itemPageActionSubject) |. ignore
       | _ => ignore()
       }
     )
  |> ignore;
  mergeArray([|
    Subject.asStream(topPageActionSubject)
    |> TopPageModel.epic
    |> map(topPageAction),
    Subject.asStream(itemPageActionSubject)
    |> ItemPageModel.epic
    |> map(itemPageAction),
  |]);
};
