type state = {
  page: ReasonReact.reactElement,
};

type action =
  | ChangePage(ReasonReact.reactElement);

let s_ = ReasonReact.stringToElement;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState: () => {
    {page: <TopPage />};
  },
  reducer: (action, state) => {
    switch(action) {
    | ChangePage(page) => ReasonReact.Update({...state, page})
    }
  },
  subscriptions: self => [
  Sub(
    () =>
      ReasonReact.Router.watchUrl(url =>
        switch(url.path) {
        | [] => self.send(ChangePage(<TopPage />))
        }
      ),
    ReasonReact.Router.unwatchUrl
  )
],
  render: self => self.state.page,
};
