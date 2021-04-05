var dataTabBtn = document.querySelector("#data-tab-btn");
var dataTab = document.querySelector("#data-tab");
var controlTabBtn = document.querySelector("#control-tab-btn");
var controlTab = document.querySelector("#control-tab");
var tabUnderline = document.querySelector("#tabUnderline");
var dropDownBtn = document.querySelector(".dropDownBtn");
var dropDownContent = document.querySelector(".dropDownContent");

// DATA TAB BUTTON
dataTabBtn.addEventListener("click", () => {
  tabUnderline.style.transform = "";

  tabUnderline.classList.remove("activeTabUnderline");
  dataTab.classList.remove("hidden");
  controlTab.classList.add("hidden");
  dataTabBtn.classList.add("activeTabBtn");
  controlTabBtn.classList.remove("activeTabBtn");
  listMissions();
});

// MISSION CONTROL TAB BUTTON
controlTabBtn.addEventListener("click", () => {
  tabUnderline.style.transform = "translateX(100%)";

  controlTab.classList.remove("hidden");
  dataTab.classList.add("hidden");
  controlTabBtn.classList.add("activeTabBtn");
  dataTabBtn.classList.remove("activeTabBtn");
  if (typeof missionMap == "undefined") {
    // initMissionMap();
  }

  document.querySelector("#latestMissionNumber").innerText =
    "Mission " + (latestMission + 1);
});

dropDownBtn.addEventListener("click", () => {
  dropDownContent.classList.toggle("hidden");
});

window.addEventListener("click", (e) => {
  if (e.target != dropDownBtn && e.target.parentNode != dropDownBtn) {
    dropDownContent.classList.add("hidden");
  }
});

/* ********************************************* */
// Scrollock body when scrolling in this element
// @params div lock body scroll on
// @params Scrollspeed in % of default
function addScrollLock(div, scrollSpeed = 100) {
  div.addEventListener("wheel", (e) => {
    e.preventDefault();
    let scrollTo = e.wheelDelta * -(scrollSpeed / 100);
    div.scrollTop = scrollTo + div.scrollTop;
  });
}
addScrollLock(dropDownContent, 50);
addScrollLock(document.querySelector("#hours"), 50);
addScrollLock(document.querySelector("#minutes"), 20);

// let url = window.location.pathname;
// console.log(url.substring(url.lastIndexOf("/") + 1).replace(".html", ""));

var charts = []; //Conductivity, Pressure, Temperature
var activeMission = "";
var latestMission = 0;

// Store all chartdata from clicked missions
var missionDataset = {};
// When clicking on a dropdown mission, retrieve data from firestore and update charts
async function listMissions() {
  dropDownContent.innerHTML = "";
  // All mission documents for this glider
  let missions = await db
    .collection("Gliders")
    .doc("311910")
    .collection("Missions")
    .get();

  // Loop through all mission-docs
  missions.forEach((mission) => {
    // test if mission is newer than previously fetched
    let missionNum = parseInt(mission.id.split(" ")[1]);
    if (missionNum > latestMission) {
      latestMission = missionNum;
    }
    // Create dropdown content for each mission
    dropDownContent.innerHTML += `<div class="mission">${mission.id}</div>`;
  });

  activeMission = `Mission ${latestMission}`;

  // Add click-event for each dropdown-content
  let missionDivs = document.querySelectorAll(".mission");
  missionDivs.forEach((div) => {
    // When clicked, get data from firestore, update chart
    div.addEventListener("click", async () => {
      // Current mission data
      let data;

      // Get data from firestore
      activeMission = div.innerText;

      // Give missionSelector the current mission as text
      document.querySelector(
        "#missionSelector div p"
      ).innerText = activeMission;

      // Do not retrieve mission if data is already retrieved
      if (activeMission in missionDataset) {
        data = missionDataset[activeMission];
        console.log(`Getting data from missionDataset: ${activeMission}`);
      } else {
        data = await getMissionData(div.innerHTML);
        missionDataset[activeMission] = data;
        console.log(`Getting data from firestore: ${activeMission}`);
      }
      // Update chart
      updateDataUI(data);
    });
  });

  // Click on latest mission
  // missionDivs[missionDivs.length - 1].click();
  document.querySelector("#missionSelector div p").innerText = activeMission;
}

// Returns promise of missionData-object
async function getMissionData(missionName) {
  let dataObj = {};
  let dataset = [];

  let dataCoordinatesRaw = {};
  let coordinates = [];

  // Specific mission document
  let mission = await db
    .collection("Gliders")
    .doc("311910")
    .collection("Missions")
    .doc(missionName);

  // // Mission commands!
  // let getCommands = await mission.get();
  // let commands = getCommands.data();

  let missionData = await mission.collection("Data").get();

  // Looping through data documents
  missionData.forEach((date) => {
    //Get document keys, which is timestamp
    let timestamps = Object.keys(date.data());

    // Loop through each row in document
    timestamps.forEach((timestamp) => {
      // Convert row value, to json-object
      let val = JSON.parse("{" + date.data()[timestamp].slice(0, -1) + "}");

      // For each row, append to dataset
      // row -> { "2020-03-28T02:24:12" : "{"T": 22..."}
      let row = {
        [`${date.id}T${timestamp}`]: val,
      };
      dataset.push(row);
    });
  });

  // Sort the data, to display properly on chart
  dataset.sort(compare);

  // TODO: Find a simpler way to get UNIQUE types.
  // Loop through to find each data type
  let dataTypeFound = [];
  dataset.forEach((dataRow) => {
    timestamp = Object.keys(dataRow);
    dataTypeArray = Object.keys(dataRow[timestamp]);
    dataTypeArray.forEach((dataType) => {
      if (!dataTypeFound.includes(dataType)) {
        dataTypeFound.push(dataType);
      }
    });
  });

  // Loop through found datatypes and add to dataObj
  // lat, lng need to be parsed further
  dataTypeFound.forEach((dataType) => {
    let data = getDataType(dataset, dataType);
    if (dataType === "lng" || dataType === "lat") {
      dataCoordinatesRaw[dataType] = data;
    } else {
      dataObj[dataType] = data;
    }
  });

  // parsing of lat, lng before added to dataObj
  for (let i = 0; i < dataCoordinatesRaw.lat.length; i++) {
    let t = dataCoordinatesRaw.lat[i].t;
    let lat = parseFloat(dataCoordinatesRaw.lat[i].y);
    let lng = parseFloat(dataCoordinatesRaw.lng[i].y);

    coordinates.push({ t: t, lat: lat, lng: lng });
  }
  // add coordinates to dataObj
  dataObj["coordinates"] = coordinates;

  return dataObj;
}

// Returns an array with objects of time:val pair of given type
function getDataType(dataset, type) {
  let tyPair = [];

  dataset.forEach((row) => {
    let datetime = Object.keys(row);
    if (typeof row[datetime][type] != "undefined") {
      tyPair.push({ t: datetime[0], y: row[datetime][type] });
    }
  });
  return tyPair;
}

// Updates chart and map UI with data
function updateDataUI(data) {
  charts.forEach((chartObj) => {
    chartObj.chart.data.datasets[0].data = data[chartObj.type];
    chartObj.chart.update();
    chartObj.chart.resetZoom();
  });

  addDataMarkers(data.coordinates, dataMap);
}

// Creates dropdownlist
listMissions();

createAllCharts();
