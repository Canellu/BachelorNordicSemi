let satelliteModal = document.querySelector("#satelliteModal");

async function sendSatelliteData() {
  let gliderField = await db.collection("Gliders").doc(gliderUID).get();
  let satIMEI = gliderField.data()["Sat IMEI"];
  console.log(satIMEI);
}

function showSatelliteModal() {
  satelliteModal.classList.remove("hidden");
}

function hideSatelliteModal() {
  satelliteModal.classList.add("hidden");
}
