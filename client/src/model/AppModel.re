open MostEx;

[@bs.deriving accessors]
type action =
  | ChangeUrl(ReasonReact.Router.url)
  | PageAction(PageModel.adaptedAction);

module PageCmp =
  Belt.Id.MakeComparable({
    type t = PageModel.id;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type store = {
  pageStates:
    Belt.Map.t(PageModel.id, PageModel.adaptedState, PageCmp.identity),
  url: ReasonReact.Router.url,
};

let initialStore = () => {
  pageStates: Belt.Map.make(~id=(module PageCmp)),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t((action, store)) = Most.Subject.make();

let reducer = (action, store) => {
  let pageStateID = Router.getPageStateID(store.url);
  let model = PageModelMap.getModel(pageStateID);
  let newStore =
    switch (action, model) {
    | (ChangeUrl(url), _) => {...store, url}
    | (PageAction(action), Some((module M))) =>
      let updater = nullableState => {
        let state =
          Belt.Option.getWithDefault(nullableState, M.initialState());
        Some(M.reducer(action, state));
      };
      {
        ...store,
        pageStates: Belt.Map.update(store.pageStates, pageStateID, updater),
      };
    | (PageAction(_), None) => store
    };
  ReasonReact.UpdateWithSideEffects(
    newStore,
    _self => Most.Subject.next((action, newStore), actionSubject) |. ignore,
  );
};

let getCurrentPageState = store => {
  let id = Router.getPageStateID(store.url);
  Belt.Map.get(store.pageStates, id);
};

let getPageActionAndState = ((action, store)) =>
  switch (action) {
  | PageAction(a) => Some((a, getCurrentPageState(store)))
  | _ => None
  };

let getActionIfPageStateIsEmpty = ((action, store)) => {
  let state = getCurrentPageState(store);
  switch (state) {
  | None => Some(action)
  | _ => None
  };
};

let getStateIDFromChangeUrl = a =>
  switch (a) {
  | ChangeUrl(url) => Some(Router.getPageStateID(url))
  | _ => None
  };

let epic = stream =>
  PageModelMap.models
  |. Belt.Array.mapU((. (module M): (module PageModel.T)) =>
       stream |> Most.keepMap(getPageActionAndState) |> M.epic
     )
  |. Belt.Array.concat([|
       stream
       |> Most.keepMap(getActionIfPageStateIsEmpty)
       |> Most.keepMap(getStateIDFromChangeUrl)
       |> Router.epic,
     |])
  |> Most.mergeArray
  |> Most.map(a => PageAction(a));
