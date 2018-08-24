let baseStyle = Css.[display(`flex), marginBottom(`rem(1.0))];

let component = ReasonReact.statelessComponent("MyFormGroup");

let make = children => {
  ...component,
  render: _self => <div className=(Css.style(baseStyle))> ...children </div>,
};
