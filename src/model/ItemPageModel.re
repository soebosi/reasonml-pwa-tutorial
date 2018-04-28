type state = {
  text: string,
  source: string,
};

let initialState = () => {text: "", source: ""};

[@bs.deriving accessors]
type action =
  | ChangeText(string)
  | ChangeSource(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | ChangeSource(source) => {...state, source}
  };
