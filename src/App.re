type state = {
  page: ReasonReact.reactElement,
};

type action =
  | ChangedPage(ReasonReact.reactElement);

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState: () => {
    {page: <div>{ReasonReact.stringToElement("hello")}</div>};
  },
  reducer: (action, state) => {
    switch(action) {
    | ChangedPage(page) => ReasonReact.Update({...state, page})
    }
  },
  render: self => self.state.page,
};
