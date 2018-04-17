open Belt;

open Util;

open TopPageReducer;

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
            <MyInputText
              name="name"
              value=self.state.name
              placeholder="name"
              onChange=(self.handle(handleChange))
            />
          </label>
          <input _type="submit" />
        </form>
        <MyList
          items=(
            Set.String.toArray(self.state.nameSet)
            |. Array.mapU((. name) =>
                 <MyListItem key=name id=name text=name />
               )
          )
        />
      </div>,
  };
};
