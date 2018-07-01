let baseStyle = Css.[display(`inlineBlock), marginBottom(`rem(0.5))];

let component = ReasonReact.statelessComponent("MyLabel");

let make = (~htmlFor, ~style=?, children) => {
  ...component,
  render: _self => {
    let customStyle = Belt.Option.getWithDefault(style, []);
    let className = Css.style @@ Css.merge([customStyle, baseStyle]);
    ReasonReact.createDomElement(
      "label",
      ~props={"type": "submit", "className": className, "htmlFor": htmlFor},
      children,
    );
  },
};
