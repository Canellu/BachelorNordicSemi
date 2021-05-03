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

var sliderDepth = document.querySelector("#sliderDepth");
var inputMinDepth = document.querySelector("#inputMinDepth");
var inputMaxDepth = document.querySelector("#inputMaxDepth");
noUiSlider.create(sliderDepth, {
  start: [100, 300],
  connect: true,
  step: 1,
  orientation: "horizontal", // 'horizontal' or 'vertical'
  range: {
    min: 0,
    max: 500,
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
  console.log(this.value);
  sliderDepth.noUiSlider.set([this.value, null]);
});

inputMaxDepth.addEventListener("change", function () {
  sliderDepth.noUiSlider.set([null, this.value]);
});
