var dataTabBtn = document.querySelector("#data-tab-btn");
var dataTab = document.querySelector("#data-tab");
var controlTabBtn = document.querySelector("#control-tab-btn");
var controlTab = document.querySelector("#control-tab");
var dropDownBtn = document.querySelector(".dropDownBtn");
var dropDownContent = document.querySelector(".dropDownContent");

dataTabBtn.addEventListener("click", () => {
  dataTab.classList.remove("hidden");
  controlTab.classList.add("hidden");

  dataTabBtn.classList.add("activeTabBtn");
  controlTabBtn.classList.remove("activeTabBtn");
});

controlTabBtn.addEventListener("click", () => {
  controlTab.classList.remove("hidden");
  dataTab.classList.add("hidden");

  controlTabBtn.classList.add("activeTabBtn");
  dataTabBtn.classList.remove("activeTabBtn");
});

dropDownBtn.addEventListener("mouseover", () => {
  dropDownContent.classList.remove("hidden");
  // let body = document.querySelector("body");
  // document.body.style.overflow = "hidden";
});
dropDownBtn.addEventListener("mouseleave", () => {
  dropDownContent.classList.add("hidden");
  // document.body.style.overflow = "auto";
});

// let url = window.location.pathname;
// console.log(url.substring(url.lastIndexOf("/") + 1).replace(".html", ""));

var charts = []; //Conductivity, Pressure, Temperature
var activeMission = "";
// Store all chartdata from clicked missions
var missionDataset = {};

// When clicking on a dropdown mission, retrieve data from firestore and update charts
async function listMissions() {
  // All mission documents for this glider
  const missions = await db
    .collection("Gliders")
    .doc("807381")
    .collection("Missions")
    .get();

  let totalNumberOfMissions = 0;
  // Loop through all mission-docs
  missions.forEach((mission) => {
    // Create dropdown content for each mission
    document.querySelector(
      ".dropDownContent"
    ).innerHTML += `<div class="mission">${mission.id}</div>`;
    totalNumberOfMissions++;
  });

  activeMission = `Mission ${totalNumberOfMissions}`;

  // Add click-event for each dropdown-content
  let missionDivs = document.querySelectorAll(".mission");
  missionDivs.forEach((div) => {
    // When clicked, get data from firestore, update chart
    div.addEventListener("click", async () => {
      document.querySelector(
        ".missionIndicator"
      ).innerText = div.innerText.split(" ")[1];

      // Get data from firestore
      activeMission = div.innerText;
      let data = await getMissionData(div.innerHTML.split(" ")[1]);

      if (!(activeMission in missionDataset)) {
        missionDataset[activeMission] = data;
        console.log(`Getting data from Active mission: ${activeMission}`);
      }
      // Update chart
      charts[0].data.datasets[0].data = data.C;
      charts[1].data.datasets[0].data = data.P;
      charts[2].data.datasets[0].data = data.T;

      charts.forEach((chart) => {
        chart.update();
        chart.resetZoom();
      });
    });
  });

  // missionDivs[missionDivs.length - 1].click();
  document.querySelector("#missionSelector div p").innerText = activeMission;
  document.querySelector(".missionIndicator").innerText = totalNumberOfMissions;
}

// Returns promise of missionData-object
async function getMissionData(missionNum) {
  let dataset = [];

  // Specific mission document
  let mission = await db
    .collection("Gliders")
    .doc("807381")
    .collection("Missions")
    .doc("Mission " + missionNum);

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

  // Extract specific datatype for each chart.
  let dataC = getDataType(dataset, "C");
  let dataP = getDataType(dataset, "P");
  let dataT = getDataType(dataset, "T");

  return { T: dataT, P: dataP, C: dataC };
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

// Creates dropdownlist
listMissions();

createAllCharts();