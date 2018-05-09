open Belt;

open MostEx;

let models = AdaptedModels.models;

type action = [
  AdaptedModels.adaptedAction
  | `ChangeUrl(ReasonReact.Router.url)
];

let changeUrl = a => `ChangeUrl(a);

type state = {
  pageStates: array(AdaptedModels.adaptedState),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates:
    Array.mapU(models, (. (module M): (module AdaptedModels.AdaptedModel)) =>
      M.initialState()
    ),
  url: ReasonReact.Router.dangerouslyGetInitialUrl(),
};

let actionSubject: Most.Subject.t(action) = Most.Subject.make();

let reducer = (action, state) => {
  let newState =
    switch (action) {
    | `ChangeUrl(url) => {...state, url}
    | _ =>
      let pageStates =
        Array.mapWithIndexU(
          models, (. i, (module M): (module AdaptedModels.AdaptedModel)) =>
          M.reducer(action, Option.getExn(state.pageStates[i]))
        );
      {...state, pageStates};
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  Array.mapU(models, (. (module M): (module AdaptedModels.AdaptedModel)) =>
    M.epic(stream)
  )
  |. Most.mergeArray;
