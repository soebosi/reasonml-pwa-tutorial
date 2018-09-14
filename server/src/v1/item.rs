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
            text -> Text,
        }
    }
}

use self::schema::items;
use self::schema::items::dsl::{items as all_items, text as item_text};

#[table_name = "items"]
#[derive(Serialize, Queryable, Insertable, Debug, Clone)]
pub struct Item {
    pub id: String,
    pub name: String,
    pub text: String,
}

#[derive(Serialize, Deserialize)]
pub struct Message {
    name: String,
    text: String,
}

#[post("/items", format = "application/json", data = "<message>")]
pub fn create(message: Json<Message>, conn: db::Conn) -> Result<Json<Item>, Json<Value>> {
    let id: u32 = random();
    let item = Item {
        id: id.to_string(),
        name: message.name.clone(),
        text: message.text.clone(),
    };
    let result = diesel::insert_into(items::table).values(&item).execute(
        &conn as &SqliteConnection,
    );
    match result {
        Ok(_) => Ok(Json(item)),
        Err(e) => Err(Json(json!({ "error": e.to_string() }))),
    }
}

#[get("/items")]
pub fn retrieve_all(conn: db::Conn) -> Result<Json<Vec<Item>>, Json<Value>> {
    let items = all_items.load::<Item>(&conn as &SqliteConnection);
    match items {
        Ok(i) => Ok(Json(i)),
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

#[put("/items/<id>", format = "application/json", data = "<message>")]
pub fn update(id: String, message: Json<Message>, conn: db::Conn) -> Json<Value> {
    let item = diesel::update(all_items.find(id.clone()));
    let item = item.set(item_text.eq(message.text.clone())).execute(
        &conn as &SqliteConnection,
    );
    let v = match item {
        Ok(_) => json!({}),
        Err(e) => json!({ "error": e.to_string() }),
    };
    Json(v)
}

#[delete("/items/<id>")]
pub fn delete(id: String, conn: db::Conn) -> Json<Value> {
    let item = diesel::delete(all_items.find(id.clone())).execute(&conn as &SqliteConnection);
    let v = match item {
        Ok(_) => json!({}),
        Err(e) => json!({ "error": e.to_string() }),
    };
    Json(v)
}
