open Util;

let component = ReasonReact.statelessComponent("ErrorPage");

let make = _children => {
  ...component,
  render: _self => <div> (s_("NotFound")) </div>,
};
