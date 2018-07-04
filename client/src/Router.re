open Util;

open MostEx;

let getPageStateID = (url: ReasonReact.Router.url) =>
  PageModel.(
    switch (url.path) {
    | [] => TopPage
    | ["items", id] => ItemPage(id)
    | _ => ErrorPage
    }
  );

let getURL = id =>
  PageModel.(
    switch (id) {
    | TopPage => "/"
    | ItemPage(name) => {j|/items/$name|j}
    | _ => ""
    }
  );

let getTopPageID = ((stateID, state)) =>
  switch (stateID, state) {
  | (PageModel.TopPage, None) => Some(stateID)
  | _ => None
  };

let getItemPageID = ((stateID, state)) =>
  switch (stateID, state) {
  | (PageModel.ItemPage(id), None) => Some(id)
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getTopPageID)
      |> map(_ => TopPageModel.Initialize)
      |> map(a => `TopPageAction(a)),
      stream
      |> keepMap(getItemPageID)
      |> flatMap(fromPromise << ItemModel.retrieve)
      |> map(ItemPageModel.initialize << ItemModel.name)
      |> map(a => `ItemPageAction(a)),
    |])
  );
