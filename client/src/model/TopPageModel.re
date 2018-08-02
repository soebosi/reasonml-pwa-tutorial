open MostEx;

open Util;

module ItemCmp =
  Belt.Id.MakeComparable({
    type t = string;
    let cmp = Pervasives.compare;
  });

type state = {
  text: string,
  itemMap: Belt.Map.t(ItemCmp.t, ItemModel.t, ItemCmp.identity),
};

let initialState = () => {
  text: "",
  itemMap: Belt.Map.make(~id=(module ItemCmp)),
};

[@bs.deriving accessors]
type action =
  | Initialize
  | CreateItem(string)
  | CreatedItem(ItemModel.t)
  | DeleteItem(string)
  | DeletedItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | Initialize => state
  | ChangeText(text) => {...state, text}
  | DeletedItem(id) =>
    let itemMap = Belt.Map.remove(state.itemMap, id);
    {...state, itemMap};
  | CreatedItem(item) =>
    let id = ItemModel.idGet(item);
    let itemMap = Belt.Map.set(state.itemMap, id, item);
    {...state, itemMap};
  | _ => state
  };

let getAddItem = ((a, _s)) =>
  switch (a) {
  | CreateItem(name) => Some(name)
  | _ => None
  };

let getDeleteItem = ((a, _s)) =>
  switch (a) {
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
