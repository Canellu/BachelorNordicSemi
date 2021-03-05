// Get classes
var progressBar = document.querySelector(".prog-bar");
var downloadBtn = document.querySelector(".download-btn");
var fileList = document.querySelector(".tbodyFiles");

// Init materialize elements
document.addEventListener("DOMContentLoaded", function () {
  M.Tabs.init(document.querySelectorAll(".tabs"));
  M.Modal.init(document.querySelectorAll(".modal"));
  M.Carousel.init(document.querySelectorAll(".carousel"), {
    indicators: true,
    noWrap: true,
  });
});

// M.AutoInit();

downloadBtn.addEventListener("click", () => {
  createZip();
  downloadBtn.classList.add("disabled");
  progressBar.classList.remove("hide");
  progressFill = document.querySelector(".determinate");
});

function addFileRow(fileName, fileSize) {
  var row = ` <tr>
                   <td>${fileName}</td>
                   <td>${fileSize}</td>
                 </tr>`;
  fileList.innerHTML = row + fileList.innerHTML;
}

addFileRow("TOO BIG", ">9000");
addFileRow("TOO23r BIG", ">9000");
addFileRow("23rG", ">23r23r00");

function createZip() {
  // Instantiate ZIP object
  var zip = new JSZip();
  zip.file("TEST.txt", "Hello World\n");
  zip.file("TEST1.txt", "YO BOI\n");
  zip.file("TEST2.txt", "Another one\n");
  zip.file("TEST3.txt", "Catz\n");
  zip
    .generateAsync({
      type: "blob",
      compression: "STORE",
    })
    .then((content) => {
      // see FileSaver.js
      saveAs(content, "GliderData.zip");
    });
}

init();
var Socket;
function init() {
  Socket = new WebSocket("ws://" + window.location.hostname + ":81/");
  Socket.onmessage = function (event) {
    console.log(event.data);
    document.querySelector(".textAreaSD").value += event.data;
  };
}

var terminalBtn = document.querySelector(".terminalBtn");
var textInput = document.querySelector(".textInput");
terminalBtn.addEventListener("click", () => {
  Socket.send(textInput.value);
  textInput.value = "";
});
