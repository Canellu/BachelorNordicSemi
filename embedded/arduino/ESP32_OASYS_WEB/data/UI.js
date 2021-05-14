// --------------------------------------------------------------------
// Logging frequencies
// --------------------------------------------------------------------

let tempGroupBtns = document.querySelectorAll("[name=temperatureGroup]");
let tempText = document.querySelectorAll(
  "[name=temperatureGroup] + .radioText"
);
let presGroupBtns = document.querySelectorAll("[name=pressureGroup]");
let presText = document.querySelectorAll("[name=pressureGroup] + .radioText");
let condGroupBtns = document.querySelectorAll("[name=conductivityGroup]");
let condText = document.querySelectorAll(
  "[name=conductivityGroup] + .radioText"
);

tempGroupBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    tempText.forEach((text) => {
      text.classList.remove("radioTextActive");
    });
    btn.nextElementSibling.classList.add("radioTextActive");
  });
});

presGroupBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    presText.forEach((text) => {
      text.classList.remove("radioTextActive");
    });
    btn.nextElementSibling.classList.add("radioTextActive");
  });
});

condGroupBtns.forEach((btn) => {
  btn.addEventListener("change", (e) => {
    condText.forEach((text) => {
      text.classList.remove("radioTextActive");
    });
    btn.nextElementSibling.classList.add("radioTextActive");
  });
});

// --------------------------------------------------------------------
// Dive Depth
// --------------------------------------------------------------------

let sliderDepth = document.querySelector("#sliderDepth");
let inputMinDepth = document.querySelector("#inputMinDepth");
let inputMaxDepth = document.querySelector("#inputMaxDepth");

noUiSlider.create(sliderDepth, {
  start: [100, 300],
  connect: true,
  step: 1,
  orientation: "horizontal", // 'horizontal' or 'vertical'
  range: {
    min: 0,
    max: 300,
  },
  // tooltips: [true, true],
  // pips: {
  //   mode: "range",
  //   density: 4,
  // },
  format: wNumb({
    decimals: 0,
  }),
});

// Link slider to input
sliderDepth.noUiSlider.on("update", function (values, handle) {
  inputMinDepth.value = values[0];
  inputMaxDepth.value = values[1];
});

// Link input to slider
inputMinDepth.addEventListener("change", function () {
  sliderDepth.noUiSlider.set([this.value, null]);
});

inputMaxDepth.addEventListener("change", function () {
  sliderDepth.noUiSlider.set([null, this.value]);
});

// --------------------------------------------------------------------
// 4G Message Limit
// --------------------------------------------------------------------

let slider4GLimit = document.querySelector("#slider4GLimit");
let input4GLimit = document.querySelector("#input4GLimit");

let sliderRange4G = {
  min: [0],
  "20%": [20],
  "40%": [40],
  "60%": [60],
  "80%": [80],
  max: [100],
};
noUiSlider.create(slider4GLimit, {
  start: 50,
  connect: [true, false],
  step: 1,
  orientation: "horizontal", // 'horizontal' or 'vertical'
  range: sliderRange4G,
  // pips: {
  //   mode: "range",
  //   density: 4,
  // },
  format: wNumb({
    decimals: 0,
  }),
});

// Link slider to input
slider4GLimit.noUiSlider.on("update", function (values, handle) {
  input4GLimit.value = values[handle];
});

// Link input to slider
input4GLimit.addEventListener("change", function () {
  slider4GLimit.noUiSlider.set([this.value]);
});

// --------------------------------------------------------------------
// Waypoints
// --------------------------------------------------------------------

let waypointListDiv = document.querySelector("#waypointList");
let wpLatInput = document.querySelector("#wpLat");
let wpLngInput = document.querySelector("#wpLng");
let waypoints = [];
let index = 0;
function addWaypoint() {
  let lat = Number(Number(wpLatInput.value).toFixed(4));
  let lng = Number(Number(wpLngInput.value).toFixed(4));

  if (!(lat == "" || lng == "")) {
    let row = `  
    <div class="row" style="margin-bottom: 10px">
      <div class="col s2" id="numberBox">${index + 1}</div>
      <div class="col s4">
        <input type="number" id="latlngInputBox" value="${lat}" />
      </div>
      <div class="col s4">
        <input type="number" id="latlngInputBox" value="${lng}"  />
      </div>
      <div class="col s2" onclick="deleteRow(this)">
        <div class="smallBtn">
          <img src="delete.svg" alt="" />
        </div>
      </div>
    </div>
    `;

    waypointListDiv.innerHTML += row;
    waypoints.push({ lat, lng });
    index++;
  }

  wpLatInput.value = "";
  wpLngInput.value = "";
}

// --------------------------------------------------------------------
// CONFIGURE BUTTON
// --------------------------------------------------------------------

function sendMissionParams() {
  let currentMissionNum = document.querySelector("#inputMissionNum").value;
  let maxDepth = document.querySelector("#inputMaxDepth").value;
  let minDepth = document.querySelector("#inputMinDepth").value;
  let msgLimit4G = document.querySelector("#input4GLimit").value;
  let startDate = document
    .querySelector("#startDate")
    .value.replaceAll("-", "");
  let startTime = document
    .querySelector("#startTime")
    .value.replaceAll(":", "");
  let conductivityFreq;
  let pressureFreq;
  let temperatureFreq;
  condGroupBtns.forEach((btn) => {
    if (btn.checked) {
      conductivityFreq = freqModeToNum(btn.nextElementSibling.innerText);
    }
  });
  presGroupBtns.forEach((btn) => {
    if (btn.checked) {
      pressureFreq = freqModeToNum(btn.nextElementSibling.innerText);
    }
  });
  tempGroupBtns.forEach((btn) => {
    if (btn.checked) {
      temperatureFreq = freqModeToNum(btn.nextElementSibling.innerText);
    }
  });

  let latArr = waypoints.map((obj) => obj.lat.toString());
  let lngArr = waypoints.map((obj) => obj.lng.toString());

  let params = {
    M: parseInt(currentMissionNum),
    "4G": parseInt(msgLimit4G),
    C: conductivityFreq,
    P: pressureFreq,
    T: temperatureFreq,
    lat: latArr,
    lng: lngArr,
    maxD: parseInt(maxDepth),
    minD: parseInt(minDepth),
    start: startDate + startTime,
  };

  websocket.send(JSON.stringify(params));
  console.log(JSON.stringify(params));

  // resetParams();
}

function resetParams() {
  waypoints = [];
  waypointListDiv.innerHTML = "";
  index = 0;
}

// --------------------------------------------------------------------
// HELPER FUNCTIONS
// --------------------------------------------------------------------

// Scrolls window to top
function scrollToTop() {
  document.body.scrollTop = 0;
  document.documentElement.scrollTop = 0;
}

// Converts logging level to integer
function freqModeToNum(mode) {
  switch (mode) {
    case "High":
      return 3;
      break;
    case "Medium":
      return 2;
      break;
    case "Low":
      return 1;
      break;
    case "None":
      return 0;
      break;
  }
}

// Disabling outer scroll while scrolling in this div
// function addScrollLock(div, scrollSpeed = 100) {
//   div.addEventListener(
//     "wheel",
//     (e) => {
//       e.preventDefault();
//       let scrollTo = e.wheelDelta * -(scrollSpeed / 100);
//       div.scrollTop = scrollTo + div.scrollTop;
//     },
//     { passive: false }
//   );
// }
// addScrollLock(fileListBody, 50);

// Latlng row delete
function deleteRow(e) {
  e.parentElement.remove();
}
