const datepickerDataDiv = document.querySelector("#datepickerData");
const datepickerControlDiv = document.querySelector("#datepickerControl");

// ********** functions/declarations for DATEpicker in CONTROL tab ****************
const datepickerControl = new Litepicker({
  element: datepickerControlDiv,
  singleMode: true,
  setup: (picker) => {
    picker.on("selected", (date) => {
      let val = document.querySelector("#datepickerControl").value;
      document.querySelector("#previewDate").innerText = val;
    });
  },
});

datepickerControlDiv.addEventListener("click", () => {
  datepickerControl.clearSelection();
  document.querySelector("#previewDate").innerText = "YYYY-MM-DD";
});

// ********** functions/declarations for TIMEpicker in CONTROL tab ****************

let timeField = document.querySelector("#timepickerControl");
let timeContent = document.querySelector("#timepickerContentControl");
let hoursDiv = document.querySelector("#hours");
let minutesDiv = document.querySelector("#minutes");
let time = [];
// Fill hour/minute divs with items
function createHourMinuteItems() {
  // Fill in hours
  for (let i = 0; i < 24; i++) {
    hoursDiv.innerHTML += `<div class="hour-item saveTime" tabindex="-1">${pad(
      i
    )}</div>`;
  }

  // Fill in minutes
  for (let i = 0; i < 60; i += 5) {
    minutesDiv.innerHTML += `<div class="minute-item saveTime" tabindex="-1">${pad(
      i
    )}</div>`;
  }

  document.querySelectorAll(".saveTime").forEach((item) => {
    item.addEventListener("click", (e) => {
      if (e.target.classList.contains("hour-item")) {
        time[0] = e.target.innerText;
      } else {
        time[1] = e.target.innerText;
      }

      if (time.length == 2 && !time.includes(undefined)) {
        timeField.value = `${time[0]}:${time[1]}`;
        time = [];
        timeContent.classList.add("hidden");
        document.querySelector("#previewTime").innerText = timeField.value;
      }
    });
  });
}

timeField.addEventListener("click", (e) => {
  timeContent.classList.toggle("hidden");
  hoursDiv.scrollTop = 0;
  minutesDiv.scrollTop = 0;
  timeField.value = "";
});

window.addEventListener("click", (e) => {
  let timeDivCondition =
    !e.target.classList.contains("saveTime") &&
    e.target.id != "hours" &&
    e.target.id != "minutes" &&
    e.target != timeField;

  if (timeDivCondition) {
    timeContent.classList.add("hidden");
  }
});

createHourMinuteItems();

// *********** functions/declarations for DATEpicker in DATA tab ******************
const datepickerData = new Litepicker({
  element: datepickerDataDiv,
  singleMode: false,
  delimiter: "    |    ",
});

// Clearing date selection on click
datepickerDataDiv.addEventListener("click", () => {
  datepickerData.clearSelection();
});

// Apply dates to show only data within specified dates
async function applySelection() {
  document.querySelector("#missionSelector div p").innerText = "Date range";

  let splitDate = datepickerDataDiv.value.split("|");

  if (splitDate.length > 1) {
    var startDate = parseInt(splitDate[0].replaceAll("-", "").trim());
    var endDate = parseInt(splitDate[1].replaceAll("-", "").trim());

    const missions = await db
      .collection("Gliders")
      .doc("311910")
      .collection("Missions")
      .get();

    let missionStartDate;
    let missionEndDate;
    let missionList = [];
    let data = {};

    // Get mission start and end dates as INT
    // And push the mission if its within the specified date-range
    missions.forEach((mission) => {
      missionStartDate = parseInt(
        mission.data().start.split(" ")[0].replaceAll("-", "")
      );
      missionEndDate = parseInt(
        mission.data().end.split(" ")[0].replaceAll("-", "")
      );

      if (!(endDate < missionStartDate) && !(missionEndDate < startDate)) {
        missionList.push(mission.id);
      }
    });

    missionList.forEach(async (missionName) => {
      if (missionName in missionDataset) {
        getRangedData(missionName, startDate, endDate, data);
        console.log(`Getting data from missionDataset: ${missionName}`);

        // Update chart
        updateDataUI(data);
      } else {
        let fetchedData = await getMissionData(missionName);
        console.log(`Getting data from firestore: ${missionName}`);

        missionDataset[missionName] = fetchedData;
        getRangedData(missionName, startDate, endDate, data);

        // Update chart
        updateDataUI(data);
      }
    });
  }
}

// get data within specified range, only checks data within passed mission
function getRangedData(missionName, startDate, endDate, data) {
  // Loop through all data types
  Object.keys(missionDataset[missionName]).forEach((type) => {
    // check to ensure no overlaps between datatypes
    if (!(type in data)) data[type] = [];

    // check each row
    missionDataset[missionName][type].forEach((row) => {
      // fetch date and test if within specified range, if valid, add entire row
      let testDate = parseInt(row.t.split("T")[0].replaceAll("-", ""));
      if (testDate >= startDate && testDate <= endDate) {
        data[type].push(row);
      }
    });
  });
}
