let baseStyle =
  Css.[
    Css.float(`left),
    width(`percent(45.0)),
    height(`px(500)),
    margin(`px(10)),
    borderRadius(`rem(0.25)),
    focus([MyBoxShadow.style]),
    transition(~duration=150, ~timingFunction=`easeInOut, "box-shadow"),
  ];

let component = ReasonReact.statelessComponent("MyTextarea");

let make = (~onChange, ~value, ~style=?, children) => {
  ...component,
  render: _self => {
    let customStyle = Belt.Option.getWithDefault(style, []);
    let className = Css.style @@ Css.merge([customStyle, baseStyle]);
    ReasonReact.createDomElement(
      "textarea",
      ~props={
        "type": "submit",
        "className": className,
        "onChange": onChange,
        "value": value,
      },
      children,
    );
  },
};
