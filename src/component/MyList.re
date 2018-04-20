open Util;

let baseStyle = Css.[listStyleType(none), maxWidth(px(400))];

let component = ReasonReact.statelessComponent("MyList");

let make = children => {
  ...component,
  render: _self =>
    ReasonReact.createDomElement(
      "ul",
      ~props={"className": Css.style(baseStyle)},
      children,
    ),
};
