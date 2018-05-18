open Util;

open ItemPageModel;

module Styles = {
  open Css;
  let textarea =
    style([
      float(`left),
      width(`percent(40.0)),
      height(`px(500)),
      margin(`px(10)),
    ]);
};

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
        <MyTextarea onChange value=itemPageState.text />
        <div className=Styles.textarea>
          <Markdown source=itemPageState.source />
        </div>
      </div>,
  };
};
