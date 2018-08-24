let baseStyle =
  Css.[
    display(`grid),
    gridTemplateColumns([`fr(1.0), `fr(1.0), `fr(1.0)]),
  ];

let component = ReasonReact.statelessComponent("MyList");

let make = children => {
  ...component,
  render: _self => <ul className=(Css.style(baseStyle))> ...children </ul>,
};
