open Belt;

module Most = {
  include Most;
  let filterMap = (f, stream) =>
    stream
    |> map(f)
    |> filter(Option.isSome)
    |> map(Option.getExn)
};
