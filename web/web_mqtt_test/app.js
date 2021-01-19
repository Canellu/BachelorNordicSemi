// Create a client instance
client = new Paho.MQTT.Client(
  "broker.mqttdashboard.com",
  Number(8000),
  "canellosWebsiteNRF9160"
);

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({ onSuccess: onConnect });

// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("einarNordicnrf9160");
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
textToReceive = document.querySelector(".receiveBox");
function onMessageArrived(message) {
  console.log("onMessageArrived:" + message.payloadString);
  textToReceive.value = message.payloadString;
}

textToSend = document.querySelector(".sendBox");
function sendData() {
  message = new Paho.MQTT.Message(textToSend.value);
  message.destinationName = "einarNordicnrf9160_2";
  client.send(message);
  textToSend.value = "";
}
