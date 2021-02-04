// Create a client instance
//mqtt.googleapis.com:443 or port 8883 with TLS
//broker.mqttdashboard.com
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
  client.subscribe("projects/nordicoasys/topics/data");
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
  if (textToReceive.value == "") {
    textToReceive.value += "> " + message.payloadString;
  } else {
    textToReceive.value += "\n> " + message.payloadString;
  }
  console.log("onMessageArrived:" + message.payloadString);

  textToReceive.scrollTop = textToReceive.scrollHeight;
}

textToSend = document.querySelector(".sendBox");

// Enter key will sendData
textToSend.addEventListener("keyup", function (event) {
  if (event.keyCode === 13) {
    document.querySelector(".sendBtn").click();
  }
});

//Attached to send button
function sendData() {
  if (textToSend.value == "") return;
  message = new Paho.MQTT.Message(textToSend.value);
  message.destinationName = "projects/nordicoasys/topics/data";
  client.send(message);
  updateDoc(textToSend.value);
  textToSend.value = "";
}
//Attached to clear button
function clearScreen() {
  textToReceive.value = "";
}

var currThumbPos;
var releaseVal = 0;
var releaseValOld = 0;
//progress bar //
$(".neumorphic-slider__thumb").on("mousedown", function () {
  $(document).on("mousemove.mm", function (e) {
    var new_value = 0;
    if (e.clientX < $(".neumorphic-slider").offset().left) {
      new_value = "0%";
    } else if (
      e.clientX >
      $(".neumorphic-slider").offset().left +
        $(".neumorphic-slider").width() -
        10
    ) {
      new_value = "100%";
    } else {
      new_value =
        (
          ((e.clientX - $(".neumorphic-slider").offset().left) /
            ($(".neumorphic-slider").width() - 10)) *
          100
        ).toFixed(0) + "%";
      releaseVal = new_value;
      // currThumbPos = e.clientX - $(".neumorphic-slider").offset().left;
    }
    document.documentElement.style.setProperty("--value", new_value);
    $(".neumorphic-slider__popover").text(new_value);
  });

  $(document).on("mouseup.mu", function () {
    if (releaseVal != releaseValOld) {
      releaseValOld = releaseVal;
      updateDoc(`battery ${releaseVal}`);
    }

    $(document).off("mousemove.mm");
    $(".neumorphic-slider__thumb").off("mouseup.mu");
  });
});

function renderData(doc) {
  batteryText = document.querySelector(".battery").childNodes[1];
  durationText = document.querySelector(".duration").childNodes[1];
  statusText = document.querySelector(".status").childNodes[1];
  healthText = document.querySelector(".health").childNodes[1];
  temperatureText = document.querySelector(".temperature").childNodes[1];

  batteryText.textContent = doc.data().Battery;
  durationText.textContent = doc.data()["Dive duration"];
  statusText.textContent = doc.data()["Dive status"];
  healthText.textContent = doc.data().Health;
  temperatureText.textContent = doc.data().Temperature;

  indicHealth = document.querySelector(".indicHealth");

  if (doc.data().Health == "false") {
    indicHealth.classList.add("alert");
  } else {
    indicHealth.classList.remove("alert");
  }

  document.querySelector(
    ".neumorphic-slider__popover"
  ).textContent = doc.data().Battery;
  document.documentElement.style.setProperty("--value", doc.data().Battery);
}

db.collection("Sensor").onSnapshot((snapshot) => {
  let changes = snapshot.docChanges();
  changes.forEach((change) => {
    if (change.type === "added" || change.type === "modified") {
      renderData(change.doc);
    }
  });
});

function updateDoc(command) {
  var doc = db.collection("Sensor").doc("H4JYgp8VythfG7JX4GZ4");
  let value = command.split(" ");

  if (value[0] == "health") {
    console.log(`Health new value: ${value[1]}`);
    if (value[1] == "true") {
      doc.update({ Health: "true" });
    }
    if (value[1] == "false") {
      doc.update({ Health: "false" });
    }
  }

  if (value[0] == "battery") {
    doc.update({ Battery: value[1] });
  }
}
