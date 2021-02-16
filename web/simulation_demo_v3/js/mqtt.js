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
  // Once a connection has been made, make a subscription
  console.log("onConnect");
  client.subscribe("projects/nordicoasys/topics/data");
  client.subscribe("projects/nordicoasys/topics/commands");
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
textToReceive = document.querySelector(".display");
function onMessageArrived(message) {
  if (textToReceive.value == "") {
    textToReceive.value += "> " + message.payloadString;
  } else {
    textToReceive.value += "\n> " + message.payloadString;
  }
  console.log("onMessageArrived:" + message.payloadString);

  addDocument(message.payloadString);
  textToReceive.scrollTop = textToReceive.scrollHeight;
}

// publish data
function mqttPublish(data) {
  message = new Paho.MQTT.Message(data);
  message.destinationName = "projects/nordicoasys/topics/data";
  message.destinationName = "projects/nordicoasys/topics/commands";
  client.send(message);
}
