let satelliteModal = document.querySelector("#satelliteModal");

function showSatelliteModal() {
  satelliteModal.classList.remove("hidden");
}

function hideSatelliteModal() {
  satelliteModal.classList.add("hidden");
}

async function sendSatelliteData() {
  const options = { method: "POST" };
  let gliderField = await db.collection("Gliders").doc(gliderUID).get();
  let satIMEI = gliderField.data()["Sat IMEI"];

  //   fetch(`https://rockblock.rock7.com/rockblock/MT?${queryParams}`, options)
  //   .then(response => console.log(response))
  //   .catch(err => console.error(err));
  let queryParams = {
    imei: satIMEI,
    username: "nordicoasys@gmail.com",
    password: "Bachelorgroup2021",
    data: ascii_to_hexa("hello"),
    flush: "yes",
  };

  let queryString = "";
  for (const [key, value] of Object.entries(queryParams)) {
    console.log(`${key}: ${value}`);
    queryString += key + "=" + value + "&";
  }
  console.log(queryString);
}

function ascii_to_hexa(str) {
  let arr = [];
  for (let n = 0; n < str.length; n++) {
    let hex = Number(str.charCodeAt(n)).toString(16);
    arr.push(hex);
  }
  return arr.join("");
}
