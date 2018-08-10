use rocket_contrib::{Json, Value};

use super::super::db;

#[derive(Serialize, Deserialize)]
pub struct Message {
    name: String,
}

#[post("/items", format = "application/json", data = "<message>")]
pub fn create_item(message: Json<Message>, _conn: db::Conn) -> Json<Value> {
    Json(json!({
      "id":   "1",
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
