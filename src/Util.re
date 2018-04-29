open Belt;

let s_ = ReasonReact.string;

let a_ = ReasonReact.array;

let filterMap = (f, stream) =>
  Most.(
    stream
    |> map(f)
    |> filter(Option.isSome)
    |> map(Option.getExn)
  );

