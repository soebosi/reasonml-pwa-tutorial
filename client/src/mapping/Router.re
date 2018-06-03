let getKey = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | ["items", name] => PageModel.ItemPage(name)
  | [] => PageModel.TopPage
  | _ => PageModel.ErrorPage
  };
