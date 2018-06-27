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
  | (PageModel.ItemPage(_), None) => Some(id)
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getTopPageID)
      |> map(id =>
           `InitialPageState((id, `TopPageAction(TopPageModel.Initialize)))
         ),
      stream
      |> keepMap(getItemPageID)
      |> map(id =>
           `InitialPageState((id, `ItemPageAction(ItemPageModel.Initialize)))
         ),
    |])
  );
