// *********************** SLIDEEEEEERS *************************
function createSliderHTML(title, min, max) {
  let type = title.replaceAll(" ", "");

  let previewHTML = `<div class="flex justify-between width-full border-b"><p>${title}:  </p><p id="preview${type}"> 0 </p></div>`;

  document.querySelector("#previewParams").innerHTML += previewHTML;

  let sliderHTML = `
    <div class="sliderBox">
      <h1 class="font-medium text-gray-800">${title}</h1>
      <div id="slider${type}" class="cursor-pointer w-full mx-6 relative">
        <span class="absolute text-xs right-1">${max}</span>
      </div>
  
      <input
        type="number"
        min="0"
        step="1"
        id="input${type}"
        class="w-16 text-md font-medium text-dark border pl-1 rounded-sm focus:outline-none"
      />
    </div>`;

  document.querySelector("#sliders").innerHTML += sliderHTML;
}

function createSlider(title, min, max, start) {
  // Add slider to sliders-flex-div
  let type = title.replaceAll(" ", "");
  // Create slider
  let sliderDiv = document.querySelector(`#slider${type}`);
  let inputDiv = document.querySelector(`#input${type}`);

  noUiSlider.create(sliderDiv, {
    start: [start],
    step: 1,
    connect: [true, false],
    //   tooltips: [wNumb({ decimals: 0 })],
    range: {
      min: min,
      max: max,
    },
    format: wNumb({
      decimals: 0,
    }),
  });

  // Link slider to input
  sliderDiv.noUiSlider.on("update", function (values, handle) {
    inputDiv.value = values[handle];
  });

  sliderDiv.noUiSlider.on("change", () => {
    document.querySelector(`#preview${type}`).innerText = inputDiv.value;
  });

  // Link input to slider
  inputDiv.addEventListener("change", function () {
    sliderDiv.noUiSlider.set(this.value);
    document.querySelector(`#preview${type}`).innerText = inputDiv.value;
  });
}

let sliderList = [
  { title: "Conductivity", min: 0, max: 20, start: 0 },
  { title: "Pressure", min: 0, max: 20, start: 0 },
  { title: "Temperature", min: 0, max: 20, start: 0 },
  { title: "Min Depth", min: 0, max: 300, start: 0 },
  { title: "Max Depth", min: 0, max: 300, start: 0 },
];

sliderList.forEach((obj) => {
  createSliderHTML(obj.title, obj.min, obj.max);
});
sliderList.forEach((obj) => {
  createSlider(obj.title, obj.min, obj.max, obj.start);
});

// *********************** WAYPOINTS ***********************************

let isEditMode = 0;

let latInput = document.querySelector("#addLat");
let lngInput = document.querySelector("#addLng");

function resetInput() {
  latInput.classList.remove("redBorder");
  lngInput.classList.remove("redBorder");
}
function addWaypoint() {
  if (latInput.value != "" && lngInput.value != "") {
    let path = missionWaypoints.getPath();
    let latLng = new google.maps.LatLng(latInput.value, lngInput.value);
    path.push(latLng);
    latInput.value = "";
    lngInput.value = "";
  } else {
    if (latInput.value == "") latInput.classList.add("redBorder");
    if (lngInput.value == "") lngInput.classList.add("redBorder");
  }

  renderWaypointList();
}

function lockIt(e) {
  e.preventDefault();
  let div = document.querySelector("#waypointList");
  let scrollTo = e.wheelDelta * -(100 / 100);
  div.scrollTop = scrollTo + div.scrollTop;
}

function toggleScrollBar() {
  let div = document.querySelector("#waypointList");
  let scrollBar = div.clientHeight < div.scrollHeight;
  if (scrollBar) {
    div.addEventListener("wheel", lockIt);
  } else {
    div.removeEventListener("wheel", lockIt);
  }
}

function renderWaypointList() {
  let waypointList = document.querySelector("#waypointList");
  document.querySelector("#addLatLngIndex").innerText =
    missionWaypoints.getPath().length + 1;

  waypointList.innerHTML = "";

  missionWaypoints.getPath().forEach((waypoint, index) => {
    let lat = waypoint.lat();
    let lng = waypoint.lng();
    let bg = "";
    if (isEditMode) {
      bg = "bg-white";
    } else {
      bg = "bg-light";
    }

    let waypointItem = `
      <div class="waypoint-item grid grid-cols-8 gap-x-2"  data-index="${index}">
        <h1
          class="col-span-1 flex justify-center items-center text-sm font-medium mt-2">${
            index + 1
          }
        </h1>
        <input
          readonly
          class="col-span-3 text-right text-sm font-medium pl-1 rounded-sm focus:outline-none ${bg} pr-2 border-b-2"
          value="${lat.toFixed(6)}"
        />
        <input
          readonly
          class="col-span-3 text-right text-sm font-medium pl-1 rounded-sm focus:outline-none ${bg} pr-2 border-b-2"
          value="${lng.toFixed(6)}"
        />
    
        <span onclick="deleteWaypoint(this)" class="col-span-1 material-icons chartBtn" style="transform: scale(${isEditMode});"> clear </span>
   
      </div>`;

    waypointList.innerHTML += waypointItem;
  });

  toggleScrollBar();
}

function deleteWaypoint(clearBtn) {
  let index = parseInt(clearBtn.parentElement.getAttribute("data-index"));
  missionWaypoints.getPath().removeAt(index);
}

let editWaypointsBtn = document.querySelector("#editWaypointsBtn");

editWaypointsBtn.addEventListener("click", () => {
  let waypointListInputs = document.querySelectorAll(".waypoint-item input");

  if (editWaypointsBtn.innerText == "edit") {
    // Remove readonly and whiten bg
    waypointListInputs.forEach((input) => {
      input.removeAttribute("readonly");
      input.style.backgroundColor = "white";
    });

    isEditMode = 1;
    document.querySelectorAll(".waypoint-item span").forEach((btn) => {
      btn.style.transform = "scale(1)";
    });

    editWaypointsBtn.innerText = "done";
  } else {
    isEditMode = 0;
    editWaypointsBtn.innerText = "edit";

    // Set input readonly and "remove" bg
    waypointListInputs.forEach((input) => {
      input.setAttribute("readonly", "");
      input.style.backgroundColor = "#F9FAFB";
    });

    let path = missionWaypoints.getPath();
    let updatedWaypoints = [];

    document.querySelectorAll(".waypoint-item").forEach((row) => {
      let children = row.children;
      let lat = children[1].value;
      let lng = children[2].value;

      updatedWaypoints.push(new google.maps.LatLng(lat, lng));
    });

    path.forEach((waypoint, index) => {
      let currentLat = updatedWaypoints[index].lat();
      let currentLng = updatedWaypoints[index].lng();

      if (currentLat != waypoint.lat() || currentLng != waypoint.lng()) {
        path.setAt(index, updatedWaypoints[index]);
        console.log("Changed!");
      }
    });

    renderWaypointList();
  }
});

// ************************* PREVIEW ***********************************
let sendMissionParamsBtn = document.querySelector("#sendMissionParams");
let confirmMissionParamsBtn = document.querySelector("#confirmMissionParams");
let yesMissionParamsBtn = document.querySelector("#yesMissionParams");
let noMissionParamsBtn = document.querySelector("#noMissionParams");
let resetMissionParamsBtn = document.querySelector("#resetMissionParams");
let sliderPreviews = document.querySelectorAll("#previewParams [id^=preview]");

resetMissionParamsBtn.addEventListener("click", () => {
  // Reset sliders to 0
  document.querySelectorAll("#sliders [id^=slider]").forEach((slider) => {
    slider.noUiSlider.set(0);
  });

  // Reset sliderpreviews to 0
  sliderPreviews.forEach((preview) => {
    preview.innerText = "0";
  });

  // // Reset datetimes to blank
  document.querySelector("#datepickerControl").value = "";
  document.querySelector("#timepickerControl").value = "";
  document.querySelector("#previewDate").innerText = "YYYY-MM-DD";
  document.querySelector("#previewTime").innerText = "--:--";

  // Reset waypoints added to Map
  missionWaypoints.getPath().clear();
});

yesMissionParamsBtn.addEventListener("click", async () => {
  let date = document.querySelector("#previewDate").innerText;
  let time = document.querySelector("#previewTime").innerText;

  let missionParameters = {
    start: `${date} ${time}`,
  };

  sliderPreviews.forEach((preview) => {
    let propertyName = preview.id.replace("preview", "");
    let propertyVal = preview.innerText;
    // console.log({ propertyName, propertyVal });
    missionParameters[propertyName] = propertyVal;
  });

  let waypoints = missionWaypoints.getPath().Nb;
  let waypointsToSend = [];

  waypoints.forEach((waypoint) => {
    let lat = waypoint.lat().toFixed(6);
    let lng = waypoint.lng().toFixed(6);
    // console.log({ lat, lng });
    waypointsToSend.push(`${lat}, ${lng}`);
  });

  missionParameters["Waypoints"] = waypointsToSend;

  let latestMissionNumber = document.querySelector("#latestMissionNumber")
    .innerText;

  latestMission++;
  document.querySelector("#latestMissionNumber").innerText =
    "Mission " + (latestMission + 1);

  // let missions = await db
  //   .collection("Gliders")
  //   .doc(gliderUID)
  //   .collection("Missions")
  //   .doc(latestMissionNumber)
  //   .set(missionParameters);

  confirmMissionParamsBtn.style.transform = "scale(0)";
  sendMissionParamsBtn.style.transform = "scale(1)";

  setTimeout(() => {
    confirmMissionParamsBtn.classList.add("hidden");
    sendMissionParamsBtn.classList.remove("hidden");
  }, 200);

  console.log("clicked yes");
});

noMissionParamsBtn.addEventListener("click", () => {
  confirmMissionParamsBtn.style.transform = "scale(0)";
  sendMissionParamsBtn.style.transform = "scale(1)";

  setTimeout(() => {
    confirmMissionParamsBtn.classList.add("hidden");
    sendMissionParamsBtn.classList.remove("hidden");
  }, 200);
});

sendMissionParamsBtn.addEventListener("click", () => {
  sendMissionParamsBtn.style.transform = "scale(0)";
  confirmMissionParamsBtn.style.transform = "scale(1)";
  setTimeout(() => {
    sendMissionParamsBtn.classList.add("hidden");
    confirmMissionParamsBtn.classList.remove("hidden");
  }, 200);
});
