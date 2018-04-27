open Belt;

type state = {
  name: string,
  nameSet: Set.String.t,
};

let initialState = () => {name: "", nameSet: Set.String.empty};

[@bs.deriving accessors]
type action =
  | AddName(string)
  | ChangeText(string);

let reducer = (action, state, store, _dispatch) =>
  switch (action) {
  | ChangeText(name) => ReasonReact.Update(store({...state, name}))
  | AddName(name) =>
    let nameSet = Set.String.add(state.nameSet, name);
    ReasonReact.Update(store({...state, nameSet}));
  };
