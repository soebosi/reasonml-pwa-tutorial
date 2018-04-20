open Util;

let baseStyle = Css.[display(`inlineBlock), marginBottom(`rem(0.5))];

let component = ReasonReact.statelessComponent("MyLabel");

let make = (~htmlFor, ~text, children) => {
  ...component,
  render: _self =>
    <label className=(Css.style(baseStyle)) htmlFor> (s_(text)) </label>,
};
