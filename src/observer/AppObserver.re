open Util;

open AppModel;

let observe = stream =>
  Most.(
    mergeArray([|
      stream
      |> filterMap(getTopPageAction)
      |> TopPageObserver.observe
      |> map(topPageAction),
      stream
      |> filterMap(getItemPageAction)
      |> ItemPageObserver.observe
      |> map(itemPageAction),
    |])
  );
