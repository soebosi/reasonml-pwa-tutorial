open Util;

module Mermaid = {
  [@bs.module "mermaid"]
  external init : ([@bs.as {json|{}|json}] _, Dom.element) => unit = "";
};

type action =
  | Dummy;

type state = {
  divRef: ref(option(Dom.element)),
  timer: ref(option(Js.Global.timeoutId)),
};

let component = ReasonReact.reducerComponent("Mermaid");

let make = (~value, _children) => {
  ...component,
  initialState: () => {divRef: ref(None), timer: ref(None)},
  willUnmount: self =>
    switch (self.state.timer^) {
    | Some(t) => Js.Global.clearTimeout(t)
    | None => ()
    },
  didMount: self =>
    self.state.timer :=
      Some(
        Js.Global.setTimeout(
          () => {
            Js.log(value);
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
            };
          },
          1000,
        ),
      ),
  reducer: (_action: action, state: state) => ReasonReact.Update(state),
  render: self =>
    <div ref=(theRef => self.state.divRef := Js.Nullable.toOption(theRef))>
      (s_(value))
    </div>,
};
