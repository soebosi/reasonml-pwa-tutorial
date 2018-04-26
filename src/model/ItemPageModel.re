open Belt;

type state = {text: string};

let initialState = () => {text: ""};

[@bs.deriving accessors]
type action =
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  };
