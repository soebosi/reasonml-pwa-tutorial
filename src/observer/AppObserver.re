open Util;

open AppModel;

let observe = (stream, send) => {
  stream
  |> filterMap(getTopPageAction)
  |> TopPageObserver.observe(send @@@ topPageAction);
  stream
  |> filterMap(getItemPageAction)
  |> ItemPageObserver.observe(send @@@ itemPageAction);
};
