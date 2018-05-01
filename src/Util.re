open Belt;

let s_ = ReasonReact.string;

let a_ = ReasonReact.array;

let compose = (f, g) => x => f(g(x));

let (<<) = compose;
