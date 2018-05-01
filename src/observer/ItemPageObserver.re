open Util;

open ItemPageModel;

let observe = stream =>
  Most.(
    stream |> filterMap(getChangeText) |> debounce(200) |> map(changeSource)
  );
