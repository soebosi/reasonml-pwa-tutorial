open MostEx;

open Belt;

open Util;

module ItemCmp =
  Id.MakeComparable({
    type t = string;
    let cmp = Pervasives.compare;
  });

type state = {
  text: string,
  itemMap: Map.t(ItemCmp.t, ItemModel.t, ItemCmp.identity),
};

let initialState = () => {text: "", itemMap: Map.make(~id=(module ItemCmp))};

[@bs.deriving accessors]
type action =
  | AddItem(string)
  | AddedItem(ItemModel.t)
  | RemoveItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | RemoveItem(id) =>
    let itemMap = Map.remove(state.itemMap, id);
    {...state, itemMap};
  | AddedItem(item) =>
    let itemMap = Map.set(state.itemMap, ItemModel.id(item), item);
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
    |> flatMap(fromPromise << ItemModel.create)
    |> map(addedItem)
  );
