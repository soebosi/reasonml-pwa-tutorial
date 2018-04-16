open Util;

module Styles = {
  open Css;
  let li = style([border(px(1), `solid, rgba(0, 0, 0, 0.3))]);
};

let component = ReasonReact.statelessComponent("MyListItem");

let make = (~id, ~text, _children) => {
  ...component,
  render: self => <li key=id className=Styles.li> (s_(text)) </li>,
};
