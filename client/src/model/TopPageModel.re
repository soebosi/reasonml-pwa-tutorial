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
  | Initialize(array(ItemModel.t))
  | CreateItem(string)
  | CreatedItem(ItemModel.t)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | Initialize(items) =>
    let itemMap =
      items
      |. Belt.Array.map(item => (item |. ItemModel.idGet, item))
      |. Belt.Map.fromArray(~id=(module ItemCmp));
    {...state, itemMap};
  | ChangeText(text) => {...state, text}
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

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getAddItem)
      |> flatMap(fromPromise << ItemModel.create)
      |> map(createdItem),
    |])
  );
