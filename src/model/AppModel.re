open Belt;

type state = {
  itemPage: ItemPageModel.state,
  topPage: TopPageModel.state,
  url: ReasonReact.Router.url,
};

let initialState = () => {
  itemPage: ItemPageModel.initialState(),
  topPage: TopPageModel.initialState(),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

[@bs.deriving accessors]
type action =
  | ItemPageAction(ItemPageModel.action)
  | TopPageAction(TopPageModel.action)
  | ChangeUrl(ReasonReact.Router.url);

let sub = Most.Subject.make();

let filterMap = (f, stream) =>
  Most.(
    stream
    |> filter(x =>
         switch (f(x)) {
         | Some(_) => true
         | None => false
         }
       )
    |> map(x => Option.getExn(f(x)))
  );

let middleware = (send, stream) =>
  Most.(
    stream
    |> filterMap(x =>
         switch (x) {
         | ItemPageAction(ChangeText(text)) => Some(text)
         | _ => None
         }
       )
    |> debounce(200)
    |> observe(x => send(ItemPageAction(ChangeSource(x))))
  );

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | ItemPageAction(action) => {
        ...state,
        itemPage: ItemPageModel.reducer(action, state.itemPage),
      }
    | TopPageAction(action) => {
        ...state,
        topPage: TopPageModel.reducer(action, state.topPage),
      }
    | ChangeUrl(url) => {...state, url}
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, sub) |> ignore,
  );
};
