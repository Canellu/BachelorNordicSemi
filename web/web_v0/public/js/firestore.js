// Log out method
const logout = document.querySelector("#logout");
logout.addEventListener("click", async (e) => {
  await auth.signOut();
  location.replace("index.html");
});

const glidersCollection = db.collection("Gliders");

// TODO
async function getDocs() {
  const dataCollectionG1 = glidersCollection.doc("G1").collection("Data");
  const docs = await dataCollectionG1.get();

  docs.forEach((doc) => {
    var fieldList = Object.keys(doc.data());
    console.log(fieldList);
    console.log(doc.data()[fieldList[0]]);
  });
}

// Creates dummy Gliders with GX as name X = number 1 -> something
function createGliders() {
  //Math.floor(Math.random() * 100) + 1; // returns a random integer from 1 to 100

  var today = new Date();
  var dd = String(today.getDate()).padStart(2, "0");
  var mm = String(today.getMonth() + 1).padStart(2, "0"); //January is 0!
  var yyyy = today.getFullYear();

  today = yyyy + "-" + mm + "-" + dd;

  for (let i = 1; i <= 6; i++) {
    let gliderID = "G" + i;

    glidersCollection.doc(gliderID).set({
      "Unique ID": randNum(0, 999999),
      added: today,
    });
  }
}

// If time or date is less than 10, add 0.  09, 08 etc...
function pad(n) {
  return n < 10 ? "0" + n : n;
}

// Simple random number generator with min max parameters
function randNum(min, max) {
  return Math.floor(Math.random() * max) + min;
}

// Create documents with timestamp consisting of JSON data for each glider
async function createGliderData() {
  const gliders = await glidersCollection.get();
  console.log(gliders.size);
  gliders.forEach((glider) => {
    let gliderID = glider.id;
    console.log("Creating doc for: ", glider.id);

    // Date Doc
    for (let i = 0; i < 5; i++) {
      let year = 2020;
      let day = randNum(1, 31);
      let month = randNum(1, 12);
      var date = `${year}-${pad(month)}-${pad(day)}`;
      // Timestamp with data
      for (let j = 0; j < 100; j++) {
        let hour = randNum(0, 23);
        let min = randNum(0, 59);
        let second = randNum(0, 59);
        let timestamp = `${pad(hour)}:${pad(min)}:${pad(second)}`;
        let temp = randNum(0, 20000) / 1000;
        let pressure = randNum(0, 4000) / 1000;
        let conductivity = randNum(4000, 6000) / 1000;
        let jsonDATA = `T:${temp}, P:${pressure}, C:${conductivity}`;
        glidersCollection
          .doc(gliderID)
          .collection("Data")
          .doc(date)
          .set({ [timestamp]: jsonDATA }, { merge: true });
      }
    }
  });
}

async function createAllCards() {
  const gliders = await glidersCollection.get();
  gliders.forEach((glider) => {
    getGliderData(glider.id);
  });
}

createAllCards();

async function getGliderData(gliderID) {
  const glider = glidersCollection.doc(gliderID);
  let data = await glider.get();
  let uid = data.data()["Unique ID"];
  let sync = data.data()["last sync"];
  createCard(uid, sync);
}
