open Belt;

open MostEx;

let models = Adaptor.models;

type action = [ Adaptor.adaptedAction | `ChangeUrl(ReasonReact.Router.url)];

let changeUrl = a => `ChangeUrl(a);

type state = {
  pageStates: array(Adaptor.adaptedState),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates:
    Array.mapU(models, (. (module M): (module Adaptor.AdaptedModel)) =>
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
          models, (. i, (module M): (module Adaptor.AdaptedModel)) =>
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
  Array.mapU(models, (. (module M): (module Adaptor.AdaptedModel)) =>
    M.epic(stream)
  )
  |. Most.mergeArray;
