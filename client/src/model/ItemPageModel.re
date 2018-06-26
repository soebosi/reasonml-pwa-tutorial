open MostEx;

type state = {
  text: string,
  source: string,
};

let initialState = () => {text: "", source: ""};

[@bs.deriving accessors]
type action =
  | Initialize
  | ChangeText(string)
  | ChangeSource(string);

let reducer = (action, state) =>
  switch (action) {
  | Initialize => state
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
