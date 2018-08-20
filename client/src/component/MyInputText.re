let baseStyle =
  Css.[
    display(`inline),
    fontSize(`rem(1.0)),
    lineHeight(1.5),
    padding2(`rem(0.375), `rem(0.75)),
    border(px(1), `solid, rgb(180, 180, 180)),
    borderRadius(`rem(0.25)),
    width(`percent(1.0)),
    flexGrow(1),
    flexShrink(1),
    flexBasis(`auto),
    transition(~duration=150, ~timingFunction=`easeInOut, "box-shadow"),
    focus([MyBoxShadow.style]),
  ];

let component = ReasonReact.statelessComponent("MyInputText");

let make = (~id, ~name, ~value, ~onChange, ~placeholder, _children) => {
  ...component,
  render: _self =>
    <input
      _type="text"
      className=(Css.style(baseStyle))
      id
      name
      onChange
      value
      placeholder
    />,
};
