let baseStyle =
  Css.[
    fontSize(`rem(1.0)),
    lineHeight(1.5),
    padding2(`rem(0.375), `rem(0.75)),
    color(rgb(256, 256, 256)),
    border(px(1), `solid, rgb(180, 180, 180)),
    borderRadius(`rem(0.25)),
    transition(~duration=150, ~timingFunction=`easeInOut, "background-color"),
    active([MyBoxShadow.style]),
  ];

type type_ =
  | Add
  | Remove;

let component = ReasonReact.statelessComponent("MyButton");

let baseColor = type_ =>
  switch (type_) {
  | Add =>
    Css.[
      backgroundColor(`rgb((0, 0, 139))),
      hover([backgroundColor(rgb(0, 0, 105))]),
    ]
  | Remove =>
    Css.[
      backgroundColor(`rgb((250, 50, 50))),
      hover([backgroundColor(rgb(230, 0, 0))]),
    ]
  };

let make = (~style=?, ~type_, children) => {
  ...component,
  render: _self => {
    let customStyle = Belt.Option.getWithDefault(style, []);
    let className =
      Css.style @@ Css.merge([customStyle, baseColor(type_), baseStyle]);
    ReasonReact.createDomElement(
      "button",
      ~props={"type": "submit", "className": className},
      children,
    );
  },
};
