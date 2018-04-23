open Belt;

open Util;

open TopPageReducer;

module Styles = {
  open Css;
  let contents = style([maxWidth(`percent(75.0))]);
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
      <div className=Styles.contents>
        <form onSubmit=(self.handle(handleSubmit))>
          <MyFormGroup>
            <MyLabel htmlFor="nameInput"> (s_("Name:")) </MyLabel>
            <MyInputText
              id="nameInput"
              name="name"
              value=self.state.name
              placeholder="name"
              onChange=(self.handle(handleChange))
            />
          </MyFormGroup>
          <MyButton> (s_("submit")) </MyButton>
        </form>
        <MyList>
          ...(
               Set.String.toArray(self.state.nameSet)
               |. Array.mapU((. name) =>
                    <MyListItem key=name>
                      <Link href=("./" ++ name)> (s_(name)) </Link>
                    </MyListItem>
                  )
             )
        </MyList>
      </div>,
  };
};
