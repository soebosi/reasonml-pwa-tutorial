open Belt;

open Util;

open TopPageReducer;

module Styles = {
  open Css;
  let ulist = style([
    listStyleType(none),
    maxWidth(px(400)),
  ]);
  let list = style([
    border(px(1), `solid, rgba(0, 0, 0, 0.3)),
  ]);
};

let component = ReasonReact.reducerComponent("TopPage");

let make = _children => {
  let handleChange = (e, self) => {
    let dom = ReactEventRe.Form.target(e);
    let name = ReactDOMRe.domElementToObj(dom)##value;
    self.ReasonReact.send(ChangeText(name));
  };
  let handleSubmit = (e, self) => {
    ReactEventRe.Form.preventDefault(e);
    self.ReasonReact.send(AddName(self.state.name));
    self.ReasonReact.send(ChangeText(""));
  };
  {
    ...component,
    initialState,
    reducer,
    render: self =>
      <div>
        <form onSubmit=(self.handle(handleSubmit))>
          <label>
            (s_("Name:"))
            <input
              _type="text"
              name="name"
              onChange=(self.handle(handleChange))
              value=self.state.name
            />
          </label>
          <input _type="submit" />
        </form>
        <ul className=Styles.ulist>
          (
            a_(
              Set.String.toArray(self.state.nameSet)
              |. Array.mapU((. name) =>
                <li key=name className=Styles.list>
                  (s_(name))
                </li>
              ),
            )
          )
        </ul>
      </div>,
  };
};
