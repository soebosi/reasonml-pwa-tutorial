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
  | AddedItem(string)
  | RemoveItem(string)
  | ChangeText(string);

let reducer = (action, state) =>
  switch (action) {
  | ChangeText(text) => {...state, text}
  | RemoveItem(name) =>
    let itemSet = Set.String.remove(state.itemSet, name);
    {...state, itemSet};
  | AddedItem(id) =>
    let itemSet = Set.String.add(state.itemSet, id);
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
           |> then_(Fetch.Response.json)
         );
       })
    |> keepMap(response =>
         switch (Js.Json.decodeObject(response)) {
         | Some(dict) =>
           switch (Js.Dict.get(dict, "id")) {
           | Some(str) => Js.Json.decodeString(str)
           | None => None
           }
         | None => None
         }
       )
    |> map(id => addedItem(id))
  );
