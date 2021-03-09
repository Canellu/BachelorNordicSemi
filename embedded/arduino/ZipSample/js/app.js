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

// Init materialize elements
document.addEventListener("DOMContentLoaded", function () {
  M.Tabs.init(document.querySelectorAll(".tabs"));
  M.Modal.init(document.querySelectorAll(".modal"));
});

form.addEventListener("submit", (e) => {
  e.preventDefault();
});
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

// M.AutoInit();

var toastHTML =
  '<span style="color: #c6ff00; font-weight: bold;">Do <u>NOT</u> close the browser while files are downloading!</span>';

downloadBtn.addEventListener("click", () => {
  createZip();
  Socket.send("{generate_all}");
  // downloadBtn.classList.add("disabled");
  progressBar.classList.remove("hide");

  M.toast({
    html: toastHTML,
    displayLength: 100000,
    classes: "rounded posTop",
  });
});

function addFileRow(fileName, fileSize) {
  var row = `<tr><td>${fileName}</td><td>${fileSize}</td></tr>`;
  fileList.innerHTML = row + fileList.innerHTML;
}

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

var zip = new JSZip();

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

var midlertidigTerminal = document.querySelector(".midlertidigTerminal");
var sizeCard = document.querySelector(".totalSize");
var fileCard = document.querySelector(".totalFiles");

var textToZip = "";

var filename = "";
var fileData = "";
var fileSize = 0;
var totalFileSize = 0;
var totalFiles = 0;
var rowsAdded = [];

var dataReceived = 0;

//init();
var Socket;
function init() {
  Socket = new WebSocket("ws://" + window.location.hostname + ":81/");
  Socket.onmessage = function (event) {
    // Test, append to Moar Card
    document.querySelector(".midlertidigTerminal").innerText += event.data;
    textToZip += event.data;

    if (event.data.includes("TXT")) {
      var split = event.data.split(":");
      filename = split[0].toLowerCase();
      fileSize = parseInt(split[1]);

      if (!rowsAdded.includes(filename)) {
        rowsAdded.push(filename);
        totalFileSize += fileSize;
        totalFiles++;
        sizeCard.innerText = totalFileSize;
        fileCard.innerText = totalFiles;
        addFileRow(filename.slice(0, filename.length - 4), fileSize);
      }
    } else if (event.data == "BASE") {
      zip.file(filename, fileData);
    } else {
      fileData += event.data;
      dataReceived += event.data.length;
      let percentage = parseInt((dataReceived / totalFileSize) * 100);
      progressFill.style.width = `${percentage}%`;
      progressLabel.innerText = `${percentage}%`;
    }
  };
}
