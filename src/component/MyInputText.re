open Util;

let baseStyle =
  Css.[
    display(`block),
    fontSize(`rem(1.0)),
    lineHeight(1.5),
    padding2(`rem(0.375), `rem(0.75)),
    border(px(1), `solid, rgb(180, 180, 180)),
    borderRadius(`rem(0.25)),
    width(`percent(100.0)),
    transition(~duration=150, ~timingFunction=`easeInOut, "box-shadow"),
    focus([
      boxShadow(
        ~x=`px(0),
        ~y=`px(0),
        ~blur=`px(0),
        ~spread=`rem(0.2),
        rgba(0, 123, 255, 0.25),
      ),
    ]),
  ];

let component = ReasonReact.statelessComponent("MyInputText");

let make = (~id, ~name, ~value, ~onChange, ~placeholder, _children) => {
  ...component,
  render: self =>
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
