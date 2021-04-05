function createSliderHTML(title, min, max) {
  let type = title.replaceAll(" ", "");

  let previewHTML = `<p>${title}:  </p> <p id="preview${type}"> 0 </p>`;

  document.querySelector("#previewBox").innerHTML += previewHTML;

  let sliderHTML = `
    <div class="sliderBox">
      <h1 class="font-semibold text-gray-800">${title}</h1>
      <div id="slider${type}" class="cursor-pointer w-full mx-6 relative">
        <span class="absolute text-xs right-1">${max}</span>
      </div>
  
      <input
        type="number"
        min="0"
        step="1"
        id="input${type}"
        class="w-16 text-lg font-medium border pl-1 rounded-sm focus:outline-none"
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

let sendMissionParamsBtn = document.querySelector("#sendMissionParams");
let resetMissionParamsBtn = document.querySelector("#resetMissionParams");
let sliderPreviews = document.querySelectorAll("#previewBox > [id^=preview]");

resetMissionParamsBtn.addEventListener("click", () => {
  // Reset sliders to 0
  document.querySelectorAll("#sliders [id^=slider]").forEach((slider) => {
    slider.noUiSlider.set(0);
  });

  // Reset sliderpreviews to 0

  sliderPreviews.forEach((preview) => {
    preview.innerText = "0";
  });

  // Reset datetimes to blank
  document.querySelector("#datepickerControl").value = "";
  document.querySelector("#timepickerInputControl").value = "";
  document.querySelector("#previewDate").innerText = "YYYY-MM-DD";
  document.querySelector("#previewTime").innerText = "--:--";
});

sendMissionParamsBtn.addEventListener("click", async () => {
  let date = document.querySelector("#previewDate").innerText;
  let time = document.querySelector("#previewTime").innerText;

  let missionParameters = {
    start: `${date} ${time}`,
  };

  sliderPreviews.forEach((preview) => {
    let propertyName = preview.id.replace("preview", "");
    let propertyVal = preview.innerText;
    missionParameters[propertyName] = propertyVal;
  });

  let latestMissionNumber = document.querySelector("#latestMissionNumber")
    .innerText;

  latestMission++;
  document.querySelector("#latestMissionNumber").innerText =
    "Mission " + (latestMission + 1);

  let missions = await db
    .collection("Gliders")
    .doc("311910")
    .collection("Missions")
    .doc(latestMissionNumber)
    .set(missionParameters);
});
