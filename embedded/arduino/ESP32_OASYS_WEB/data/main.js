// ----------------------------------------------------------------------------
// Global Variables
// ----------------------------------------------------------------------------

// Websocket
let gateway = `ws://${window.location.hostname}/ws`;
let websocket;

// DOM Elements
var progressBar = document.querySelector(".prog-bar");
var progressFill = document.querySelector(".determinate");
var progressLabel = document.querySelector(".progressLabel");
var downloadBtn = document.querySelector(".download-btn");
var fileListBody = document.querySelector(".tbodyFiles");
var commandList = document.querySelector(".tbodyCommands");
var modalList = document.querySelector(".tbodyModal");
var form = document.querySelector("form");
var commandInput = document.querySelector("#commands");
var commandLabel = document.querySelector("label[for=commands]");
var checkAll = document.querySelector("#checkAll");
var sizeCard = document.querySelector(".totalSize");
var fileCard = document.querySelector(".totalFiles");
var checkedLabel = document.querySelector("#numChecked");
var toastHTML =
  '<span style="color: #ffc400; font-weight: bold;">Do <u>NOT</u> close or refresh the browser while files are downloading!</span>';

// Funciton variables
var zip = new JSZip();
var files = []; // Contains file objects
var requestList = [];
var requestFile;
var requestDataReceived = 0;
var requestTotalSize = 0;
var espString = "";

var checkValAll = true;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------
window.addEventListener("load", onLoad);
function onLoad(event) {
  initWebSocket();
}

// ----------------------------------------------------------------------------
// WebSocket handling
// ----------------------------------------------------------------------------
function initWebSocket() {
  console.log("Trying to open a WebSocket connection...");
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log("Connection opened");
}

function onClose(event) {
  console.log("Connection closed");
}

function onMessage(event) {
  console.log(`Received a notification from ${event.origin}`);

  espString = event.data;
  console.log(espString);
  // Populate files and update UI
  if (espString.includes("TXT")) {
    initFileTab(espString);
    adjustCheckboxesPosition();
  } else if (espString == "EOF") {
    requestFile.complete = true;
    sendFileRequest();
  } else {
    requestFile.data += espString;
    updateProgressBar();
  }
  espString = "";
}

// Adjusts checkbox position based on scrollbar presence
function adjustCheckboxesPosition() {
  let checkBoxes = document.querySelectorAll(
    "#files label span:not(span.checkSpan)"
  );

  // Has scrollbar if true
  if (fileListBody.scrollHeight > fileListBody.offsetHeight) {
    checkBoxes.forEach((e) => {
      e.classList.add("adjustForScrollbar");
      e.parentNode.parentNode.classList.add("adjustForMiniScrollbar");
    });
  }
}

// Listen for click on checkAll-box
checkAll.addEventListener("click", () => {
  let checkBoxes = document.querySelectorAll(".filled-in:not(#checkAll)");
  checkBoxes.forEach((checkbox) => {
    checkbox.checked = checkValAll;
  });
  checkValAll = !checkValAll;
});

// Listen for clicks, update total files checked
fileListBody.addEventListener("click", () => {
  let checkBoxes = document.querySelectorAll(
    "#files .filled-in:not(#checkAll)"
  );

  let checkedFiles = 0;
  checkBoxes.forEach((e) => {
    if (e.checked == true) checkedFiles++;
  });

  document.querySelector(
    "#numChecked"
  ).innerText = `${checkedFiles}/${checkBoxes.length}`;
});

// Init materialize elements
document.addEventListener("DOMContentLoaded", function () {
  M.Tabs.init(document.querySelectorAll(".tabs"));
  M.Modal.init(document.querySelectorAll(".modal"));
});

// Prevent default submit action when Enter
form.addEventListener("submit", (e) => {
  e.preventDefault();
});

// Terminal inputField ON ENTER
commandInput.addEventListener("keypress", (e) => {
  if (e.keyCode == 13) {
    let split = commandInput.value.split(" ");
    // websocket.send(commandInput.value);
    addCommandRow(split[0], split[1]);
    // Validation
    for (var key in validCommands) {
      if (split[0] == key && split.length == 2) {
        commandInput.classList.remove("invalid");
        commandInput.classList.add("valid");
        break;
      } else {
        commandInput.classList.remove("valid");
        commandInput.classList.add("invalid");
      }
    }
    commandInput.value = "";
  }
});

// Add row to file table
function addFileRow(fileName, fileSize) {
  var row = `<tr><td colspan="2">${fileName}</td><td colspan="2">${(
    fileSize / 1000
  ).toFixed(
    2
  )}</td><td><label><input type="checkbox" class="filled-in" /><span></span></label></td></tr>`;
  fileListBody.innerHTML = row + fileListBody.innerHTML;
}

// Add row to command table
function addCommandRow(key, value) {
  var date = new Date();
  var time =
    date.getHours() +
    ":" +
    (date.getMinutes() < 10 ? "0" : "") +
    date.getMinutes() +
    ":" +
    (date.getSeconds() < 10 ? "0" : "") +
    date.getSeconds();
  var row = `<tr><td>${time}</td><td>${key.toUpperCase()}</td><td>${value}</td></tr>`;
  commandList.innerHTML = row + commandList.innerHTML;
}

// Modal data
var validCommands = {
  T: "0 - 12",
  P: "0 - 15",
  C: "0 - 20",
  D: "0 - 20",
  E: "0 - 20",
  F: "0 - 20",
  G: "0 - 20",
  H: "0 - 20",
};
var descriptions = [
  "Temperature",
  "Pressure",
  "Conductivity",
  "S",
  "S",
  "S",
  "S",
  "S",
];

// Populate modal table with data
function createModalContent(commandObj) {
  let count = 0;
  for (var key in commandObj) {
    var row = `<tr><td>${key}</td><td>${commandObj[key]}</td><td>${
      descriptions[count++]
    }</td></tr>`;
    modalList.innerHTML += row;
  }
}
createModalContent(validCommands);

downloadBtn.addEventListener("click", () => {
  // Resetting variables for next download
  requestDataReceived = 0;
  requestTotalSize = 0;
  files.forEach((file) => {
    file.checked = false;
  });

  // Iterate over rows to see if checkbox is true/false

  requestList = [];
  Array.from(fileListBody.rows).forEach((row) => {
    let name = row.cells[0].textContent;
    // let size = parseInt(row.cells[1].textContent);
    let checkBox = row.cells[2].querySelector("input[type=checkbox]").checked;

    if (checkBox) {
      // CHECK IF WORK
      let file = files.find((file) => {
        return file.name == name;
      });

      file.checked = true;

      if (!file.complete) requestList.push(file);

      requestTotalSize += file.size;
    }
  });

  // CHECK IF WORK
  let missingData = files.some((file) => {
    return file.checked && !file.complete;
  });

  console.log(missingData);
  // let isCheckedComplete = true;
  // files.forEach((file) => {
  //   if (file.checked && !file.complete) {
  //     isCheckedComplete = false;
  //     return false;
  //   }
  // });

  // Update UI
  downloadBtn.classList.add("disabled");
  progressBar.classList.remove("hide");

  if (!missingData) createZip();

  // Request filedata from NRF
  requestFile = requestList.shift();
  if (typeof requestFile !== "undefined") {
    websocket.send(`D:${requestFile.name}\r`);
  }

  M.toast({
    html: toastHTML,
    displayLength: 8000,
    classes: "rounded",
  });
});

function createZip() {
  files.forEach((file) => {
    zip.remove(file.name);

    if (file.checked) {
      zip.file(file.name, file.data);
    }
  });

  zip
    .generateAsync({
      type: "blob",
      compression: "STORE",
    })
    .then((content) => {
      saveAs(content, "GliderData.zip");
    });

  downloadBtn.classList.remove("disabled");
  progressBar.classList.add("hide");
}

function initFileTab(data) {
  let split = data.split(":");

  // Create file object
  let file = {
    name: split[0].toLowerCase(), // 20200304.txt
    size: parseInt(split[1]),
    data: "",
    complete: false,
    checked: false,
  };

  // Add to files array && table-row
  files.push(file);
  addFileRow(file.name, file.size);

  //Update UI
  fileCard.innerText = files.length;

  // CHECK IF WORK
  sizeCard.innerText =
    (
      files.reduce((currentTotalSize, file) => {
        return file.size + currentTotalSize;
      }, 0) / 1000000
    ).toFixed(2) + " MB";
  // let totalSize = 0;
  // files.forEach((file) => {
  //   totalSize += file.size;
  // });
  checkedLabel.innerText = `0/${files.length}`;
}

function sendFileRequest() {
  requestFile = requestList.shift();

  if (typeof requestFile !== "undefined") {
    websocket.send(`D:${requestFile.name}\r`);
  } else {
    createZip();
  }
}

function updateProgressBar() {
  requestDataReceived += parseInt(event.data.length);
  let percentage = Math.floor((requestDataReceived / requestTotalSize) * 100);

  progressFill.style.width = `${percentage}%`;
  progressLabel.innerText = `${percentage}%`;
}

function endConnection() {
  websocket.send("wifi_end\r");
}

// DUMMY DATA
// for (let i = 0; i < 20; i++) {
//   addFileRow(20202020 + i, i + 100);
// }
// adjustCheckboxesPosition();


if (!!window.EventSource) {
  var source = new EventSource('/events');
  console.log("Added event source");

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(event) {
    console.log("nrf_msg", event.data);

    espString = event.data;
    // Populate files and update UI
    if (espString.includes("TXT")) {
      initFileTab(espString);
      adjustCheckboxesPosition();
    } else if (espString == "EOF") {
      requestFile.complete = true;
      sendFileRequest();
    } else {
      requestFile.data += espString;
      updateProgressBar();
    }
    espString = "";
  }, false);
}