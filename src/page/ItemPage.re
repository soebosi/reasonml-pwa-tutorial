open Util;

open ItemPageModel;

let component = ReasonReact.statelessComponent("ItemPage");

let make = (~name, ~send, ~itemPageState, _children) => {
  let onChange = e => {
    let dom = ReactEventRe.Form.target(e);
    let text = ReactDOMRe.domElementToObj(dom)##value;
    send @@ changeText(text);
  };
  {
    ...component,
    render: _self =>
      <div>
        <h2> (s_(name)) </h2>
        <textarea value=itemPageState.text onChange />
        <Markdown source=itemPageState.source />
      </div>,
  };
};
