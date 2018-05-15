open Belt;

open Util;

open TopPageModel;

module Styles = {
  open Css;
  let contents = style([maxWidth(`percent(75.0))]);
};

let component = ReasonReact.statelessComponent("TopPage");

let make = (~send, ~topPageState, _children) => {
  let onChange = e => {
    let dom = ReactEventRe.Form.target(e);
    let name = ReactDOMRe.domElementToObj(dom)##value;
    send @@ changeText(name);
  };
  let onSubmit = e => {
    ReactEventRe.Form.preventDefault(e);
    send @@ addItem(topPageState.text);
    send @@ changeText("");
  };
  {
    ...component,
    render: _self =>
      <div className=Styles.contents>
        <form onSubmit>
          <MyFormGroup>
            <MyLabel htmlFor="nameInput"> (s_("Name:")) </MyLabel>
            <MyInputText
              id="nameInput"
              name="name"
              value=topPageState.text
              placeholder="name"
              onChange
            />
          </MyFormGroup>
          <MyButton> (s_("submit")) </MyButton>
        </form>
        <MyList>
          ...(
               Set.String.toArray(topPageState.itemSet)
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
