let baseStyle =
  Css.[
    border(px(1), `solid, rgba(0, 0, 0, 0.15)),
    backgroundColor(`rgb((256, 256, 256))),
    padding2(`rem(0.75), `rem(1.25)),
    margin(`rem(0.50)),
    marginBottom(px(-1)),
    borderRadius(`rem(0.25)),
    boxShadow(~y=px(2), rgba(0, 0, 0, 0.12)),
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
