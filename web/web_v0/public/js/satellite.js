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

// Input character-counter
satelliteInput.addEventListener("input", () => {
  document.querySelector("#byteCounter").innerText =
    satelliteInput.value.length;
});

async function sendSatelliteData() {
  let gliderField = await db.collection("Gliders").doc(gliderUID).get();
  let satIMEI = gliderField.data()["Sat IMEI"];

  let queryParams = {
    imei: satIMEI,
    username: "nordicoasys@gmail.com12",
    password: "Bachelorgroup2021",
    data: ascii_to_hexa(satelliteInput.value),
    //flush: "yes", // Optional
  };

  let queryString = "";
  for (const [key, value] of Object.entries(queryParams)) {
    // console.log(`${key}: ${value}`);
    queryString += key + "=" + value + "&";
  }

  const options = {
    method: "POST",
    mode: "no-cors",
  };

  // fetch(`https://rockblock.rock7.com/rockblock/MT?${queryString}`, options)
  //   .then((response) => console.log("RESPONSE BODY: ", response.body))
  //   .catch((err) => console.err({ err }));
  // satelliteInput.value = "";

  const currentDateTime = moment()
    .tz("Europe/Oslo")
    .format("YYYY-MM-DD HH:mm:ss");

  await db
    .collection("Gliders")
    .doc(gliderUID)
    .collection("Satellite")
    .doc(currentDateTime)
    .set({ Direction: "MT", Payload: satelliteInput.value }, { merge: true });

  satelliteInput.value = "";
}

async function populateSatelliteMessageTable() {
  let tbody = document.querySelector("#satelliteMessageTable tbody");
  tbody.innerHTML = "";

  db.collection("Gliders")
    .doc(gliderUID)
    .collection("Satellite")
    .onSnapshot((snapshot) => {
      snapshot.docChanges().forEach((change) => {
        let msg = change.doc;
        console.log(change.type);
        if (change.type === "added") {
          let date = msg.id.split(" ")[0];
          let time = msg.id.split(" ")[1];
          let payload = msg.data().Payload;
          let direction = msg.data().Direction;
          let color = direction == "MT" ? "#d0e9c6" : "#c4e3f3"; // blue : green
          let icon = direction == "MT" ? "arrow_upward" : "arrow_downward";

          let row = `
          <tr data-datetime="${msg.id}">
            <td>${date}</td>
            <td>${time}</td>
            <td>${payload}</td>
            <td style="background-color: ${color}">
              ${direction}
              <span class="align-middle text-lg font-bold material-icons-outlined">
                ${icon}
              </span>
            </td>
          </tr>`;

          tbody.innerHTML += row;
        }

        if (change.type === "removed") {
          let row = document.querySelector(`[data-datetime='${msg.id}']`);
          row.remove();
        }
      });
    });

  console.log("Getting snapshot listener...");
}
