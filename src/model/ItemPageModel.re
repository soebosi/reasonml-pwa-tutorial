type state = {text: string};

let initialState = () => {text: ""};

[@bs.deriving accessors]
type action =
  | ChangeText(string);

let reducer = (action, _state, store, _dispatch) =>
  switch (action) {
  | ChangeText(text) => ReasonReact.Update(store({text: text}))
  };
