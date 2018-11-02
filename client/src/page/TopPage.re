open TopPageModel;

module Styles = {
  open Css;
  let contents = style([padding(`rem(0.75))]);
};

let component = ReasonReact.statelessComponent("TopPage");

let make = (~send, ~state, _children) => {
  let onChange = e => {
    let name = ReactEvent.Form.target(e)##value;
    send @@ changeText(name);
  };
  let onSubmit = e => {
    ReactEvent.Form.preventDefault(e);
    send @@ createItem(state.text);
    send @@ changeText("");
  };
  {
    ...component,
    render: _self =>
      <div className=Styles.contents>
        <form onSubmit>
          <MyFormGroup>
            <MyInputText
              id="filterInput"
              name="filter"
              value=state.text
              placeholder="filter"
              onChange
            />
          </MyFormGroup>
        </form>
        <MyList>
          ...Belt.Map.(
               state.itemMap
               |. keepU((. _, item) => {
                    let name = item |. ItemModel.nameGet;
                    Js.String.includes(state.text, name);
                  })
               |. mapU((. item) => {
                    let (id, name, text) =
                      item |. ItemModel.(idGet, nameGet, textGet);
                    let href = Router.getURL @@ ItemPage(id);
                    <MyListItem href key=id> <Card name text /> </MyListItem>;
                  })
               |. valuesToArray
             )
        </MyList>
      </div>,
  };
};
