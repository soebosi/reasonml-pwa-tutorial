open Util;

[@bs.deriving abstract]
type t = {
  id: string,
  name: string,
  text: string,
};

[@bs.scope "JSON"] [@bs.val] external parseIntoItem : string => t = "parse";
[@bs.scope "JSON"] [@bs.val]
external parseIntoItems : string => array(t) = "parse";

let create = name => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "name", Js.Json.string(name));
  Js.Dict.set(payload, "text", Js.Json.string(""));
  Dom.Storage.setItem("id", dict2string(payload), Dom.Storage.localStorage);
  Js.Promise.make((~resolve, ~reject as _) =>
    resolve(. t(~id="id", ~name="name", ~text="text"))
  );
};

let retrieveAll = () => {
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  Fetch.fetchWithInit(
    {j|/api/v1/items|j},
    Fetch.RequestInit.make(~method_=Get, ~headers, ()),
  )
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(Js.Promise.resolve << parseIntoItems);
};

let retrieve = id => {
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  Fetch.fetchWithInit(
    {j|/api/v1/items/$id|j},
    Fetch.RequestInit.make(~method_=Get, ~headers, ()),
  )
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(Js.Promise.resolve << parseIntoItem);
};

let update = (id, name, text) => {
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "name", Js.Json.string(name));
  Js.Dict.set(payload, "text", Js.Json.string(text));
  let body = Fetch.BodyInit.make @@ dict2string(payload);
  Fetch.fetchWithInit(
    {j|/api/v1/items/$id|j},
    Fetch.RequestInit.make(~method_=Put, ~body, ~headers, ()),
  )
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(Js.Promise.resolve << parseIntoItem);
};

let delete = id => {
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  Fetch.fetchWithInit(
    {j|/api/v1/items/$id|j},
    Fetch.RequestInit.make(~method_=Delete, ~headers, ()),
  )
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(_ => Js.Promise.resolve(id));
};
