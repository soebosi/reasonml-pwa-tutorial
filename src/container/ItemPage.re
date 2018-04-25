open Belt;

open Util;

let component = ReasonReact.statelessComponent("ItemPage");

let make = (~name, ~dispatch, ~itemPageState, _children) => {
  ...component,
  render: self =>
    <div>
      (s_(name))
    </div>,
};
