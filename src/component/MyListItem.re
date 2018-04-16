open Util;

module Styles = {
  open Css;
  let li =
    style([
      firstChild([
        borderTopRightRadius(`rem(0.25)),
        borderTopLeftRadius(`rem(0.25)),
      ]),
      border(px(1), `solid, rgba(0, 0, 0, 0.15)),
      backgroundColor(`rgb((256, 256, 256))),
      padding2(`rem(0.75), `rem(1.25)),
      marginBottom(px(-1)),
      lastChild([
        borderBottomRightRadius(`rem(0.25)),
        borderBottomLeftRadius(`rem(0.25)),
      ]),
    ]);
};

let component = ReasonReact.statelessComponent("MyListItem");

let make = (~id, ~text, _children) => {
  ...component,
  render: self => <li key=id className=Styles.li> (s_(text)) </li>,
};
