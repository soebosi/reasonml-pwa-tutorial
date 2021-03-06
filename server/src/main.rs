#![feature(plugin, decl_macro)]
#![plugin(rocket_codegen)]

extern crate rand;
extern crate rocket;

#[macro_use]
extern crate diesel;
#[macro_use]
extern crate rocket_contrib;
#[macro_use]
extern crate serde_derive;


use std::io;
use std::path::{Path, PathBuf};

use rocket::response::NamedFile;

mod v1;
mod db;

#[get("/")]
fn index() -> io::Result<NamedFile> {
    NamedFile::open("../static/index.html")
}

#[get("/static/<file..>", rank = 2)]
fn files(file: PathBuf) -> Option<NamedFile> {
    NamedFile::open(Path::new("../static/").join(file)).ok()
}

#[get("/<_file..>", rank = 3)]
fn fallback(_file: PathBuf) -> io::Result<NamedFile> {
    NamedFile::open("../static/index.html")
}

fn rocket() -> rocket::Rocket {
    rocket::ignite()
        .mount("/", routes![index, files, fallback])
        .mount(
            "/api/v1/",
            routes![
                v1::item::create,
                v1::item::retrieve_all,
                v1::item::retrieve,
                v1::item::update,
                v1::item::delete,
            ],
        )
        .manage(db::init_pool())
}

fn main() {
    rocket().launch();
}
