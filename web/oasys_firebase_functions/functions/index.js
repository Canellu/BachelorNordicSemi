const functions = require("firebase-functions");
const iot = require("@google-cloud/iot");
const moment = require("moment-timezone");

// The Firebase Admin SDK to access Firestore.
const admin = require("firebase-admin");
admin.initializeApp();
const db = admin.firestore();

const iotClient = new iot.v1.DeviceManagerClient({
  // optional auth parameters.
});

// To Glider
exports.fromDatabaseToGlider = functions
  .region("europe-west2")
  .firestore.document("Gliders/{gliderId}/Missions/{mission}")
  .onWrite(async (change, context) => {
    // TODO: Check only for changes in commands, do not send all
    // compare change.before with change.after
    const data = change.after.data();
    // Helper function
    function logLevelToInt(logLevel) {
      switch (logLevel) {
        case "High":
          return 3;
        case "Medium":
          return 2;
        case "Low":
          return 1;
        default:
          return 0;
      }
    }

    let missionNum = Number(context.params.mission.split(" ")[1]);
    let gliderId = "G" + context.params.gliderId;

    const { WP, C, P, T, ...rest } = data;

    let latArr = WP.map((latlng) => latlng.split(",")[0]);
    let lngArr = WP.map((latlng) => latlng.split(",")[1]);

    let toSendData = {
      M: missionNum,
      C: logLevelToInt(C),
      P: logLevelToInt(P),
      T: logLevelToInt(T),
      lat: latArr,
      lng: lngArr,
      ...rest,
    };

    console.log("-------- DATA OBJECT TO SEND: -------------- ", toSendData);

    const formattedName = iotClient.devicePath(
      "oasys-2d5b2",
      "europe-west1",
      "oasys_gliders",
      gliderId
    );

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

    console.log(context);

    try {
      const configResponse = await iotClient.modifyCloudToDeviceConfig(request);

      console.log("Sent config:", configResponse[0]);
    } catch (err) {
      console.error("Could not send config:", err);
    }
  });

// From Glider
exports.fromGliderToDatabase = functions
  .region("europe-west2")
  .pubsub.topic("data")
  .onPublish(async (message, context) => {
    // Decode the PubSub Message body.
    // const messageBody = message.data
    //   ? Buffer.from(message.data, "base64").toString()
    //   : null;

    // console.log("GLIDER UID: " + message.attributes.deviceId);

    let gliderId = message.attributes.deviceId.substring(1);
    let utcDate = moment(context.timestamp);
    let localTime = utcDate.tz("Europe/Oslo").format("YYYY-MM-DD HH:mm:ss");

    console.log("utcDate: " + utcDate);
    console.log("localTime: " + localTime);

    await db.collection("Gliders").doc(gliderId).set(
      {
        "Last sync": localTime,
      },
      { merge: true }
    );

    let messageJSON = null;
    try {
      messageJSON = message.json;
      console.log("JSON: ", messageJSON);
    } catch (e) {
      console.error("PubSub message was not JSON", e);
    }

    if (messageJSON != null) {
      const { start, data } = messageJSON;

      // Add mission to Firestore
      if (start !== undefined) {
        await db
          .collection("Gliders")
          .doc(gliderId)
          .collection("Missions")
          .doc("Mission " + messageJSON.M)
          .set(messageJSON, { merge: true });
      }

      // Add data to Firestore
      if (data !== undefined) {
        // Update 'Last seen' if data includes latlng
        if ("lat" in data) {
          await db
            .collection("Gliders")
            .doc(gliderId)
            .set(
              {
                "Last seen": `lat: ${data.lat}, lng:${data.lng}`,
              },
              { merge: true }
            );
        }

        let gliderData = JSON.stringify({ data }).slice(1, -1);
        let logDate = messageJSON.ts.slice(0, 8);
        let logTime = messageJSON.ts.slice(8);

        logDate =
          logDate.slice(0, 4) +
          "-" +
          logDate.slice(4, 6) +
          "-" +
          logDate.slice(6);

        logTime =
          logTime.slice(0, 2) +
          ":" +
          logTime.slice(2, 4) +
          ":" +
          logTime.slice(4);

        await db
          .collection("Gliders")
          .doc(gliderId)
          .collection("Missions")
          .doc("Mission " + messageJSON.M)
          .collection("Data")
          .doc(logDate)
          .set({ [logTime]: gliderData }, { merge: true });

        console.log({ logDate, logTime, gliderData });
      }
    }
  });

// ------------------------------------------------------------
// From Rockblock Iridium Satellite
// ------------------------------------------------------------
exports.fromGliderSatellite = functions
  .region("europe-west2")
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

        let split = hex_to_ascii(payload.data).split(",");
        let lat = split[0];
        let lng = split[1];
        let data = split[2];

        // Updating glider fields in Firestore
        db.collection("Gliders")
          .doc(glider.id)
          .set(
            {
              "Last seen": `lat: ${lat}, lng: ${lng}`,
              "Last sync": localTime,
              Status: data !== undefined ? data : "",
            },
            { merge: true }
          );

        console.log(payload);
        // Updating satellite msg log in Firestore
        db.collection("Gliders")
          .doc(glider.id)
          .collection("Satellite")
          .doc(localTime)
          .set(
            { Direction: "MO", Payload: hex_to_ascii(payload.data) },
            { merge: true }
          );
      }
    });

    res.status(200).send();
  });
