let baseStyle = Css.[marginBottom(`rem(1.0))];

let component = ReasonReact.statelessComponent("MyFormGroup");

let make = children => {
  ...component,
  render: _self =>
    ReasonReact.createDomElement(
      "div",
      ~props={"className": Css.style(baseStyle)},
      children,
    ),
};
