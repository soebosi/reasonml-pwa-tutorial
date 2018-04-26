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

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(name) => {...state, name}
  | AddName(name) =>
    let nameSet = Set.String.add(state.nameSet, name);
    {...state, nameSet};
  };
