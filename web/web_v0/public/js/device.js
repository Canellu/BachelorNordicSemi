var dataTabBtn = document.querySelector("#data-tab-btn");
var dataTab = document.querySelector("#data-tab");
var controlTabBtn = document.querySelector("#control-tab-btn");
var controlTab = document.querySelector("#control-tab");







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
