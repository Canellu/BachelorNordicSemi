function runTest(module) {
  switch (module) {
    case "Satellite":
      disableClickEvents("Satellite");
      break;
    case "GPS":
      disableClickEvents("GPS");

      // code block
      break;
    case "4G":
      disableClickEvents("4G");

      break;

    case "Sensor":
      disableClickEvents("Sensor");

      break;
    default:
    // code block
  }
}

// Disables all click events while testing.
function disableClickEvents(moduleType) {
  let testBtns = document.querySelectorAll("a[id*=testBtn]");
  console.log(testBtns);

  // Disable moduleBtns while testing.
  testBtns.forEach((testBtn) => {
    if (testBtn.id == `testBtn${moduleType}`) {
      testBtn.style.pointerEvents = "none";

      // TODO: Add animation while testing is running
      // TODO: change message window with some text.
    } else {
      testBtn.classList.add("disabled");
    }
  });
}

// Enables click events when testing is finished.
function enableClickEvents(responseMsg) {
  // TODO: remove animation added by disableClickEvents
  // TODO: change color of moduleBtn to corresponding response OK/ERROR
  // TODO: change message window with some text about response.
  // TODO: enable all buttons
  console.log("test:4G,ERROR");
  console.log("test:gps,OK");
  console.log("test:sensor,ERROR");
}
