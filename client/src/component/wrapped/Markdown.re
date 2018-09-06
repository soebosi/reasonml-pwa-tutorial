open Util;

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

let make = (~source, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=reactMarkdown,
    ~props={
      "source": source,
      "renderers": renderers(~code=args =>
                switch (languageFromJs @@ languageGet(args)) {
                | Some(`Mermaid) =>
                  <Mermaid value=valueGet(args) />
                | _ => <div> (s_("no support")) </div>
                }
            ),
    },
    children,
  );
