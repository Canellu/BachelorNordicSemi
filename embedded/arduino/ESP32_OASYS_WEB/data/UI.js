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
let sliderRange = {
  min: [0, 10],
  "20%": [100, 10],
  "40%": [200, 10],
  "60%": [300, 10],
  "80%": [400, 10],
  max: [500, 10],
};
noUiSlider.create(sliderDepth, {
  start: [100, 300],
  connect: true,
  step: 1,
  orientation: "horizontal", // 'horizontal' or 'vertical'
  range: sliderRange,
  pips: {
    mode: "range",
    density: 4,
  },
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
  pips: {
    mode: "range",
    density: 4,
  },
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

let tbodyWP = document.querySelector("#tbodyWP");
let wpLatInput = document.querySelector("#wpLat");
let wpLngInput = document.querySelector("#wpLng");
let waypoints = [];
function addWaypoint() {
  let lat = Number(Number(wpLatInput.value).toFixed(4));
  let lng = Number(Number(wpLngInput.value).toFixed(4));

  if (!(lat == "" || lng == "")) {
    let row = `<tr>
                <td>${waypoints.length + 1}</td>
                <td>${lat}</td>
                <td>${lng}</td>
               </tr>`;

    tbodyWP.innerHTML += row;
    waypoints.push({ lat, lng });
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

  let latArr = waypoints.map((obj) => obj.lat);
  let lngArr = waypoints.map((obj) => obj.lng);

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
}

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
