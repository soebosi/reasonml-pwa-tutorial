open Util;

let component = ReasonReact.statelessComponent("Mermaid");

let make = (~value, _children) => {
  ...component,
  render: _self => <> <div> (s_(value)) </div> </>,
};
