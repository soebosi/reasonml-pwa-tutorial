open Util;

open ItemPageModel;

let component = ReasonReact.statelessComponent("ItemPage");

let make = (~name, ~dispatch, ~itemPageState, _children) => {
  let handleChange = e => {
    let dom = ReactEventRe.Form.target(e);
    let text = ReactDOMRe.domElementToObj(dom)##value;
    dispatch(changeText(text));
  };
  {
    ...component,
    render: _self =>
      <div>
        <h2> (s_(name)) </h2>
        <textarea value=itemPageState.text onChange=handleChange />
        <Markdown source=itemPageState.text />
      </div>,
  };
};
