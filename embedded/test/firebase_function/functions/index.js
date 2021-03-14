const functions = require("firebase-functions");

// The Firebase Admin SDK to access Firestore.
const admin = require("firebase-admin");
admin.initializeApp();

const db = admin.firestore();

exports.fromNRFtoFirestore = functions
  .region("europe-west1")
  .pubsub.topic("data")
  .onPublish(async (message) => {
    // Decode the PubSub Message body.
    // const messageBody = message.data
    //   ? Buffer.from(message.data, "base64").toString()
    //   : null;

    let dataJSON = null;
    try {
      dataJSON = message.json;
      console.log("JSON: ", dataJSON);
      await db.collection("fromNRF").add(dataJSON);
    } catch (e) {
      console.error("PubSub message was not JSON", e);
    }
  });
