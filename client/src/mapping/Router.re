let getModelId = (url: ReasonReact.Router.url) =>
  PageModel.(
    switch (url.path) {
    | [] => TopPage
    | ["items", name] => ItemPage(name)
    | _ => ErrorPage
    }
  );
