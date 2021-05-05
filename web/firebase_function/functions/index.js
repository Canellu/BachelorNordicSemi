const functions = require("firebase-functions");
const iot = require("@google-cloud/iot");
const moment = require("moment-timezone");

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

// To Glider
exports.fromFirestoreToNRF = functions
  .region("europe-west1")
  .firestore.document("Gliders/{gliderId}/Missions/{mission}")
  .onWrite(async (change, context) => {
    // TODO: Check only for changes in commands, do not send all
    // compare change.before with change.after
    const data = change.after.data();

    let missionNum = Number(context.params.mission.split(" ")[1]);
    const { WP, ...rest } = data;

    let latArr = WP.map((latlng) => Number(latlng.split(",")[0]));
    let lngArr = WP.map((latlng) => Number(latlng.split(",")[1]));

    let toSendData = {
      M: missionNum,
      lat: latArr,
      lng: lngArr,
      ...rest,
    };

    // console.log("-------- DATA OBJCET TO SEND: -------------- ", toSendData);

    const stringData = JSON.stringify(toSendData);
    const binaryData = Buffer.from(stringData);
    const request = {
      name: formattedName,
      binaryData: binaryData,
    };

    // try {
    //   const commandResponse = await iotClient.sendCommandToDevice(request);

    //   console.log("Sent command: ", commandResponse[0]);
    // } catch (err) {
    //   console.error("Could not send command:", err);
    // }

    try {
      const configResponse = await iotClient.modifyCloudToDeviceConfig(request);

      console.log("Sent config:", configResponse[0]);
    } catch (err) {
      console.error("Could not send config:", err);
    }
  });

// From Glider
exports.fromNRFtoFirestore = functions
  .region("europe-west1")
  .pubsub.topic("data")
  .onPublish(async (message, context) => {
    // Decode the PubSub Message body.
    // const messageBody = message.data
    //   ? Buffer.from(message.data, "base64").toString()
    //   : null;
    console.log("GLIDER UID: " + message.attributes.deviceId);

    let utcDate = moment(context.timestamp);
    let localTime = utcDate.tz("Europe/Oslo").format("YYYY-MM-DD HH:mm:ss");
    console.log("utcDate: " + utcDate);
    console.log("localTime: " + localTime);

    let dataJSON = null;
    try {
      dataJSON = message.json;
      console.log("JSON: ", dataJSON);
      // await db.collection("fromNRF").add(dataJSON);
    } catch (e) {
      console.error("PubSub message was not JSON", e);
    }
  });

// ------------------------------------------------------------
// From Rockblock Iridium Satellite
// ------------------------------------------------------------
exports.satellite = functions
  .region("europe-west1")
  .https.onRequest(async (req, res) => {
    function hex_to_ascii(str1) {
      let hex = str1.toString();
      let str = "";
      for (var n = 0; n < hex.length; n += 2) {
        str += String.fromCharCode(parseInt(hex.substr(n, 2), 16));
      }
      return str;
    }

    let payload = JSON.parse(req.rawBody.toString());
    let gliders = await db.collection("Gliders").get();

    gliders.forEach(async (glider) => {
      let gliderFields = await db.collection("Gliders").doc(glider.id).get();

      if (gliderFields.data()["Sat IMEI"] == payload.imei) {
        let utcDate = moment(
          "20" + payload.transmit_time.replace(" ", "T") + "Z"
        );
        let localTime = utcDate.tz("Europe/Oslo").format("YYYY-MM-DD HH:mm:ss");

        db.collection("Gliders")
          .doc(glider.id)
          .set(
            {
              "Last seen": hex_to_ascii(payload.data),
              "Last sync": localTime,
            },
            { merge: true }
          );
      }
    });

    res.status(200).send();
  });
