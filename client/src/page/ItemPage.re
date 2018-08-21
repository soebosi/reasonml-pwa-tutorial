open Util;

open ItemPageModel;

module Styles = {
  open Css;
  let contents = style([padding(`rem(0.75))]);
  let textarea =
    style([
      Css.float(`left),
      width(`percent(45.0)),
      height(`px(500)),
      margin(`px(10)),
    ]);
};

let component = ReasonReact.statelessComponent("ItemPage");

let make = (~send, ~state, _children) => {
  let onChange = e => {
    let dom = ReactEventRe.Form.target(e);
    let text = ReactDOMRe.domElementToObj(dom)##value;
    send @@ changeText(text);
  };
  {
    ...component,
    render: _self =>
      <div className=Styles.contents>
        <h2> (s_ @@ state.name) </h2>
        <MyTextarea onChange value=state.text />
        <div className=Styles.textarea> <Markdown source=state.source /> </div>
      </div>,
  };
};
