module AdaptedTopPageModel =
  PageModel.Make({
    include TopPageModel;
    let adaptState = PageModel.topPageState;
    let getState = a =>
      switch (a) {
      | PageModel.TopPageState(a) => Some(a)
      | _ => None
      };
    let adaptAction = a => `TopPageAction(a);
    let getAction = a =>
      switch (a) {
      | `TopPageAction(a) => Some(a)
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
    let adaptAction = a => `ItemPageAction(a);
    let getAction = a =>
      switch (a) {
      | `ItemPageAction(a) => Some(a)
      | _ => None
      };
  });

let getModel = (key: PageModel.key) : (module PageModel.T) =>
  switch (key) {
  | TopPage => (module AdaptedTopPageModel)
  | ItemPage(_) => (module AdaptedItemPageModel)
  | ErrorPage => raise(Not_found)
  };

let models: array((module PageModel.T)) = [|
  (module AdaptedTopPageModel),
  (module AdaptedItemPageModel),
|];
