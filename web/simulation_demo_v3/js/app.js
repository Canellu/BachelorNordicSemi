// terminal input field
var sendBox = document.querySelector(".sendBox");

// Enter key will sendData
sendBox.addEventListener("keyup", function (event) {
  if (event.keyCode === 13) {
    document.querySelector(".sendBtn").click();
  }
});

//Attached to send button
function sendData() {
  if (sendBox.value == "") return;

  mqttPublish(sendBox.value);
  updateDoc(sendBox.value);

  if (sendBox.value == "cls") textToReceive.value = "";
  sendBox.value = "";
}
//Attached to clear button
function clearScreen() {
  textToReceive.value = "";
}

// Updating UI
function renderData(doc) {
  const keys = Object.keys(doc);

  keys.forEach((key, index) => {
    // console.log(`${key}: ${doc[key]}`);
    switch (key) {
      case "avgCurrent":
        document.querySelector(".avgCurrent").textContent = doc[key] + " A";
        break;
      case "battery":
        document.querySelector(".battery").textContent = doc[key] + " %";
        break;
      case "conductivity":
        document.querySelector(".conductivity").textContent = doc[key];
        break;
      case "maxCurrent":
        document.querySelector(".maxCurrent").textContent = doc[key] + " A";
        break;
      case "pressure":
        document.querySelector(".pressure").textContent = doc[key];
        break;
      case "temperature":
        document.querySelector(".temperature").textContent = doc[key];
        break;
      case "timestamp":
        break;
      case "health":
        indicHealth = document.querySelector(".indicHealth");
        if (doc[key] == "false") {
          indicHealth.classList.add("alert");
        } else {
          indicHealth.classList.remove("alert");
        }
    }
  });

  //Update indicator
}

//TODO finish conversion function
function convertToFirestoreFormat(nrfFormat) {
  var tmp = JSON.parse(nrfFormat);
  console.log(Object.keys(tmp));
}

// convertToFirestoreFormat('{"t":"17:10:52","P":"78.654","C":"20.452"}');
