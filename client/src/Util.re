let s_ = ReasonReact.string;

let a_ = ReasonReact.array;

let compose = (f, g, x) => f(g(x));

let (<<) = compose;

let dict2string = Js.Json.(stringify << object_);
