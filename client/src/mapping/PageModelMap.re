module PageCmp =
  Belt.Id.MakeComparable({
    type t = PageModel.key;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type id = PageCmp.identity;

let m: Belt.Map.t(PageCmp.t, (module PageModel.T), id) =
  Belt.Map.make(~id=(module PageCmp));

let getModel = (key: PageModel.key): (module PageModel.T) =>
  switch (key) {
  | TopPage =>
     (module
      PageModel.Make({
        open PageModel;
        include TopPageModel;
        let adaptState = topPageState;
        let getState = a =>
          switch (a) {
          | TopPageState(a) => Some(a)
          | _ => None
          };
        let adaptAction = a => `TopPageAction(a);
        let getAction = a =>
          switch (a) {
          | `TopPageAction(a) => Some(a)
          | _ => None
          };
      }))
     | ItemPage(_) =>
       (module
        PageModel.Make({
          open PageModel;
          include ItemPageModel;
          let adaptState = itemPageState;
          let getState = a =>
            switch (a) {
            | ItemPageState(a) => Some(a)
            | _ => None
            };
          let adaptAction = a => `ItemPageAction(a);
          let getAction = a =>
            switch (a) {
            | `ItemPageAction(a) => Some(a)
            | _ => None
            };
        }))
    };

let models = [|getModel(TopPage), getModel(ItemPage(""))|];
