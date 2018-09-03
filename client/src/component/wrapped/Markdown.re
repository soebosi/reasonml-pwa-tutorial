[@bs.module]
external reactMarkdown : ReasonReact.reactClass = "react-markdown";

[@bs.deriving jsConverter]
type language = [
  | [@bs.as "mermaid"] `Mermaid
];

[@bs.deriving abstract]
type codeArguments = {
  language: string,
  value: string,
};

[@bs.deriving abstract]
type renderers = {
  code: codeArguments => ReasonReact.reactElement,
};

let make = (~source, ~renderers, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=reactMarkdown,
    ~props={
      "source": source,
      "renderers": renderers,
    },
    children,
  );
