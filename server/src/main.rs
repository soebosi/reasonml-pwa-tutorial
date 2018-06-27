#![feature(plugin, decl_macro)]
#![plugin(rocket_codegen)]

extern crate rocket;

#[macro_use]
extern crate rocket_contrib;
#[macro_use]
extern crate serde_derive;

use std::io;
use std::path::{Path, PathBuf};
use std::sync::atomic::AtomicUsize;

use rocket::response::NamedFile;

mod v1;

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
                v1::item::new_item,
                v1::item::delete_item,
                v1::item::retrieve_item,
            ],
        )
        .manage(v1::item::SequenceNumber { id: AtomicUsize::new(0) })
}

fn main() {
    rocket().launch();
}
