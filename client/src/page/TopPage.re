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
  let onSubmitDelete = e => {
    ReactEventRe.Form.preventDefault(e);
    let dom = ReactEventRe.Form.target(e);
    let id = ReactDOMRe.domElementToObj(dom)##dataset##id;
    send @@ deleteItem(id);
  };
  {
    ...component,
    render: _self =>
      <div className=Styles.contents>
        <form onSubmit>
          <MyFormGroup>
            <MyInputText
              id="nameInput"
              name="name"
              value=state.text
              placeholder="name"
              onChange
            />
          </MyFormGroup>
        </form>
        <MyList>
          ...(
               Belt.Map.mapU(
                 state.itemMap,
                 (. item) => {
                   let (id, name) = item |. ItemModel.(idGet, nameGet);
                   let href = Router.getURL @@ ItemPage(id);
                   <MyListItem key=id>
                     <Link href> (s_(name)) </Link>
                     (
                       ReasonReact.cloneElement(
                         <form
                           onSubmit=onSubmitDelete
                           className=Styles.removeBtn
                         />,
                         ~props={"data-id": id},
                         [|
                           <MyButton type_=Remove> (s_("remove")) </MyButton>,
                         |],
                       )
                     )
                   </MyListItem>;
                 },
               )
               |. Belt.Map.valuesToArray
             )
        </MyList>
      </div>,
  };
};
