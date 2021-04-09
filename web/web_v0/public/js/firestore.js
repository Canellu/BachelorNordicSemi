// ------------  Helpers  ---------------

// If time or date is less than 10, add 0.  09, 08 etc...
function pad(n) {
  return n < 10 ? "0" + n : n;
}

// Simple random number generator with min max parameters
function randNum(min, max) {
  return Math.floor(Math.random() * max) + min;
}

// ------------  End Helpers  ---------------

// Creates dummy Gliders with GX as name X = number 1 -> something
function createGliders() {
  //Math.floor(Math.random() * 100) + 1; // returns a random integer from 1 to 100

  var today = new Date();
  var dd = String(today.getDate()).padStart(2, "0");
  var mm = String(today.getMonth() + 1).padStart(2, "0"); //January is 0!
  var yyyy = today.getFullYear();

  today = yyyy + "-" + mm + "-" + dd;

  for (let i = 1; i <= 3; i++) {
    let gliderID = randNum(100000, 899999).toString();
    db.collection("Gliders")
      .doc(gliderID)
      .set({
        Alias: `G${i}`,
        Added: today,
        "Last sync": today,
      });
  }
}

let numDocs = 8;
let day = 1;
// Create documents with timestamp consisting of JSON data for each glider
function createMission(gliderID, missionNum) {
  console.log("Creating doc for: ", gliderID);
  let year = 2021;

  let month = 3;
  let date = `${year}-${pad(month)}-${pad(day)}`;
  let hour = randNum(0, 23);
  let min = randNum(0, 59);
  let second = randNum(0, 59);
  let timestamp = `${pad(hour)}:${pad(min)}:${pad(second)}`;

  db.collection("Gliders")
    .doc(gliderID)
    .collection("Missions")
    .doc("Mission " + missionNum)
    .set({
      start: `${date} ${timestamp}`,
      end: `${year}-${pad(month)}-${pad(day + 1 + numDocs)} ${timestamp}`,
      freqP: randNum(1, 10),
      freqT: randNum(1, 10),
      freqC: randNum(1, 10),
      minD: randNum(1, 20),
      maxD: randNum(100, 200),
    });

  // Date Doc
  for (let i = 0; i < numDocs; i++) {
    let year = 2021;
    day++;
    date = `${year}-${pad(month)}-${pad(day)}`;
    // Timestamp with data
    for (let j = 0; j < 30; j++) {
      let hour = randNum(0, 23);
      let min = randNum(0, 59);
      let second = randNum(0, 59);
      let timestamp = `${pad(hour)}:${pad(min)}:${pad(second)}`;
      let temp = randNum(0, 20000) / 1000;
      let pressure = randNum(0, 4000) / 1000;
      let conductivity = randNum(4000, 6000) / 1000;
      let latMax = 59.014248;
      let longMax = 10.32792;
      let latMin = 58.838464;
      let longMin = 10.809945;
      let longitude =
        longMin + randNum(0, (longMax - longMin) * 1000000) / 1000000;
      let latitude = latMin + randNum(0, (latMax - latMin) * 1000000) / 1000000;
      let jsonDATA = "";

      if (randNum(1, 14) == 1) {
        jsonDATA += `"lng":${longitude}, "lat":${latitude},`;
      }
      if (randNum(1, 4) <= 3) {
        jsonDATA += `"T":${temp},`;
      }
      if (randNum(1, 4) <= 3) {
        jsonDATA += `"P":${pressure},`;
      }
      if (randNum(1, 4) <= 3) {
        jsonDATA += `"C":${conductivity},`;
      }

      if (jsonDATA !== "") {
        db.collection("Gliders")
          .doc(gliderID)
          .collection("Missions")
          .doc("Mission " + missionNum)
          .collection("Data")
          .doc(date)
          .set({ [timestamp]: jsonDATA }, { merge: true });
      }
    }
  }
}

async function createGliderData() {
  const gliders = await db.collection("Gliders").get();
  gliders.forEach((glider) => {
    createMission(glider.id, 1);
    createMission(glider.id, 2);
  });
}

// createGliders();
// createGliderData();
