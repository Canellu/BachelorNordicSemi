// function is called when a module button is clicked. Arguments are the module name.
function runTest(module) {
  switch (module) {
    case "Satellite":
      startModuleTest("test:sat", "Satellite");
      break;
    case "GPS":
      startModuleTest("test:gps", "GPS");
      break;
    case "4G":
      startModuleTest("test:4G", "4G");
      break;
    case "Sensor":
      startModuleTest("test:sensor", "Sensor");
      break;
  }
}

// Sends message to NRF/ESP32 to start test
function startModuleTest(msg, module) {
  if (websocket.readyState == 1) {
    disableClickEvents(module);
    websocket.send(msg);
  } else {
    document.querySelector("#messageWindow").innerHTML =
      "Couldn't start test... Websocket is not open.";
  }
}

// Disables all click events while testing.
function disableClickEvents(moduleType) {
  let testBtns = document.querySelectorAll("a[id*=testBtn]");

  // Disabling module buttons, adding animation, changing texts
  testBtns.forEach((testBtn) => {
    if (testBtn.id == `testBtn${moduleType}`) {
      testBtn.innerText = "TESTING MODULE";
      testBtn.dataset.testStatus = "tested";
      testBtn.style.pointerEvents = "none";
      testBtn.classList.add("activeTest");

      currentBtnText = testBtn.innerText;
      document.querySelector(
        "#messageWindow"
      ).innerHTML = `Module <i style="font-weight: bold;">${moduleType}</i> is currently being tested. This could take up to a few minutes...`;
    } else {
      testBtn.dataset.testStatus == "tested"
        ? (testBtn.style.pointerEvents = "none")
        : testBtn.classList.add("disabled");
    }
  });

  // Disabling tab navigation
  document.querySelectorAll("svg[fill='currentColor'").forEach((tabIcon) => {
    tabIcon.setAttribute("fill", "#9F9F9F");

    tabIcon.parentNode.parentNode.style.pointerEvents = "none";
  });
}

// Enables click events when testing is finished. Revert buttons back.
function enableClickEvents(responseMsg) {
  let testBtns = document.querySelectorAll("a[id*=testBtn]");
  let moduleName = responseMsg.split(",")[0].split(":")[1];
  let statusCode = responseMsg.split(",")[1];

  testBtns.forEach((testBtn) => {
    if (testBtn.classList.contains("activeTest")) {
      testBtn.innerText = moduleName.toUpperCase();
      testBtn.dataset.testStatus == "";
      testBtn.style.pointerEvents = "auto";
      testBtn.classList.remove("activeTest");

      statusCode == "OK"
        ? (testBtn.style.backgroundColor = "#81C784") // Green
        : (testBtn.style.backgroundColor = "#E57373"); // Red
      document.querySelector(
        "#messageWindow"
      ).innerHTML = `Module <i style="font-weight: bold;">${moduleType}</i> is done testing. Status: ${statusCode}`;
    } else {
      testBtn.classList.remove("disabled");
    }
  });

  // Enable tab navigation
  document.querySelectorAll("svg[fill='#9F9F9F'").forEach((tabIcon) => {
    tabIcon.setAttribute("fill", "currentColor");
    tabIcon.parentNode.parentNode.style.pointerEvents = "auto";
  });
}
