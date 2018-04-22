[@bs.deriving abstract]
type state = {
  page: ReasonReact.reactElement,
};

[@bs.deriving accessors]
type action =
  | ChangePage(ReasonReact.reactElement);

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
  ...component,
  initialState: () => {
    state(~page=<TopPage />);
  },
  reducer: (action, _state) => {
    switch(action) {
    | ChangePage(page) => ReasonReact.Update(state(~page))
    }
  },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch(url.path) {
          | _ => <TopPage />
          }
          |> changePage
          |> self.send
        ),
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: self => page(self.state),
};
