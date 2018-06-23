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
  | CreateItem(string)
  | CreatedItem(ItemModel.t)
  | DeleteItem(string)
  | DeletedItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | DeletedItem(id) =>
    let itemMap = Map.remove(state.itemMap, id);
    {...state, itemMap};
  | CreatedItem(item) =>
    let id = ItemModel.id(item);
    let itemMap = Map.set(state.itemMap, id, item);
    {...state, itemMap};
  | _ => state
  };

let getAddItem = x =>
  switch (x) {
  | CreateItem(name) => Some(name)
  | _ => None
  };

let getDeleteItem = x =>
  switch (x) {
  | DeleteItem(id) => Some(id)
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getAddItem)
      |> flatMap(fromPromise << ItemModel.create)
      |> map(createdItem),
      stream
      |> keepMap(getDeleteItem)
      |> flatMap(fromPromise << ItemModel.delete)
      |> map(deletedItem),
    |])
  );
