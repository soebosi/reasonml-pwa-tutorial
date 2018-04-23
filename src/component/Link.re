open Util;

let component = ReasonReact.statelessComponent("Link");

let make = (~href, children) => {
  ...component,
  render: self =>
    ReasonReact.createDomElement(
      "a",
      ~props={
        "href": href,
        "onClick": event => {
          ReactEventRe.Mouse.preventDefault(event);
          ReasonReact.Router.push(href);
        },
      },
      children,
    ),
};
