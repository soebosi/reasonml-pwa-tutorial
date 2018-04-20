open Belt;

open Util;

let baseStyle =
  Css.[
    fontSize(`rem(1.0)),
    lineHeight(1.5),
    padding2(`rem(0.375), `rem(0.75)),
    color(rgb(256, 256, 256)),
    border(px(1), `solid, rgb(180, 180, 180)),
    borderRadius(`rem(0.25)),
    backgroundColor(`rgb((0, 150, 256))),
    transition(~duration=150, ~timingFunction=`easeInOut, "background-color"),
    hover([backgroundColor(rgb(0, 100, 255))]),
    active([
      boxShadow(
        ~x=`px(0),
        ~y=`px(0),
        ~blur=`px(0),
        ~spread=`rem(0.2),
        rgba(0, 123, 255, 0.25),
      ),
    ]),
  ];

let component = ReasonReact.statelessComponent("MyButton");

let make = (~styles=?, children) => {
  ...component,
  render: self => {
    let styles = Option.getWithDefault(styles, []);
    let className = Css.(merge([styles, baseStyle]) |. style);
    ReasonReact.createDomElement(
      "button",
      ~props={"type": "submit", "className": className},
      children,
    );
  },
};
