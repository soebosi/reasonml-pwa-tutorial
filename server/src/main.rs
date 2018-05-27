#![feature(plugin, decl_macro)]
#![plugin(rocket_codegen)]

extern crate rocket;

#[cfg(test)]
mod tests;

use std::io;
use std::path::{Path, PathBuf};

use rocket::response::NamedFile;

#[get("/")]
fn index() -> io::Result<NamedFile> {
    NamedFile::open("../public/index.html")
}

#[get("/public/<file..>", rank = 2)]
fn files(file: PathBuf) -> Option<NamedFile> {
    NamedFile::open(Path::new("../public/").join(file)).ok()
}

#[get("/<_file..>", rank = 3)]
fn fallback(_file: PathBuf) -> io::Result<NamedFile> {
    NamedFile::open("../public/index.html")
}

fn rocket() -> rocket::Rocket {
    rocket::ignite().mount("/", routes![index, files, fallback])
}

fn main() {
    rocket().launch();
}
