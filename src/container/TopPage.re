open Belt;

open Util;

open TopPageReducer;

module Styles = {
  open Css;
  let contents = style([maxWidth(`percent(75.0))]);
};

let component = ReasonReact.statelessComponent("TopPage");

let make = (~dispatch, ~topPageState, _children) => {
  let handleChange = e => {
    let dom = ReactEventRe.Form.target(e);
    let name = ReactDOMRe.domElementToObj(dom)##value;
    dispatch(ChangeText(name));
  };
  let handleSubmit = e => {
    ReactEventRe.Form.preventDefault(e);
    dispatch(AddName(topPageState.name));
    dispatch(ChangeText(""));
  };
  {
    ...component,
    render: self =>
      <div className=Styles.contents>
        <form onSubmit=handleSubmit>
          <MyFormGroup>
            <MyLabel htmlFor="nameInput"> (s_("Name:")) </MyLabel>
            <MyInputText
              id="nameInput"
              name="name"
              value=topPageState.name
              placeholder="name"
              onChange=handleChange
            />
          </MyFormGroup>
          <MyButton> (s_("submit")) </MyButton>
        </form>
        <MyList>
          ...(
               Set.String.toArray(topPageState.nameSet)
               |. Array.mapU((. name) =>
                    <MyListItem key=name>
                      <Link href=("/items/" ++ name)> (s_(name)) </Link>
                    </MyListItem>
                  )
             )
        </MyList>
      </div>,
  };
};
