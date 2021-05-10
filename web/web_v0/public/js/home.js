// Check if user is logged in and redirect them
auth.onAuthStateChanged((user) => {
  if (user) {
    console.log("Is logged in!");
  } else {
    // User is logged out
    console.log("Is logged out!");
    location.replace("index.html");
  }
});

// ****************** CARD-GRID SECTION ******************
async function createCard(uid, alias, sync, img) {
  sync = sync.replace(/\D/g, "");
  let year = Number(sync.slice(0, 4));
  let month = Number(sync.slice(4, 6)) - 1;
  let day = Number(sync.slice(6, 8));
  let hour = Number(sync.slice(8, 10));
  let minute = Number(sync.slice(10, 12));
  let second = Number(sync.slice(12));
  let fromNow = moment([year, month, day, hour, minute, second]).fromNow();

  let batteryState = "";
  switch (Math.floor(Math.random() * 4)) {
    case 0:
      batteryState = "quarter";
      break;
    case 1:
      batteryState = "half";
      break;
    case 2:
      batteryState = "three-quarters";
      break;
    case 3:
      batteryState = "full";
      break;
  }

  let sdCardState =
    Math.floor(Math.random() * 2) == 0 ? "sd_card" : "sd_card_alert";
  let healthState =
    Math.floor(Math.random() * 2) == 0 ? "check_circle" : "error";
  let fixState = Math.floor(Math.random() * 2) == 0 ? "hidden" : "";

  let html = ` 
    <a href="device.html?gliderUID=${uid}">
      <div class="card">
        <img
          class="object-cover center w-full h-44"
          src=${img}
          alt=""
        />
        <div class="flex-grow place-self-center mt-4">
          <h4 class="text-xl font-bold tracking-widest">ID: ${uid}</h4>
        </div>

        <div class="flex justify-evenly w-full my-2">
        <i
        class="fas fa-battery-${batteryState} flex items-center text-xl transform -rotate-90 scale-y-125"
      ></i>
        <span class="material-icons text-3xl"> ${sdCardState} </span>
        <span class="material-icons text-3xl"> ${healthState} </span>
        <span class="material-icons text-3xl"> build_circle </span>
        </div>
        <div class="w-full left-2 bg-gray-800 text-white p-2">
          <!-- <i class="far fa-clock fa-lg mr-2"></i> -->
          <p class="tracking-wide text-center text-sm font-medium">Last sync: ${fromNow}</p>
        </div>

        <div
        class="badge absolute px-3 py-1 flex justify-center items-center bg-gray-800 top-4 left-4 rounded-lg font-semibold tracking-widest text-gray-200">${alias}</div>
      </div>
    </a>`;

  document.querySelector("#deviceGrid").innerHTML += html;
}

async function createAllCards() {
  const gliders = await db.collection("Gliders").get();
  gliders.forEach((glider) => {
    getGliderFields(glider.id);
    populateHomeMap(glider);
  });
}

function populateHomeMap(glider) {
  let location;
  let latlng = glider.data()["Last seen"];
  console.log(typeof latlng);
  console.log(latlng);
  if (typeof latlng != "undefined" && latlng != "") {
    let datetime = glider.data()["Last sync"];
    let alias = glider.data()["Alias"];
    let lat = latlng.split(",")[0].replace(/^\D+/g, "");
    let lng = latlng.split(",")[1].replace(/^\D+/g, "");
    location = { t: datetime, lat: Number(lat), lng: Number(lng) };
    addHomeMarkers(location, alias);
  }
}

const images = [
  "assets/img/jellyfish.jpg",
  "assets/img/turtle.jpg",
  "assets/img/shark.jpg",
  "assets/img/water1.jpg",
  "assets/img/water2.jpg",
  "assets/img/fishies.jpg",
];

async function getGliderFields(gliderID) {
  const glider = db.collection("Gliders").doc(gliderID);
  let data = await glider.get();
  let uid = gliderID;
  let alias = data.data()["Alias"];
  let sync = data.data()["Last sync"];
  createCard(uid, alias, sync, images[randNum(0, 5)]);
}

// ************** FILE UPLOAD SECTION *********************
let fileObjectList = [];

let modal = document.querySelector("#uploadModal");
let dropBox = document.querySelector("#dragDropBox");
let uploadFileListDiv = document.querySelector("#uploadFileList");
let content = document.querySelector("#uploadModalContent");
let btnRow = document.querySelector("#modalBtnRow");
let zipBtn = document.querySelector("#selectZipBtn");
let confirmBtn = document.querySelector("#confirmBtn");
let cloudBox = document.querySelector("#cloudBox");
let selectedBox = document.querySelector("#selectedFiles");

function showModal() {
  modal.classList.remove("hidden");
  modal.addEventListener("wheel", (e) => {
    e.preventDefault();
  });
  uploadFileListDiv.innerHTML = "";
  selectedBox.classList.add("hidden");
  cloudBox.classList.remove("hidden");
  confirmBtn.classList.add("disableBtn");
}

function hideModal() {
  modal.classList.add("hidden");
  fileObjectList = [];
  uploadFileListDiv.innerHTML = "";
  selectedBox.classList.add("hidden");
  cloudBox.classList.remove("hidden");
  confirmBtn.classList.add("disableBtn");
}

async function getFilesFromZip(zipFile) {
  var zipHolder = new JSZip();
  let zipContent = await zipHolder.loadAsync(zipFile);

  let fileNames = [];

  let awaits = [];
  for (let key in zipContent.files) {
    fileNames.push(key);
    awaits.push(zipContent.file(key).async("string"));
  }
  let fileContents = await Promise.all(awaits);

  fileNames.forEach((name, index) => {
    let fileObject = {};
    fileObject[name] = fileContents[index];

    fileObjectList.push(fileObject);
  });
}

function uploadToFirebase() {
  fileObjectList.forEach((fileObject) => {
    let fileName = Object.keys(fileObject)[0];

    let gliderID = fileName.slice(0, 6);
    let missionNum = fileName.substring(
      fileName.lastIndexOf("ID") + 2,
      fileName.lastIndexOf("M")
    );

    let dateRaw = fileName.substring(
      fileName.lastIndexOf("M") + 1,
      fileName.lastIndexOf(".")
    );
    let date =
      dateRaw.substring(0, 4) +
      "-" +
      dateRaw.substring(4, 6) +
      "-" +
      dateRaw.substring(6, 8);

    let dateValue = {};

    fileObject[fileName].split("&").forEach((row) => {
      try {
        let JSONRow = JSON.parse(row);
        let timestamp = Object.keys(JSONRow)[0];
        let value = JSON.stringify(JSONRow[timestamp]).slice(1, -1);
        dateValue[timestamp] = value;
      } catch (e) {
        console.log("not JSON");
      }
    });

    // db.collection("Gliders")
    //   .doc(gliderID)
    //   .collection("Missions")
    //   .doc("Mission " + missionNum)
    //   .collection("Data")
    //   .doc(date)
    //   .set(dateValue, { merge: true });
  });
  generatePopup();
  hideModal();
}

function generatePopup() {
  console.log("Generating!");
  let element = document.querySelector("#uploadSuccess");
  var cloned = element.cloneNode(true);
  cloned.style.webkitAnimationPlayState = "running";
  element.parentNode.replaceChild(cloned, element);
}

function uploadData() {
  fileObjectList = [];
  uploadFileListDiv.innerHTML = "";
  confirmBtn.classList.add("disableBtn");

  let input = document.createElement("input");
  input.setAttribute("multiple", "");
  input.type = "file";

  input.onchange = async (e) => {
    // you can use this method to get file and perform respective operations
    let zipFiles = Array.from(input.files);
    zipFiles.forEach((zip) => {
      let zipFileHTML = `<p class="border-b">${zip.name}</p>`;
      uploadFileListDiv.innerHTML += zipFileHTML;
      getFilesFromZip(zip);
    });

    selectedBox.classList.remove("hidden");
    cloudBox.classList.add("hidden");
    confirmBtn.classList.remove("disableBtn");
  };

  input.click();
}

function dropHandler(e) {
  fileObjectList = [];
  e.preventDefault(); // Prevent file from being insta-opened

  let zipFiles = Array.from(e.dataTransfer.items);
  zipFiles.forEach((zip) => {
    let zipFile = zip.getAsFile();
    let zipFileHTML = `<p class="border-b">${zipFile.name}</p>`;
    uploadFileListDiv.innerHTML += zipFileHTML;
    getFilesFromZip(zip);
  });

  selectedBox.classList.remove("hidden");
  cloudBox.classList.add("hidden");
  confirmBtn.classList.remove("disableBtn");
  dropBox.style.borderColor = "#D1D5DB";
}

function dragOverHandler(e) {
  uploadFileListDiv.innerHTML = "";
  selectedBox.classList.add("hidden");
  cloudBox.classList.remove("hidden");
  confirmBtn.classList.add("disableBtn");
  dropBox.style.borderColor = "#5F7EA9";
  e.preventDefault();
}

function dragOverLeave(e) {
  dropBox.style.borderColor = "#D1D5DB";
}

uploadFileListDiv.addEventListener("wheel", (e) => {
  e.preventDefault();
  let scrollTo = e.wheelDelta * -(50 / 100);
  uploadFileListDiv.scrollTop = scrollTo + uploadFileListDiv.scrollTop;
});

createAllCards();
