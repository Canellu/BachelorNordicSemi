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
    const messageBody = message.data
      ? Buffer.from(message.data, "base64").toString()
      : null;

    const writeMessage = await db.collection("fromNRF").add({
      message: messageBody,
    });

    console.log("WriteMessage: ", writeMessage);

    let json = null;
    try {
      json = message.json;
      console.log("JSON: ", json);
    } catch (e) {
      console.error("PubSub message was not JSON", e);
    }
  });
