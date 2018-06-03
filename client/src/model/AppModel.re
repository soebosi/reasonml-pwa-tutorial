open Belt;

open MostEx;

type action = [ PageModel.adaptedAction | `ChangeUrl(ReasonReact.Router.url)];

let changeUrl = a => `ChangeUrl(a);

module PageCmp =
  Id.MakeComparable({
    type t = PageModel.key;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type state = {
  pageStates:
    Map.t(PageModel.key, PageModel.adaptedState, PageCmp.identity),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates: Map.make(~id=(module PageCmp)),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t(action) = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | `ChangeUrl(url) => {
      let key = Router.getKey @@ url;
      let pageStates =
        state.pageStates
        |. Map.updateU(key, (. value) =>
          switch(value) {
            | Some(v) => v |. Some
            | None => {
              let (module M) = PageModelMap.getModel(key);
              M.initialState() |. Some;
            }
          }
        );
      {...state, url, pageStates};
    }
    | _ => {
      let key = Router.getKey @@ state.url;
      let pageStates =
        state.pageStates
        |. Map.updateU(key, (. value) =>
          switch(value) {
            | Some(v) =>
              let (module M) = PageModelMap.getModel(key);
              M.reducer(action, v) |. Some;
            | None => None
          }
        );
      {...state, pageStates};
    }
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  [|PageModelMap.getModel(TopPage), PageModelMap.getModel(ItemPage(""))|]
  |. Array.mapU((. (module M): (module PageModel.T)) => M.epic(stream))
  |. Most.mergeArray;
