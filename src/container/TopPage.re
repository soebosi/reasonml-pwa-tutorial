type state = {
  text: string
};

type action =
  | ChangeText(string);

let s_ = ReasonReact.stringToElement;

let component = ReasonReact.reducerComponent("TopPage");
let make = (_children) => {
  let handleChange = (e, self) => {
    let dom = ReactEventRe.Form.target(e);
    let name = ReactDOMRe.domElementToObj(dom)##value;
    self.ReasonReact.send(ChangeText(name));
  };
  let handleSubmit = (e, self) => {
    ReactEventRe.Form.preventDefault(e);
  };
  {
  ...component,
  initialState: () => {
    {text: ""};
  },
  reducer: (action, state) => {
    switch(action) {
    | ChangeText(text) => ReasonReact.Update({...state, text})
    }
  },
  render: self =>
    <div>
      <form onSubmit={self.handle(handleSubmit)}>
        <label>
          {s_("Name:")}
          <input _type="text" name="name" onChange={self.handle(handleChange)} />
        </label>
        <input _type="submit" />
      </form>
      {s_(self.state.text)}
    </div>,
  };
};
