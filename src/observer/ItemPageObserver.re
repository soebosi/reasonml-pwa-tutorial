open Util;

open ItemPageModel;

let observe = (send, stream) =>
  Most.(
    stream
    |> filterMap(getChangeText)
    |> debounce(200)
    |> observe(send << changeSource)
    |> ignore
  );
