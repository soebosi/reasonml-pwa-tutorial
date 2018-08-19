open MostEx;

type state = {
  id: string,
  name: string,
  text: string,
  source: string,
};

let updateSourceDurationMS = 400;

let initialState = () => {id: "", name: "", text: "", source: ""};

[@bs.deriving accessors]
type action =
  | Initialize(ItemModel.t)
  | ChangeText(string)
  | ChangeSource(string)
  | UpdatedItem;

let reducer = (action, state) =>
  switch (action) {
  | Initialize(item) => {
      ...state,
      id: ItemModel.idGet(item),
      name: ItemModel.nameGet(item),
    }
  | ChangeText(text) => {...state, text}
  | ChangeSource(source) => {...state, source}
  | UpdatedItem => state
  };

let getChangeText = ((a, _s)) =>
  switch (a) {
  | ChangeText(text) => Some(text)
  | _ => None
  };

let getStateWhenChangeSource = ((a, s)) =>
  switch (a) {
  | ChangeSource(_) => s
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getChangeText)
      |> debounce(updateSourceDurationMS)
      |> map(changeSource),
      stream
      |> keepMap(getStateWhenChangeSource)
      |> map(s => ItemModel.update(s.id, s.name, s.text))
      |> flatMap(fromPromise)
      |> map(_ => UpdatedItem),
    |])
  );
