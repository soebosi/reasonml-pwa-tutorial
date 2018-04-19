open Util;

module Styles = {
  open Css;
  let div = style([marginBottom(`rem(1.0))]);
};

let component = ReasonReact.statelessComponent("MyFormGroup");

let make = children => {
  ...component,
  render: self =>
    ReasonReact.createDomElement(
      "div",
      ~props={"className": Styles.div},
      children,
    ),
};
