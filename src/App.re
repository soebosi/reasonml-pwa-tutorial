type state = {
  page: ReasonReact.reactElement,
};

type action =
  | ChangePage(ReasonReact.reactElement);

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState: () => {
    {page: <TopPage />};
  },
  reducer: (action, _state) => {
    switch(action) {
    | ChangePage(page) => ReasonReact.Update({page: page})
    }
  },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch(url.path) {
          | _ => self.send(ChangePage(<TopPage />))
          }
        ),
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: self => self.state.page,
};
