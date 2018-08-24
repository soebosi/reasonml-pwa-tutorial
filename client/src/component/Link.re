let component = ReasonReact.statelessComponent("Link");

let make = (~href, children) => {
  ...component,
  render: _self =>
    <a
      href
      onClick=(
        event => {
          ReactEvent.Mouse.preventDefault(event);
          ReasonReact.Router.push(href);
        }
      )>
      ...children
    </a>,
};
