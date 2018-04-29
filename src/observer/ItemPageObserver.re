open Util;

open ItemPageModel;

let observe = (stream, send) =>
  Most.(
    stream
    |> filterMap(x =>
         switch (x) {
         | ChangeText(text) => Some(text)
         | _ => None
         }
       )
    |> debounce(200)
    |> observe(x => send(ChangeSource(x)))
  )
  |> ignore;
