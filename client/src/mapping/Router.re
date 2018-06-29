open Util;

open MostEx;

let getStateID = (url: ReasonReact.Router.url) =>
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
    | ItemPage(name) => "/items/" ++ name
    | _ => ""
    }
  );

let getTopPageID = ((id, state)) =>
  switch (id, state) {
  | (PageModel.TopPage, None) => Some(id)
  | _ => None
  };

let getItemPageID = ((id, state)) =>
  switch (id, state) {
  | (PageModel.ItemPage(id), None) => Some(id)
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getTopPageID)
      |> map(id => (id, `TopPageAction(TopPageModel.Initialize))),
      stream
      |> keepMap(getItemPageID)
      |> flatMap(fromPromise << ItemModel.retrieve)
      |> map(item => {
           let id = ItemModel.id(item) |. PageModel.itemPage;
           let name = ItemModel.name(item);
           (id, `ItemPageAction(ItemPageModel.Initialize(name)));
         }),
    |])
  );
