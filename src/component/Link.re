open Util;

let component = ReasonReact.statelessComponent("Link");

let make = (~href, ~text, _children) => {
  ...component,
  render: self =>
    <a
      href="about:blank"
      onClick=(
        event => {
          ReactEventRe.Mouse.preventDefault(event);
          ReasonReact.Router.push(href);
        }
      )>
      (s_(text))
    </a>,
};
