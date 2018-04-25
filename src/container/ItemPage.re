open Belt;

open Util;

[@bs.module] external reactMarkdown : ReasonReact.reactClass = "react-markdown";

let make = (~name, ~dispatch, ~itemPageState, children) => {
  ReasonReact.wrapJsForReason(
    ~reactClass=reactMarkdown,
    ~props={"source": name},
    children,
  );
};
