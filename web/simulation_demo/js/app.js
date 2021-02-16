//
var textToSend = document.querySelector(".sendBox");

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
  message.destinationName = "projects/nordicoasys/topics/commands";
  client.send(message);
  updateDoc(textToSend.value);
  if (textToSend.value == "cls") textToReceive.value = "";
  textToSend.value = "";
}
//Attached to clear button
function clearScreen() {
  textToReceive.value = "";
}


// Updating UI
function renderData(doc) {
  data = doc.data()
  //Update cards
  document.querySelector(".battery").childNodes[1].textContent = data.Battery;
  document.querySelector(".duration").childNodes[1].textContent = data["Dive duration"];
  document.querySelector(".status").childNodes[1].textContent = data["Dive status"];
  document.querySelector(".health").childNodes[1].textContent = data.Health;
  document.querySelector(".temperature").childNodes[1].textContent = data.Temperature;

  //Update indicator
  indicHealth = document.querySelector(".indicHealth");
  if (data.Health == "false") {
    indicHealth.classList.add("alert");
  } else {
    indicHealth.classList.remove("alert");
  }

  //Update slider
  document.querySelector(".neumorphic-slider__popover").textContent = data.Battery;

  //Update css slider value
  document.documentElement.style.setProperty("--value", data.Battery);
}





// Updates firestore Sensor collection document
function updateDoc(command) {
  var doc = db.collection("Sensor").doc("H4JYgp8VythfG7JX4GZ4");
  var value = command.split(" ");

  if(value.length == 2) {
    switch (value[0]) {
      case "battery":
        doc.update({ Battery: value[1] })
        break;
      case "duration":
        doc.update({ ["Dive duration"]: value[1] })
        break;
      case "status":
        doc.update({ ["Dive status"]: value[1] })
        break;
      case "health":
        doc.update({ Health: value[1] })
        break;
      case "temp":
        doc.update({ Temperature: value[1] })
        break;
    }
  }

}

// Listen to firestore database changes
db.collection("Sensor").onSnapshot((snapshot) => {
  var changes = snapshot.docChanges();
  changes.forEach((change) => {
    if (change.type === "added" || change.type === "modified") {
      renderData(change.doc);
    }
  });
});