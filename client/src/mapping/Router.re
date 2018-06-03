let getKey = (url: ReasonReact.Router.url) =>
  PageModel.(
    switch (url.path) {
    | ["items", name] => ItemPage(name)
    | [] => TopPage
    | _ => ErrorPage
    }
  );
