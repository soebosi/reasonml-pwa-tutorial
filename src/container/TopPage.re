open Belt;

type state = {
  name: string,
  nameList: list(string)
};

type action =
  | AddName(string)
  | ChangeText(string);

let s_ = ReasonReact.stringToElement;
let a_ = ReasonReact.arrayToElement;

let component = ReasonReact.reducerComponent("TopPage");
let make = (_children) => {
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
  initialState: () => {
    {
      name: "",
      nameList: []
    };
  },
  reducer: (action, state) => {
    switch(action) {
    | ChangeText(name) => ReasonReact.Update({...state, name})
    | AddName(name) => {
      let nameList = state.nameList @ [name];
      ReasonReact.Update({...state, nameList})
    }
    }
  },
  render: self =>
    <div>
      <form onSubmit={self.handle(handleSubmit)}>
        <label>
          {s_("Name:")}
          <input _type="text" name="name" onChange={self.handle(handleChange)} value={self.state.name} />
        </label>
        <input _type="submit" />
      </form>
      <ul>
        {a_(
          List.toArray(self.state.nameList)
          |. SortArray.stableSortBy(Pervasives.compare)
          |. Array.mapU((. name) => <li key={name}>{s_(name)}</li>)
        )}
      </ul>
    </div>,
  };
};
