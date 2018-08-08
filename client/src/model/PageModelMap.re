module AdaptedTopPageModel =
  PageModel.Make({
    include TopPageModel;
    let adaptState = PageModel.topPageState;
    let getState = a =>
      switch (a) {
      | PageModel.TopPageState(a) => Some(a)
      | _ => None
      };
    let adaptAction = a => PageModel.TopPageAction(a);
    let getAction = a =>
      switch (a) {
      | PageModel.TopPageAction(a) => Some(a)
      | _ => None
      };
  });
module AdaptedItemPageModel =
  PageModel.Make({
    include ItemPageModel;
    let adaptState = PageModel.itemPageState;
    let getState = a =>
      switch (a) {
      | PageModel.ItemPageState(a) => Some(a)
      | _ => None
      };
    let adaptAction = a => PageModel.ItemPageAction(a);
    let getAction = a =>
      switch (a) {
      | PageModel.ItemPageAction(a) => Some(a)
      | _ => None
      };
  });

let getModel = (id: PageModel.id) : option((module PageModel.T)) =>
  switch (id) {
  | TopPage => Some((module AdaptedTopPageModel))
  | ItemPage(_) => Some((module AdaptedItemPageModel))
  | ErrorPage => None
  };

let models: array((module PageModel.T)) = [|
  (module AdaptedTopPageModel),
  (module AdaptedItemPageModel),
|];
