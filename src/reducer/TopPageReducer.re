open Belt;

type state = {
  name: string,
  nameArray: array(string),
};

let initialState = () => {
  {
    name: "",
    nameArray: [||],
  };
};

type action =
  | AddName(string)
  | ChangeText(string);

let reducer = (action, state) => {
  switch(action) {
  | ChangeText(name) => ReasonReact.Update({...state, name})
  | AddName(name) => {
    let nameArray = Array.concat(state.nameArray, [|name|])
      |. SortArray.stableSortBy(Pervasives.compare);
    ReasonReact.Update({...state, nameArray});
  }
  }
};
