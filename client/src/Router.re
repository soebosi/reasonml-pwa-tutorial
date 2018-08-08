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

let getItemPageID = stateID =>
  switch (stateID) {
  | PageModel.ItemPage(id) => Some(id)
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> filter((==)(PageModel.TopPage))
      |> map(_ => TopPageModel.Initialize)
      |> map(a => PageModel.TopPageAction(a)),
      stream
      |> keepMap(getItemPageID)
      |> flatMap(fromPromise << ItemModel.retrieve)
      |> map(ItemPageModel.initialize)
      |> map(a => PageModel.ItemPageAction(a)),
    |])
  );
