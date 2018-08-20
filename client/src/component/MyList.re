let baseStyle = Css.[listStyleType(none)];

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
