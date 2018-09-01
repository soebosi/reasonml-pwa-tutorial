open Util;

let title =
  Css.[
    marginBottom(`rem(0.75)),
    overflow(`hidden),
    textOverflow(`ellipsis),
    width(`px(100)),
  ];

let body =
  Css.[
    overflow(`hidden),
    textOverflow(`ellipsis),
    width(`px(100)),
    height(`px(50)),
  ];

let component = ReasonReact.statelessComponent("Card");

let make = (~name, ~text, _children) => {
  ...component,
  render: _self =>
    <>
      <h5 className=(Css.style(title))> (s_(name)) </h5>
      <div className=(Css.style(body))> (s_(text)) </div>
    </>,
};
