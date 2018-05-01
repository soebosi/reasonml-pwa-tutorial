open Util;

open TopPageModel;

let observe = stream => Most.(stream |> filter(_a => false));
