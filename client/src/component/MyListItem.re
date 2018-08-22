let baseStyle =
  Css.[
    border(px(1), `solid, rgba(0, 0, 0, 0.15)),
    backgroundColor(`rgb((256, 256, 256))),
    padding2(`rem(0.75), `rem(1.25)),
    marginBottom(px(-1)),
  ];

let component = ReasonReact.statelessComponent("MyListItem");

let make = children => {
  ...component,
  render: _self =>
    ReasonReact.createDomElement(
      "li",
      ~props={"className": Css.style(baseStyle)},
      children,
    ),
};
