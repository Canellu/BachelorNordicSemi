const functions = require("firebase-functions");
const admin = require("firebase-admin");
const cors = require("cors")({ origin: true });
const { PubSub } = require("@google-cloud/pubsub");
const iot = require("@google-cloud/iot");
//const runtimeConfig = require('cloud-functions-runtime-config');
const fetch = require("node-fetch");

admin.initializeApp(functions.config().firebase);

const db = admin.database();
const pubsub = new PubSub();

var client = new iot.v1.DeviceManagerClient({});

function handlePOST(data, context) {
  var obj = {
    Message: data.Message.toString(),
  };
  console.log(obj);
  let binaryData = Buffer.from(JSON.stringify(obj));
  var device = data.deviceId;
  var formattedName = client.devicePath(
    "nrf9160test",
    "europe-west1",
    "OasysFleet",
    device
  );
  request = {
    name: formattedName,
    binaryData: binaryData,
  };
  client
    .modifyCloudToDeviceConfig(request)
    .then((responses) => {})
    .catch((err) => {
      console.error(err);
    });
}

/**
 * Receive data from pubsub, then
 * Respond using device configuration.
 */
exports.mqttReceiveAndRespond = functions.pubsub
  .topic("data")
  .onPublish((message, context) => {
    const attributes = message.attributes;
    console.log(message);
    const payload = message.json;
    console.log(payload);

    const deviceId = attributes["nrf9160dk"];

    var data = {
      deviceId: deviceId,
      timestamp: context.timestamp,
      Message: payload.Message,
    };

    return Promise.all([handlePOST(data, context)]);
  });
