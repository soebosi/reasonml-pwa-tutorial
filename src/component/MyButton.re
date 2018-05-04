open Belt;

let baseStyle =
  Css.[
    fontSize(`rem(1.0)),
    lineHeight(1.5),
    padding2(`rem(0.375), `rem(0.75)),
    color(rgb(256, 256, 256)),
    border(px(1), `solid, rgb(180, 180, 180)),
    borderRadius(`rem(0.25)),
    backgroundColor(`rgb((0, 0, 139))),
    transition(~duration=150, ~timingFunction=`easeInOut, "background-color"),
    hover([backgroundColor(rgb(0, 100, 255))]),
    active([MyBoxShadow.style]),
  ];

let component = ReasonReact.statelessComponent("MyButton");

let make = (~style=?, children) => {
  ...component,
  render: _self => {
    let customStyle = Option.getWithDefault(style, []);
    let className = Css.style @@ Css.merge([customStyle, baseStyle]);
    ReasonReact.createDomElement(
      "button",
      ~props={"type": "submit", "className": className},
      children,
    );
  },
};
