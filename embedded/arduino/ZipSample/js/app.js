// Get classes
var progressBar = document.querySelector(".prog-bar");
var progressFill = document.querySelector(".determinate");
var progressLabel = document.querySelector(".progressLabel");
var downloadBtn = document.querySelector(".download-btn");
var fileList = document.querySelector(".tbodyFiles");
var commandList = document.querySelector(".tbodyCommands");
var modalList = document.querySelector(".tbodyModal");
var form = document.querySelector("form");
var commandInput = document.querySelector("#commands");
var commandLabel = document.querySelector("label[for=commands]");
var checkAll = document.querySelector("#checkAll");

// Dummy data
var tmpName = 202003001;
function createDummyRows() {
  for (let i = 0; i < 6; i++) {
    addFileRow(`${tmpName + getRndInt(1, 29)}`, getRndInt(200, 800));
  }
}
createDummyRows();
adjustCheckboxesPosition();

var toastHTML =
  '<span style="color: #FBC02D; font-weight: bold;">Do <u>NOT</u> close or refresh the browser while files are downloading!</span>';

var zip = new JSZip();

// Listen for click on checkAll-box
var checkVal = true;
checkAll.addEventListener("click", () => {
  let checkBoxes = document.querySelectorAll(".filled-in:not(#checkAll)");
  checkBoxes.forEach((e) => {
    e.checked = checkVal;
  });
  checkVal = !checkVal;
});

function adjustCheckboxesPosition() {
  let checkBoxes = document.querySelectorAll(
    "#files label span:not(.checkspan)"
  );

  console.log(fileList.scrollHeight);
  console.log(fileList.offsetHeight);
  console.log(checkBoxes);

  // Has scrollbar if true
  if (fileList.scrollHeight > fileList.offsetHeight) {
    checkBoxes.forEach((e) => {
      console.log("added");
      e.classList.add("adjustForScrollbar");
      e.parentNode.parentNode.classList.add("adjustForMiniScrollbar");
    });
  }
}

// Listen for clicks, update total files checked
document.addEventListener("click", () => {
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
  // Initialize materialize stuff
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
    // Socket.send(commandInput.value);
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
  var row = `<tr><td colspan="2">${fileName}</td><td colspan="2">${fileSize}</td><td><label><input type="checkbox" class="filled-in" /><span></span></label></td></tr>`;
  fileList.innerHTML = row + fileList.innerHTML;
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

// Zips the files and creates a download on browser
function createZip() {
  zip.file("dataFromSD.txt", textToZip);
  zip
    .generateAsync({
      type: "blob",
      compression: "STORE",
    })
    .then((content) => {
      saveAs(content, "GliderData.zip");
    });
}

function getRndInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Download button
downloadBtn.addEventListener("click", () => {
  // Iterate over rows to see if checkbox is true/false
  requestList = [];
  Array.from(fileList.rows).forEach((row) => {
    let name = row.cells[0].textContent;
    let size = parseInt(row.cells[1].textContent);
    let checkBox = row.cells[2].querySelector("input[type=checkbox]").checked;
    console.log(`${name} ${size} ${checkBox}`);
    if (checkBox) {
      requestList.push([name, size]);
      currentTotalSize += size;
    }
  });
  downloadBtn.classList.add("disabled");
  progressBar.classList.remove("hide");

  currentFile = requestList.shift();
  name = currentFile[0];
  console.log(`CurrentTotalSize: ${currentTotalSize}`);
  console.log(`REQUESTING FILE: ${currentFile[0]}`);
  requestFileData(currentFile);

  M.toast({
    html: toastHTML,
    displayLength: 8000,
    classes: "rounded",
  });
});

function requestFileData(file) {
  fileData = "";
  sendingData = true;
  dataReceived = 0;
  currentSize = file[1];
  Socket.send(`{D:${file[0]}}`);
}

var midlertidigTerminal = document.querySelector(".midlertidigTerminal");
var sizeCard = document.querySelector(".totalSize");
var fileCard = document.querySelector(".totalFiles");
var checkedLabel = document.querySelector("#numChecked");

var textToZip = "";
var filename = "";
var fileData = "";
var fileSize = 0;
var totalDataReceived = 0;
var totalFileSize = 0;
var totalFiles = 0;
var rowsAdded = [];

var sendingData = true;
var currentTotalSize = 0;
var totalReceived = 0;
var requestList = [];
var dataReceived = 0;
var currentSize = 0;
var currentFile = [];

var name = "";

var Socket;

function populateFileTable() {
  if (event.data.includes("TXT")) {
    sendingData = false;
    var split = event.data.split(":");
    filename = split[0].toLowerCase();
    fileSize = parseInt(split[1]);
    console.log(`Filename ${filename}`);
    console.log(`FileSize ${fileSize}`);

    if (!rowsAdded.includes(filename)) {
      rowsAdded.push(filename);
      totalFileSize += fileSize;
      totalFiles++;
      sizeCard.innerText = totalFileSize;
      fileCard.innerText = totalFiles;
      checkedLabel.innerText = `0/${totalFiles}`;
      addFileRow(filename.slice(0, filename.length - 4), fileSize);
    }
  }
}

function sendFileRequest() {
  if (dataReceived == currentSize) {
    currentFile = requestList.shift();
    if (typeof currentFile !== "undefined") {
      console.log(`REQUESTING FILE: ${currentFile[0]}`);
      name = currentFile[0] + ".txt";
      requestFileData(currentFile);
    } else {
      downloadBtn.classList.remove("disabled");
      progressBar.classList.add("hide");
      currentSize = -1;
    }
  }
}

function updateProgressBar() {
  dataReceived += parseInt(event.data.length);
  totalReceived += parseInt(event.data.length);
  let percentage = (totalReceived / currentTotalSize) * 100;
  console.log(
    `Total received: ${totalReceived}, Percentage: ${percentage}, dataReceived: ${dataReceived}`
  );
  progressFill.style.width = `${percentage}%`;
  progressLabel.innerText = `${percentage}%`;
}

init();
function init() {
  Socket = new WebSocket("ws://" + window.location.hostname + ":81/");
  Socket.onmessage = function (event) {
    // Test, append to Moar Card
    document.querySelector(".midlertidigTerminal").innerText += event.data;
    textToZip += event.data + "\n";

    populateFileTable();

    if (event.data == "BASE") {
      console.log(`CREATING FILE WITH NAME: ${name}`);
      zip.file(name, fileData);
    } else {
      fileData += event.data;
      updateProgressBar();
    }

    sendFileRequest();

    // Check if zip is already created
    if (totalReceived == currentTotalSize && sendingData) {
      sendingData = false;
      console.log("CREATING ZIP");
      createZip();
    }
  };
}

function init() {
  Socket = new WebSocket("ws://" + window.location.hostname + ":81/");
  Socket.onmessage = function (event) {
    /*
       Higher-order-functions
       map
       filter
       reduce
       sort
       foreach
       some
       every
     */
    // Create objects? Store in array.
    /* var file = {
           name : "20200304",
           size : 204,
           data : null;

           methodName : function() { some action... };
       }
       */
    // var files = [file, file, file]
    // Access file properties with dot-notation. file.size, file.name
    // or bracket-notation. file['size']
    // Update card views table 0/0 and populate table.
    // Length of files = number files.
    // iterate files size to to update size-card
    // Request file data, update progressbar, store data in files?
    // iterate through table and look for checked == true.
    // requestFiles
  };
}
