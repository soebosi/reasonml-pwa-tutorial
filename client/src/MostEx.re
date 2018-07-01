module Most = {
  include Most;
  let keepMap = (f, stream) =>
    stream
    |> map(f)
    |> filter(Belt.Option.isSome)
    |> map(Belt.Option.getExn);
};
