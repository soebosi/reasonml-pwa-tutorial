open Belt;

module Most = {
  include Most;
  let keepMap = (f, stream) =>
    stream
    |> map(f)
    |> filter(Option.isSome)
    |> map(Option.getExn)
};
