open Util;

let component = ReasonReact.statelessComponent("Card");

let make = (~name, ~text, _children) => {
  ...component,
  render: _self =>
    <>
      <h5
        className=(
          Css.style(
            Css.[
              marginBottom(`rem(0.75)),
              overflow(`hidden),
              textOverflow(`ellipsis),
              width(`px(100)),
            ],
          )
        )>
        (s_(name))
      </h5>
      <div
        className=(
          Css.style(
            Css.[
              overflow(`hidden),
              textOverflow(`ellipsis),
              width(`px(100)),
              height(`px(50)),
            ],
          )
        )>
        (s_(text))
      </div>
    </>,
};
