// *********************** SLIDEEEEEERS *************************
function createSliderHTML(title, min, max) {
  let type = title.replaceAll(" ", "");

  let previewHTML = `<div class="flex justify-between border-b"><p>${title}:  </p><p id="preview${type}"> 0 </p></div>`;

  document.querySelector("#previewParams").innerHTML += previewHTML;

  let sliderHTML = `
    <div class="flex justify-between">
      <h1 class="text-sm font-semibold text-gray-800">${title}</h1>
      <input
        type="number"
        min="0"
        step="1"
        id="input${type}"
        class="w-16 text-md font-medium text-dark border rounded-sm text-center"
      />
    </div>

    <div class="sliderBox w-full pb-3">
      <div id="slider${type}" class="cursor-pointer flex-grow relative">
        <span class="absolute text-xs right-1">${max}</span>
      </div>
    </div>   
    `;

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
  // { title: "Conductivity", min: 0, max: 20, start: 0 },
  // { title: "Pressure", min: 0, max: 20, start: 0 },
  // { title: "Temperature", min: 0, max: 20, start: 0 },
  { title: "Min Depth", min: 0, max: 100, start: 50 },
  { title: "Max Depth", min: 0, max: 300, start: 150 },
];

sliderList.forEach((obj) => {
  createSliderHTML(obj.title, obj.min, obj.max);
});
sliderList.forEach((obj) => {
  createSlider(obj.title, obj.min, obj.max, obj.start);
});
let max4GMsg = 100;
createSlider("4GData", 0, max4GMsg, 50);
document.querySelector("#messageMaxLimit").innerText = max4GMsg;

// *********************** RADIO BUTTONS *******************************
let tempGroupBtns = document.querySelectorAll("input[name=temperatureGroup]");
let condGroupBtns = document.querySelectorAll("input[name=conductivityGroup]");
let presGroupBtns = document.querySelectorAll("input[name=pressureGroup]");
let defaultRadios = document.querySelectorAll(
  "input[type=radio] + label[for*=None]"
);

defaultRadios.forEach((btn) => {
  btn.click();
});

let radioBtns = document.querySelectorAll("input[type=radio]");
radioBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    changeRadioBtnStyle(e.target);
  });
});

function changeRadioBtnStyle(btn, type, check) {
  if (check) {
    btn.nextElementSibling.style.color = "#6BB7BF";
    btn.nextElementSibling.style.fontWeight = "600";
    btn.nextElementSibling.style.letterSpacing = "1px";

    let previewType = document.querySelector(`#preview${type}`);
    previewType !== null
      ? (previewType.innerText = btn.nextElementSibling.innerText)
      : "";
  } else {
    btn.nextElementSibling.style.color = "black";
    btn.nextElementSibling.style.fontWeight = "normal";
    btn.nextElementSibling.style.letterSpacing = "0px";
  }
}

tempGroupBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    tempGroupBtns.forEach((btn) => {
      changeRadioBtnStyle(btn, "Temperature", btn.checked);
    });
  });
});

condGroupBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    condGroupBtns.forEach((btn) => {
      changeRadioBtnStyle(btn, "Conductivity", btn.checked);
    });
  });
});

presGroupBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    presGroupBtns.forEach((btn) => {
      changeRadioBtnStyle(btn, "Pressure", btn.checked);
    });
  });
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
    let latLng = new google.maps.LatLng(
      Number(latInput.value).toFixed(4),
      Number(lngInput.value).toFixed(4)
    );
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
    console.log({ lat, lng });
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
    // Remove readonly
    waypointListInputs.forEach((input) => {
      input.removeAttribute("readonly");
    });

    isEditMode = 1;
    document.querySelectorAll(".waypoint-item span").forEach((btn) => {
      btn.style.transform = "scale(1)";
    });

    editWaypointsBtn.innerText = "done";
  } else {
    isEditMode = 0;
    editWaypointsBtn.innerText = "edit";

    // Set input readonly
    waypointListInputs.forEach((input) => {
      input.setAttribute("readonly", "");
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

// --------- RESET BUTTON ---------------
resetMissionParamsBtn.addEventListener("click", () => {
  // Reset sliders to 0
  document.querySelectorAll("#sliders [id^=slider]").forEach((slider) => {
    slider.noUiSlider.set(0);
  });

  // Reset sliderpreviews to 0
  sliderPreviews.forEach((preview, index) => {
    if (index > 2) preview.innerText = "0";
  });

  // Reset radio btns
  document.querySelectorAll("input[type=radio]").forEach((radioBtn) => {
    let defaultRadio = radioBtn.id.includes("None");
    switch (radioBtn.name.replace("Group", "")) {
      case "temperature":
        defaultRadio ? (radioBtn.checked = true) : (radioBtn.checked = false);
        changeRadioBtnStyle(radioBtn, "Temperature", defaultRadio);
        break;
      case "conductivity":
        defaultRadio ? (radioBtn.checked = true) : (radioBtn.checked = false);
        changeRadioBtnStyle(radioBtn, "Conductivity", defaultRadio);
        break;
      case "pressure":
        defaultRadio ? (radioBtn.checked = true) : (radioBtn.checked = false);
        changeRadioBtnStyle(radioBtn, "Pressure", defaultRadio);
        break;
    }
  });

  // // Reset datetimes to blank
  document.querySelector("#datepickerControl").value = "";
  document.querySelector("#timepickerControl").value = "";
  document.querySelector("#previewDate").innerText = "YYYY-MM-DD";
  document.querySelector("#previewTime").innerText = "--:--";

  // Reset waypoints added to Map
  missionWaypoints.getPath().clear();
});

// --------- YES BUTTON ---------------
yesMissionParamsBtn.addEventListener("click", async () => {
  let date = document
    .querySelector("#previewDate")
    .innerText.replaceAll("-", "");
  let time = document
    .querySelector("#previewTime")
    .innerText.replaceAll(":", "");

  let datetime = date + time;
  let missionParameters = {
    start: datetime,
  };

  // Add preview params into object for sending
  sliderPreviews.forEach((preview) => {
    let propertyName = preview.id.replace("preview", "");
    let propertyVal = preview.innerText;
    console.log({ propertyName, propertyVal });
    switch (propertyName) {
      case "4GData":
        propertyName = "4G";
        propertyVal = Number(propertyVal);
        break;

      case "Conductivity":
        propertyName = "C";
        propertyVal = propertyVal;
        break;

      case "Pressure":
        propertyName = "P";
        propertyVal = propertyVal;
        break;

      case "Temperature":
        propertyName = "T";
        propertyVal = propertyVal;
        break;

      case "MinDepth":
        propertyName = "minD";
        propertyVal = Number(propertyVal);
        break;

      case "MaxDepth":
        propertyName = "maxD";
        propertyVal = Number(propertyVal);
        break;
    }
    missionParameters[propertyName] = propertyVal;
  });

  let waypoints = missionWaypoints.getPath().Nb;
  let waypointsToSend = [];

  waypoints.forEach((waypoint) => {
    let lat = waypoint.lat().toFixed(4);
    let lng = waypoint.lng().toFixed(4);
    // console.log({ lat, lng });
    waypointsToSend.push(`${lat},${lng}`);
  });

  missionParameters["WP"] = waypointsToSend;

  let latestMissionNumber = document.querySelector("#latestMissionNumber")
    .innerText;

  latestMission++;
  document.querySelector("#latestMissionNumber").innerText =
    "Mission " + (latestMission + 1);

  // COMMENT TO ENABLE/DISABLE MISSION PARAM SEND!
  let missions = await db
    .collection("Gliders")
    .doc(gliderUID)
    .collection("Missions")
    .doc(latestMissionNumber)
    .set(missionParameters);

  confirmMissionParamsBtn.style.transform = "scale(0)";
  sendMissionParamsBtn.style.transform = "scale(1)";

  setTimeout(() => {
    confirmMissionParamsBtn.classList.add("hidden");
    sendMissionParamsBtn.classList.remove("hidden");
  }, 200);

  console.log("clicked yes");
});

// --------- NO BUTTON ---------------
noMissionParamsBtn.addEventListener("click", () => {
  confirmMissionParamsBtn.style.transform = "scale(0)";
  sendMissionParamsBtn.style.transform = "scale(1)";

  setTimeout(() => {
    confirmMissionParamsBtn.classList.add("hidden");
    sendMissionParamsBtn.classList.remove("hidden");
  }, 200);
});

// --------- SEND BUTTON ---------------
sendMissionParamsBtn.addEventListener("click", () => {
  sendMissionParamsBtn.style.transform = "scale(0)";
  confirmMissionParamsBtn.style.transform = "scale(1)";
  setTimeout(() => {
    sendMissionParamsBtn.classList.add("hidden");
    confirmMissionParamsBtn.classList.remove("hidden");
  }, 200);
});
