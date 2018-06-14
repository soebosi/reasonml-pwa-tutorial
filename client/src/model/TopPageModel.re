open MostEx;

open Belt;

open Util;

type item = ItemModel.item;
let id = ItemModel.id;
let name = ItemModel.name;

module ItemCmp =
  Id.MakeComparable({
    type t = string;
    let cmp = Pervasives.compare;
  });

type state = {
  text: string,
  itemMap: Map.t(ItemCmp.t, item, ItemCmp.identity),
};

let initialState = () => {text: "", itemMap: Map.make(~id=(module ItemCmp))};

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
    let itemMap = Map.remove(state.itemMap, id);
    {...state, itemMap};
  | AddedItem(item) =>
    let itemMap = Map.set(state.itemMap, item |. id, item);
    {...state, itemMap};
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
    |> flatMap(fromPromise << ItemModel.createItem)
    |> map(addedItem)
  );
