open MostEx;

open Belt;

open Util;

type item = HttpClient.item;
let id = HttpClient.id;
let name = HttpClient.name;

module ItemCmp =
  Id.MakeComparable({
    type t = string;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type state = {
  text: string,
  itemSet: Map.t(ItemCmp.t, item, ItemCmp.identity),
};

let initialState = () => {text: "", itemSet: Map.make(~id=(module ItemCmp))};

[@bs.deriving accessors]
type action =
  | AddItem(string)
  | AddedItem(item)
  | RemoveItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | RemoveItem(id) =>
    let itemSet = Map.remove(state.itemSet, id);
    {...state, itemSet};
  | AddedItem(item) =>
    let itemSet = Map.set(state.itemSet, item |. id, item);
    {...state, itemSet};
  | _ => state
  };

let getAddItem = x =>
  switch (x) {
  | AddItem(text) => Some(text)
  | _ => None
  };

let epic = stream =>
  Most.(
    stream
    |> keepMap(getAddItem)
    |> flatMap(fromPromise << HttpClient.postNewItem)
    |> map(addedItem)
  );
