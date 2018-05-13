open Belt;

open MostEx;

let models = Adaptor.make();

type action = [
  AdaptedModel.adaptedAction
  | `ChangeUrl(ReasonReact.Router.url)
];

let changeUrl = a => `ChangeUrl(a);

type state = {
  pageStates: Map.String.t(AdaptedModel.adaptedState),
  url: ReasonReact.Router.url,
};

let initialState = () => {
  pageStates:
    Map.String.mapU(models, (. (module M): (module AdaptedModel.T)) =>
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
        Map.String.mapWithKeyU(
          models, (. key, (module M): (module AdaptedModel.T)) =>
          M.reducer(action, Map.String.getExn(state.pageStates, key))
        );
      {...state, pageStates};
    };
  ReasonReact.UpdateWithSideEffects(
    newState,
    _self => Most.Subject.next(action, actionSubject) |. ignore,
  );
};

let actionEpic = stream =>
  Map.String.mapU(models, (. (module M): (module AdaptedModel.T)) =>
    M.epic(stream)
  )
  |. Map.String.valuesToArray
  |. Most.mergeArray;
