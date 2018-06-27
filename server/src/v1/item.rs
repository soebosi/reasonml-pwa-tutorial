use std::sync::atomic::{AtomicUsize, Ordering};

use rocket_contrib::{Json, Value};
use rocket::State;

#[derive(Serialize, Deserialize)]
pub struct Message {
    name: String,
}

pub struct SequenceNumber {
    pub id: AtomicUsize,
}

#[post("/items", format = "application/json", data = "<message>")]
pub fn new_item(message: Json<Message>, sequence_number: State<SequenceNumber>) -> Json<Value> {
    sequence_number.id.fetch_add(1, Ordering::Relaxed);
    Json(json!({
      "id": sequence_number.id.load(Ordering::Relaxed).to_string(),
      "name": message.name,
    }))
}

#[get("/items/<id>")]
pub fn retrieve_item(id: String) -> Json<Value> {
    Json(json!({
      "id":   id,
      "name": "",
    }))
}

#[delete("/items/<id>")]
pub fn delete_item(id: String) -> Json<Value> {
    Json(json!({
      "id": id,
    }))
}
