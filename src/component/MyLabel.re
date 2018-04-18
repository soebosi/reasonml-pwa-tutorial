open Util;

module Styles = {
  open Css;
  let label = style([display(`inlineBlock), marginBottom(`rem(0.5))]);
};

let component = ReasonReact.statelessComponent("MyLabel");

let make = (~htmlFor, ~text, children) => {
  ...component,
  render: _self => <label className=Styles.label htmlFor> (s_(text)) </label>,
};
