open MostEx;

type state = {
  name: string,
  text: string,
  source: string,
};

let initialState = () => {name: "", text: "", source: ""};

[@bs.deriving accessors]
type action =
  | Initialize(string)
  | ChangeText(string)
  | ChangeSource(string);

let reducer = (action, state) =>
  switch (action) {
  | Initialize(name) => {...state, name}
  | ChangeText(text) => {...state, text}
  | ChangeSource(source) => {...state, source}
  };

let getChangeText = ((a, _s)) =>
  switch (a) {
  | ChangeText(text) => Some(text)
  | _ => None
  };

let epic = stream =>
  Most.(
    stream |> keepMap(getChangeText) |> debounce(200) |> map(changeSource)
  );
