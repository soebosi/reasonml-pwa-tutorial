open Belt;

open Util;

open ItemPageReducer;

let component = ReasonReact.reducerComponent("ItemPage");

let make = (~name, _children) => {
  ...component,
  initialState,
  reducer,
  render: self => <div> (s_(name)) </div>,
};
