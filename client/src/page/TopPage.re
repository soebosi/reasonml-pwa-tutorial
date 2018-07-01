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
    send @@ createItem(state.text);
    send @@ changeText("");
  };
  let onSubmitDelete = (id, e) => {
    ReactEventRe.Form.preventDefault(e);
    send @@ deleteItem(id);
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
               Belt.Map.mapU(
                 state.itemMap,
                 (. item) => {
                   let (id, name) = item |. ItemModel.(id, name);
                   let href = Router.getURL @@ ItemPage(id);
                   let onSubmit = onSubmitDelete(id);
                   <MyListItem key=id>
                     <Link href> (s_(name)) </Link>
                     <form onSubmit className=Styles.removeBtn>
                       <MyButton type_=Remove> (s_("remove")) </MyButton>
                     </form>
                   </MyListItem>;
                 },
               )
               |. Belt.Map.valuesToArray
             )
        </MyList>
      </div>,
  };
};
