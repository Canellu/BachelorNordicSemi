// Web app configureations from Firebase
const firebaseConfig = {
  apiKey: "AIzaSyBgmnAlnqUYSKux9s1596U6UpmSLP9ncsU",
  authDomain: "nordicoasys.firebaseapp.com",
  projectId: "nordicoasys",
  storageBucket: "nordicoasys.appspot.com",
  messagingSenderId: "40698568655",
  appId: "1:40698568655:web:0019d0a8fe097028c4fcba",
  measurementId: "G-WG6M7TCHK6",
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

const db = firebase.firestore();
const collectionG1 = db.collection("G1");

function addDocument(dataString) {
  var dataJSON = JSON.parse(dataString);
  collectionG1.add(dataJSON);
}

// Async function get() returns promise
async function getDocuments() {
  const snapshot = await collectionG1.get();

  if (snapshot.empty) {
    console.log("No documents found...");
    return;
  } else {
    // List with data of documents
    var docsList = snapshot.docs.map((doc) => doc.data());
    // List with documents
    // var docsList = snapshot.docs.map((doc) => doc);
    return docsList;
  }
}

// // EXAMPLE OF USE
// var docs = getDocuments();
// docs.then((docs) => console.log(docs[0].id));

// Listen to changes in collection G1
collectionG1.onSnapshot((snapshot) => {
  var changes = snapshot.docChanges();
  changes.forEach((change) => {
    if (change.type === "added" || change.type === "modified") {
      renderData(change.doc.data());
    }
  });
});

// Update the one document in Sensor collection
function updateDoc(command) {
  var doc = collectionG1.doc("vxynwjenv5cUCTiVIPyC");
  var value = command.split(" ");

  if (value.length == 2) {
    switch (value[0]) {
      case "battery":
        doc.update({ battery: value[1] });
        break;
      case "health":
        doc.update({ health: value[1] });
        break;
      case "temperature":
        doc.update({ temperature: value[1] });
        break;
      case "pressure":
        doc.update({ pressure: value[1] });
        break;
      case "conductivity":
        doc.update({ conductivity: value[1] });
        break;
    }
  }
}
