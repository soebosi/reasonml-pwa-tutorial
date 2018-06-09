open MostEx;

open Belt;

[@bs.deriving abstract]
type item = {
  id: string,
  name: string,
};

[@bs.scope "JSON"] [@bs.val] external parseIntoItem : string => item = "parse";

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
    |> flatMap(name => {
         let payload = Js.Dict.empty();
         Js.Dict.set(payload, "name", Js.Json.string(name));
         fromPromise @@
         Js.Promise.(
           Fetch.(
             fetchWithInit(
               "/api/v1/items/",
               RequestInit.make(
                 ~method_=Post,
                 ~body=
                   Fetch.BodyInit.make(
                     Js.Json.stringify(Js.Json.object_(payload)),
                   ),
                 ~headers=
                   Fetch.HeadersInit.make({
                     "Content-Type": "application/json",
                   }),
                 (),
               ),
             )
           )
           |> then_(Fetch.Response.text)
         );
       })
    |> map(res => addedItem @@ parseIntoItem(res))
  );
