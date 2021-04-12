const functions = require("firebase-functions");
const iot = require("@google-cloud/iot");

// The Firebase Admin SDK to access Firestore.
const admin = require("firebase-admin");

admin.initializeApp();

const db = admin.firestore();

const deviceId = "einarnrf9160dk";
const cloudRegion = "europe-west1";
const registryId = "OasysFleet";
const projectId = "nordicoasys";

const iotClient = new iot.v1.DeviceManagerClient({
  // optional auth parameters.
});

const formattedName = iotClient.devicePath(
  projectId,
  cloudRegion,
  registryId,
  deviceId
);

exports.fromFirestoreToNRF = functions
  .region("europe-west1")
  .firestore.document("Gliders/{gliderId}/Missions/{mission}")
  .onWrite(async (change, context) => {
    // TODO: Check only for changes in commands, do not send all
    // compare change.before with change.after
    const data = change.after.data();

    console.log(data);
    const stringData = JSON.stringify(data);
    const binaryData = Buffer.from(stringData);
    const request = {
      name: formattedName,
      binaryData: binaryData,
    };

    try {
      const responses = await iotClient.sendCommandToDevice(request);

      console.log("Sent command: ", responses[0]);
    } catch (err) {
      console.error("Could not send command:", err);
    }
  });

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
