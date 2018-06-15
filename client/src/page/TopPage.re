open Belt;

open Util;

open TopPageModel;

module Styles = {
  open Css;
  let contents = style([maxWidth(`percent(75.0))]);
  let removeBtn = style([textAlign(`right)]);
};

let component = ReasonReact.statelessComponent("TopPage");

let make = (~send, ~state, _children) => {
  let onChange = e => {
    let dom = ReactEventRe.Form.target(e);
    let name = ReactDOMRe.domElementToObj(dom)##value;
    send @@ changeText(name);
  };
  let onSubmit = e => {
    ReactEventRe.Form.preventDefault(e);
    send @@ addItem(state.text);
    send @@ changeText("");
  };
  let onSubmitRemove = (name, e) => {
    ReactEventRe.Form.preventDefault(e);
    send @@ removeItem(name);
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
              value=state.text
              placeholder="name"
              onChange
            />
          </MyFormGroup>
          <MyButton type_=Add> (s_("submit")) </MyButton>
        </form>
        <MyList>
          ...(
               Map.mapU(state.itemMap, (. item) =>
                 <MyListItem key=(item |. ItemModel.name)>
                   <Link href=("/items/" ++ (item |. ItemModel.id))>
                     (s_(item |. ItemModel.name))
                   </Link>
                   <form
                     onSubmit=(onSubmitRemove(item |. ItemModel.id))
                     className=Styles.removeBtn>
                     <MyButton type_=Remove> (s_("remove")) </MyButton>
                   </form>
                 </MyListItem>
               )
               |. Map.valuesToArray
             )
        </MyList>
      </div>,
  };
};
