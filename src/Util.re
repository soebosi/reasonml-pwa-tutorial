open Belt;

let s_ = ReasonReact.string;

let a_ = ReasonReact.array;

let filterMap = (f, stream) =>
  Most.(
    stream
    |> filter(x =>
         switch (f(x)) {
         | Some(_) => true
         | None => false
         }
       )
    |> map(x => Option.getExn(f(x)))
  );

