let baseStyle =
  Css.[
    border(px(1), `solid, rgba(0, 0, 0, 0.15)),
    backgroundColor(`rgb((256, 256, 256))),
    padding2(`rem(0.75), `rem(1.25)),
    marginBottom(px(-1)),
    borderRadius(`rem(0.25)),
    boxShadow(~y=px(2), rgba(0, 0, 0, 0.12)),
    hover([backgroundColor(rgba(0, 0, 0, 0.05))]),
  ];

let component = ReasonReact.statelessComponent("MyListItem");

let make = (~href, children) => {
  ...component,
  render: _self =>
    <a
      href
      className=(
        Css.style(
          Css.[
            textDecoration(`none),
            margin(`rem(0.50)),
            color(rgb(0x36, 0x3C, 0x49)),
          ],
        )
      )>
      <li className=(Css.style(baseStyle))> ...children </li>
    </a>,
};
