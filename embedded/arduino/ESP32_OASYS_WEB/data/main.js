// ----------------------------------------------------------------------------
// Global Variables
// ----------------------------------------------------------------------------

// Websocket
let gateway = `ws://${window.location.hostname}/ws`;
let websocket;

// DOM Elements
let progressBar = document.querySelector(".prog-bar");
let progressFill = document.querySelector(".determinate");
let progressLabel = document.querySelector(".progressLabel");
let downloadBtn = document.querySelector(".download-btn");
let fileListBody = document.querySelector("#tbodyFiles");
let checkAll = document.querySelector("#checkAll");
let sizeCard = document.querySelector(".totalSize");
let fileCard = document.querySelector(".totalFiles");
let checkedLabel = document.querySelector("#numChecked");
let toastHTML =
  '<span style="color: #ffc400; font-weight: bold;">Do <u>NOT</u> close or refresh the browser while files are downloading!</span>';

// Funciton variables
let zip = new JSZip();
let files = []; // Contains file objects
let requestList = [];
let requestFile;
let requestDataReceived = 0;
let requestTotalSize = 0;
let espString = "";
let missionNumFromNrf = 0;
let satIMEI = 0;
let nrfIMEI = 0;
let moduleTestStr = ""; // value assigned by modules.js

let checkedFiles = 0;
let checkValAll = true;

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
  websocket.send("time:" + Date.now());
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

// ----------------------------------------------------------------------------
// Helper functions
// ----------------------------------------------------------------------------

// Scrolls window to top
function scrollToTop() {
  document.body.scrollTop = 0;
  document.documentElement.scrollTop = 0;
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
  if (checkedFiles) {
    downloadBtn.classList.remove("disabled");
  } else {
    downloadBtn.classList.add("disabled");
  }
});

// Listen for clicks, update total files checked
fileListBody.addEventListener("click", () => {
  let checkBoxes = document.querySelectorAll(
    "#files .filled-in:not(#checkAll)"
  );

  checkBoxes.forEach((e) => {
    if (e.checked == true) checkedFiles++;
  });

  document.querySelector(
    "#numChecked"
  ).innerText = `${checkedFiles}/${checkBoxes.length}`;

  if (checkedFiles) {
    downloadBtn.classList.remove("disabled");
  } else {
    downloadBtn.classList.add("disabled");
  }
});

// Init materialize elements
document.addEventListener("DOMContentLoaded", function () {
  M.Tabs.init(document.querySelectorAll(".tabs"));
  M.Modal.init(document.querySelectorAll(".modal"));
  M.FormSelect.init(document.querySelectorAll("select"));
  // M.Pushpin.init(document.querySelectorAll(".pushpin"), {});
  M.Datepicker.init(document.querySelectorAll(".datepicker"), {
    // autoClose: true,
    format: "yyyy-mm-dd",
    firstDay: 1,
    // showClearBtn: true,
  });

  M.Timepicker.init(document.querySelectorAll(".timepicker"), {
    twelveHour: false,
  });
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
    console.log(name);
    // let size = parseInt(row.cells[1].textContent);
    let checkBox = row.cells[2].querySelector("input[type=checkbox]").checked;

    if (checkBox) {
      // CHECK IF WORK
      let file = files.find((file) => {
        return file.name == name;
      });

      if (file !== undefined) {
        file.checked = true;
        if (!file.complete) requestList.push(file);
        requestTotalSize += file.size;
      }
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
    websocket.send(`D:${requestFile.name}`);
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

function updateMissionNum() {
  document.querySelector("#missionNumFromNrf").innerHTML = missionNumFromNrf;
  document.querySelector("#inputMissionNum").value = missionNumFromNrf + 1;
}

function modulesTestResponse(testResponseStr) {
  enableClickEvents(testResponseStr);
}

// DUMMY DATA
// for (let i = 0; i < 20; i++) {
//   addFileRow(20202020 + i, i + 100);
// }
// adjustCheckboxesPosition();

if (!!window.EventSource) {
  var source = new EventSource("/events");
  console.log("Got event source");

  source.addEventListener(
    "open",
    function (e) {
      console.log("Events Connected");
    },
    false
  );

  source.addEventListener(
    "error",
    function (e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    },
    false
  );

  let lastEventId;
  source.addEventListener(
    "message",
    function (event) {
      // console.log(event.data, event.lastEventId);

      if (event.lastEventId == lastEventId) return;

      // Updates mission number if NRF sent it.
      if (event.data.includes("nIMEI")) {
        let parts = event.data.split(",");
        nrfIMEI = parts.find((part) => part.includes("nIMEI:")).split(":")[1];
        satIMEI = parts.find((part) => part.includes("sIMEI:")).split(":")[1];
        missionNumFromNrf = parts
          .find((part) => part.includes("M:"))
          .split(":")[1];
        updateMissionNum();
      }

      if (event.data.includes(moduleTestStr)) {
        modulesTestResponse(event.data);
      }

      lastEventId = event.lastEventId;
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
    },
    false
  );
}
