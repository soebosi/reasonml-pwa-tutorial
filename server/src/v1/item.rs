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
use self::schema::items::dsl::items as all_items;

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
pub fn create(message: Json<Message>, conn: db::Conn) -> Result<Json<Item>, Json<Value>> {
    let id: u32 = random();
    let item = Item {
        id: id.to_string(),
        name: message.name.clone(),
    };
    let result = diesel::insert_into(items::table).values(&item).execute(
        &conn as &SqliteConnection,
    );
    match result {
        Ok(_) => Ok(Json(item)),
        Err(e) => Err(Json(json!({ "error": e.to_string() }))),
    }
}

#[get("/items/<id>")]
pub fn retrieve(id: String, conn: db::Conn) -> Result<Json<Item>, Json<Value>> {
    let item = all_items.find(id.clone()).get_result::<Item>(
        &conn as &SqliteConnection,
    );
    match item {
        Ok(i) => Ok(Json(i)),
        Err(e) => Err(Json(json!({ "error": e.to_string() }))),
    }
}

#[delete("/items/<id>")]
pub fn delete(id: String, conn: db::Conn) -> Json<Value> {
    let item = diesel::delete(all_items.find(id.clone())).execute(&conn as &SqliteConnection);
    match item {
        Ok(_) => Json(json!({})),
        Err(e) => Json(json!({ "error": e.to_string() })),
    }
}
