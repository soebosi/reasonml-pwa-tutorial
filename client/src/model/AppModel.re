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

type state = {
  pageStates:
    Belt.Map.t(PageModel.id, PageModel.adaptedState, PageCmp.identity),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates: Belt.Map.make(~id=(module PageCmp)),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t((action, option(PageModel.adaptedState))) =
  Most.Subject.make();

let reducer = (action, store) => {
  let pageID = Router.getPageStateID(store.url);
  let model = PageModelMap.getModel(pageID);
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
        pageStates: Belt.Map.update(store.pageStates, pageID, updater),
      };
    | (PageAction(_), None) => store
    };
  let newPageID = Router.getPageStateID(newStore.url);
  let pageState = Belt.Map.get(store.pageStates, newPageID);
  ReasonReact.UpdateWithSideEffects(
    newStore,
    _self => Most.Subject.next((action, pageState), actionSubject) |. ignore,
  );
};

let getPageAction = ((a, s)) =>
  switch (a) {
  | PageAction(a) => Some((a, s))
  | _ => None
  };

let getStateIDFromChangeUrl = a =>
  switch (a) {
  | ChangeUrl(url) => Some(Router.getPageStateID(url))
  | _ => None
  };

let getActionIfPageStateIsEmpty = ((a, s)) =>
  switch (s) {
  | None => Some(a)
  | _ => None
  };

let epic = stream =>
  PageModelMap.models
  |. Belt.Array.mapU((. (module M): (module PageModel.T)) =>
       stream |> Most.keepMap(getPageAction) |> M.epic
     )
  |. Belt.Array.concat([|
       stream
       |> Most.keepMap(getActionIfPageStateIsEmpty)
       |> Most.keepMap(getStateIDFromChangeUrl)
       |> Router.epic,
     |])
  |> Most.mergeArray
  |> Most.map(a => PageAction(a));
