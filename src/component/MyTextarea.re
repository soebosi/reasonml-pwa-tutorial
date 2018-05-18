open Belt;

let baseStyle =
  Css.[
    float(`left),
    width(`percent(40.0)),
    height(`px(500)),
    margin(`px(10)),
    borderRadius(`rem(0.25)),
  ];

let component = ReasonReact.statelessComponent("MyTextarea");

let make = (~onChange, ~value, ~style=?, children) => {
  ...component,
  render: _self => {
    let customStyle = Option.getWithDefault(style, []);
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
