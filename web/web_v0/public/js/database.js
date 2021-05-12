// db is declared in firebase.js

// Returns mission documents
function getMissionDocs() {
  return db.collection("Gliders").doc(gliderUID).collection("Missions").get();
}
