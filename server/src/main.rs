#![feature(plugin, decl_macro)]
#![plugin(rocket_codegen)]

extern crate rocket;

#[macro_use]
extern crate rocket_contrib;
#[macro_use]
extern crate serde_derive;

use std::io;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};

use rocket_contrib::{Json, Value};
use rocket::response::NamedFile;
use rocket::State;

struct SequenceNumber(AtomicUsize);

#[derive(Serialize, Deserialize)]
struct Message {
    name: String,
}

#[get("/")]
fn index() -> io::Result<NamedFile> {
    NamedFile::open("../static/index.html")
}

#[post("/api/v1/items", format = "application/json", data = "<message>")]
fn new_item(message: Json<Message>, sequence_number: State<SequenceNumber>) -> Json<Value> {
    sequence_number.0.fetch_add(1, Ordering::Relaxed);
    Json(json!({
      "id": sequence_number.0.load(Ordering::Relaxed).to_string(),
      "name": message.name,
    }))
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
        .mount("/", routes![index, files, fallback, new_item])
        .manage(SequenceNumber(AtomicUsize::new(0)))

}

fn main() {
    rocket().launch();
}
