open Util;

module Styles = {
  open Css;
  let ulist = style([listStyleType(none), maxWidth(px(400))]);
};

let component = ReasonReact.statelessComponent("MyList");

let make = (~items, children) => {
  ...component,
  render: _self => <ul className=Styles.ulist> (a_(items)) </ul>,
};
