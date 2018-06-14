open Util;

[@bs.deriving abstract]
type t = {
  id: string,
  name: string,
};

[@bs.scope "JSON"] [@bs.val] external parseIntoItem : string => t = "parse";

let createItem = name => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "name", Js.Json.string(name));
  Fetch.fetchWithInit(
    "/api/v1/items/",
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=
        Fetch.BodyInit.make(Js.Json.stringify @@ Js.Json.object_(payload)),
      ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
      (),
    ),
  )
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(Js.Promise.resolve << parseIntoItem);
};
