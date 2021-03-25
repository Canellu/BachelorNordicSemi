const datePicker = new Litepicker({
  element: document.getElementById("datePicker"),
  singleMode: false,
  delimiter: "    |   ",
});

document.getElementById("datePicker").addEventListener("click", () => {
  datePicker.clearSelection();
});

async function applySelection() {
  document.querySelector("#missionSelector div p").innerText = "Custom Range";

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
        console.log("Already got data");
      } else {
        console.log("Trying to fetch data");
        data = await getMissionData(missionName);
        console.log("Got data from firestore");
        console.log({ data });
        missionDataset[missionName] = data;
        getRangedData(missionName, startDate, endDate, data);
        console.log(data);
      }
    });
  }
}

function getRangedData(missionName, startDate, endDate, data) {
  Object.keys(missionDataset[missionName]).forEach((type) => {
    data[type] = [];
    missionDataset[missionName][type].forEach((row) => {
      let testDate = parseInt(row.t.split("T")[0].replaceAll("-", ""));
      if (testDate >= startDate && testDate <= endDate) {
        data[type].push(row);
      }
    });
  });
}
