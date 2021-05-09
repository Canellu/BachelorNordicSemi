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

    // Timestamp on when glider last communicated through 4G
    await db.collection("Gliders").doc(gliderId).set(
      {
        "Last sync": localTime,
      },
      { merge: true }
    );

    let dataJSON = null;
    try {
      dataJSON = message.json;
      console.log("JSON: ", dataJSON);
    } catch (e) {
      console.error("PubSub message was not JSON", e);
    }

    // If glider sent data properly
    //add it to corresponding mission in database
    if (dataJSON != null) {
      let data;
      //TODO: check if data contains latlng, if yes
      // change glider last seen in database.
      if (data.includes("lat")) {
        let lat = dataJSON.data.lat;
        let lng = dataJSON.data.lng;

        await db
          .collection("Gliders")
          .doc(gliderId)
          .set(
            {
              "Last seen": `lat: ${lat}, lng:${lng}`,
            },
            { merge: true }
          );

        data = JSON.stringify({ lat, lng }).slice(1, -1);
      } else {
        data = JSON.stringify(dataJSON.data).slice(1, -1);
      }

      let missionNum = dataJSON.M;
      let logDate = dataJSON.ts.slice(0, 8);
      let logTime = dataJSON.ts.slice(8);

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
        .doc("Mission " + missionNum)
        .collection("Data")
        .doc(logDate)
        .set({ [logTime]: data }, { merge: true });

      console.log({ data });
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

        db.collection("Gliders")
          .doc(glider.id)
          .set(
            {
              "Last seen": `lat: ${lat}, lng: ${lng}`,
              "Last sync": localTime,
              "Sat payload": data !== undefined ? data : "",
            },
            { merge: true }
          );
      }
    });

    res.status(200).send();
  });
