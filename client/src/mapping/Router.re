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

let getChangeUrl = ((a, s)) =>
  switch (a) {
  | `ChangeUrl(url) => Some((getStateID(url), s))
  | _ => None
  };

let epic = stream =>
  Most.(
    mergeArray([|
      stream
      |> keepMap(getChangeUrl)
      |> keepMap(((id, state)) =>
           switch (id, state) {
           | (PageModel.TopPage, None) =>
             Some(
               `InitialPageState((
                 id,
                 `TopPageAction(TopPageModel.Initialize),
               )),
             )
           | (PageModel.ItemPage(_), None) =>
             Some(
               `InitialPageState((
                 id,
                 `ItemPageAction(ItemPageModel.Initialize),
               )),
             )
           | _ => None
           }
         ),
    |])
  );
