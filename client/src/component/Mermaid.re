open Util;

module Mermaid = {
  [@bs.module "mermaid"]
  external init : ([@bs.as {json|{}|json}] _, Dom.element) => unit = "";
};

type action =
  | Dummy;

type state = {divRef: ref(option(Dom.element))};

let component = ReasonReact.reducerComponent("Mermaid");

let make = (~value, _children) => {
  ...component,
  initialState: () => {divRef: ref(None)},
  didMount: self =>
    switch (self.state.divRef^) {
    | Some(r) =>
      try (Mermaid.init(r)) {
      | Js.Exn.Error(e) =>
        switch (Js.Exn.message(e)) {
        | Some(message) => Js.log({j|message: $message|j})
        | None => ()
        }
      }
    | None => ()
    },
  reducer: (_action: action, state: state) => ReasonReact.Update(state),
  render: self =>
    <div ref=(theRef => self.state.divRef := Js.Nullable.toOption(theRef))>
      (s_(value))
    </div>,
};
