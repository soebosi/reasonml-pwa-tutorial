open Util;

open AppModel;

let observe = (stream, send) => {
  stream
  |> filterMap(getTopPageAction)
  |> TopPageObserver.observe(action => send(TopPageAction(action)));
  stream
  |> filterMap(getItemPageAction)
  |> ItemPageObserver.observe(action => send(ItemPageAction(action)));
};
