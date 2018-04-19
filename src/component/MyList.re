open Util;

module Styles = {
  open Css;
  let ulist = style([listStyleType(none), maxWidth(px(400))]);
};

let component = ReasonReact.statelessComponent("MyList");

let make = children => {
  ...component,
  render: _self =>
    ReasonReact.createDomElement(
      "ul",
      ~props={"className": Styles.ulist},
      children,
    ),
};
