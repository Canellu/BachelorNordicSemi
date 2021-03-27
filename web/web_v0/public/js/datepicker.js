const datePicker = new Litepicker({
  element: document.getElementById("datePicker"),
  singleMode: false,
  delimiter: "    |    ",
});

document.getElementById("datePicker").addEventListener("click", () => {
  datePicker.clearSelection();
});

async function applySelection() {
  document.querySelector("#missionSelector div p").innerText = "Date range";

  let splitDate = document.getElementById("datePicker").value.split("|");

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
