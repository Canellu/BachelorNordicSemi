let satelliteModal = document.querySelector("#satelliteModal");
let satelliteInput = document.querySelector("#inputSatellite");
let satelliteConsole = document.querySelector("#consoleText");

function showSatelliteModal() {
  satelliteModal.classList.remove("hidden");
}

function hideSatelliteModal() {
  satelliteModal.classList.add("hidden");
}

function ascii_to_hexa(str) {
  let arr = [];
  for (let n = 0; n < str.length; n++) {
    let hex = Number(str.charCodeAt(n)).toString(16);
    arr.push(hex);
  }
  return arr.join("");
}

async function sendSatelliteData() {
  let gliderField = await db.collection("Gliders").doc(gliderUID).get();
  let satIMEI = gliderField.data()["Sat IMEI"];

  let queryParams = {
    imei: satIMEI,
    username: "nordicoasys@gmail.com",
    password: "Bachelorgroup2021",
    data: ascii_to_hexa(satelliteInput.value),
    //flush: "yes", // Optional
  };

  let queryString = "";
  for (const [key, value] of Object.entries(queryParams)) {
    console.log(`${key}: ${value}`);
    queryString += key + "=" + value + "&";
  }
  console.log(queryString);
  console.log(satelliteInput.value.length);

  const options = { method: "POST", mode: "no-cors" };

  // fetch(`https://rockblock.rock7.com/rockblock/MT?${queryString}`, options)
  //   .then((response) => console.log(response.body))
  //   .catch((err) => console.err(err));
  // satelliteInput.value = "";
}
