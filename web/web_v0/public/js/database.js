// db is declared in firebase.js

const gliderUID = new URLSearchParams(window.location.search).get("gliderUID");

// Returns mission documents
function getMissionDocs() {
  return db.collection("Gliders").doc(gliderUID).collection("Missions").get();
}
