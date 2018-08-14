use diesel;
use diesel::prelude::*;
use diesel::sqlite::SqliteConnection;

use rand::prelude::random;

use rocket_contrib::{Json, Value};

use super::super::db;

mod schema {
    table! {
        items {
            id   -> Text,
            name -> Text,
        }
    }
}

use self::schema::items;

#[table_name = "items"]
#[derive(Serialize, Queryable, Insertable, Debug, Clone)]
pub struct Item {
    pub id: String,
    pub name: String,
}

#[derive(Serialize, Deserialize)]
pub struct Message {
    name: String,
}

#[post("/items", format = "application/json", data = "<message>")]
pub fn create_item(message: Json<Message>, conn: db::Conn) -> Json<Value> {
    let id: u32 = random();
    let i = Item {
        id: id.to_string(),
        name: message.name.clone(),
    };
    let _ = diesel::insert_into(items::table).values(&i).execute(
        &conn as &SqliteConnection,
    );
    Json(json!({
      "id":   id.to_string(),
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
