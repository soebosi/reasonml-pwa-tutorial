open Belt;

type state = {
  name: string,
  nameSet: Set.String.t,
};

let initialState = () => {name: "", nameSet: Set.String.empty};

type action =
  | AddName(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(name) => ReasonReact.Update({...state, name})
  | AddName(name) =>
    let nameSet = Set.String.add(state.nameSet, name);
    ReasonReact.Update({...state, nameSet});
  };
