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
  | RemoveItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | RemoveItem(name) =>
    let itemSet = Set.String.remove(state.itemSet, name);
    {...state, itemSet};
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
