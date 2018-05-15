open MostEx;

open Belt;

type state = {
  text: string,
  itemSet: Set.String.t,
};

let initialState = () => {text: "", itemSet: Set.String.empty};

[@bs.deriving accessors]
type action =
  | AddItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | AddItem(name) =>
    let itemSet = Set.String.add(state.itemSet, name);
    {...state, itemSet};
  };

let getChangeText = x =>
  switch (x) {
  | ChangeText(text) => Some(text)
  | _ => None
  };

let epic = stream => Most.(stream |> filter(_a => false));
