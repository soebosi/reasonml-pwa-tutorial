open Belt;

open MostEx;

let models = AdaptedModelMap.make();

type action = [
  AdaptedModel.adaptedAction
  | `ChangeUrl(ReasonReact.Router.url)
];

let changeUrl = a => `ChangeUrl(a);

type state = {
  pageStates:
    Map.t(AdaptedModelMap.key, AdaptedModel.adaptedState, AdaptedModelMap.id),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates:
    models
    |. Map.mapU((. (module M): (module AdaptedModel.T)) => M.initialState()),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t(action) = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | `ChangeUrl(url) => {...state, url}
    | _ => {
        ...state,
        pageStates:
          models
          |. Map.mapWithKeyU((. key, (module M): (module AdaptedModel.T)) =>
               M.reducer(action, Map.getExn(state.pageStates, key))
             ),
      }
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  models
  |. Map.mapU((. (module M): (module AdaptedModel.T)) => M.epic(stream))
  |. Map.valuesToArray
  |. Most.mergeArray;
