use std::sync::atomic::{AtomicUsize, Ordering};

use rocket_contrib::{Json, Value};
use rocket::State;

#[derive(Serialize, Deserialize)]
pub struct Message {
    name: String,
}

pub struct SequenceNumber(pub AtomicUsize);

#[post("/api/v1/items", format = "application/json", data = "<message>")]
pub fn new_item(message: Json<Message>, sequence_number: State<SequenceNumber>) -> Json<Value> {
    sequence_number.0.fetch_add(1, Ordering::Relaxed);
    Json(json!({
      "id": sequence_number.0.load(Ordering::Relaxed).to_string(),
      "name": message.name,
    }))
}
