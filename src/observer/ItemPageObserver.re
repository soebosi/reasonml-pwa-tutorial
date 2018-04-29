open Util;

open ItemPageModel;

let observe = (stream, send) =>
  Most.(
    stream
    |> filterMap(getChangeText)
    |> debounce(200)
    |> observe(x => send(ChangeSource(x)))
  )
  |> ignore;
