open MostEx;

type state = {
  id: string,
  name: string,
  text: string,
  source: string,
};

let initialState = () => {id: "", name: "", text: "", source: ""};

[@bs.deriving accessors]
type action =
  | Initialize(string, string)
  | ChangeText(string)
  | ChangeSource(string);

let reducer = (action, state) =>
  switch (action) {
  | Initialize(id, name) => {...state, id, name}
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
