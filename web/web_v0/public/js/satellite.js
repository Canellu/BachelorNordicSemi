let satelliteModal = document.querySelector("#satelliteModal");
let satelliteInput = document.querySelector("#inputSatellite");

function showSatelliteModal() {
  satelliteModal.classList.remove("hidden");
}

function hideSatelliteModal() {
  satelliteModal.classList.add("hidden");
}

function s2ab(s) {
  var buf = new ArrayBuffer(s.length);
  var view = new Uint8Array(buf);
  for (var i = 0; i < s.length; i++) view[i] = s.charCodeAt(i) & 0xff;
  return buf;
}

async function sendSatelliteData() {
  let gliderField = await db.collection("Gliders").doc(gliderUID).get();
  let satIMEI = gliderField.data()["Sat IMEI"];

  console.log(" ************************ Message: " + satelliteInput.value);
  console.log(
    " ************************ HexMsg: " + ascii_to_hexa(satelliteInput.value)
  );
  console.log(
    " ************************ ByteArray: " +
      hexStringToArrayBuffer(ascii_to_hexa(satelliteInput.value))
  );

  // // TEST
  // var enc = new TextEncoder(); // always utf-8
  // console.log(enc.encode(satelliteInput.value));

  let queryParams = {
    imei: satIMEI,
    username: "nordicoasys@gmail.com",
    password: "Bachelorgroup2021",
    data: ascii_to_hexa(satelliteInput.value),
    // flush: "yes", // Optional
  };

  let queryString = "";
  for (const [key, value] of Object.entries(queryParams)) {
    console.log(`${key}: ${value}`);
    queryString += key + "=" + value + "&";
  }
  console.log(queryString);

  //   fetch(`https://rockblock.rock7.com/rockblock/MT?${queryString}`, {method: "POST"})
  //   .then(response => console.log(response))
  //   .catch(err => console.error(err));
  satelliteInput.value = "";
}

function ascii_to_hexa(str) {
  let arr = [];
  for (let n = 0; n < str.length; n++) {
    let hex = Number(str.charCodeAt(n)).toString(16);
    arr.push(hex);
  }
  return arr.join("");
}

/**
 * Convert a hex string to an ArrayBuffer.
 *
 * @param {string} hexString - hex representation of bytes
 * @return {ArrayBuffer} - The bytes in an ArrayBuffer.
 */
function hexStringToArrayBuffer(hexString) {
  // remove the leading 0x
  hexString = hexString.replace(/^0x/, "");

  // ensure even number of characters
  if (hexString.length % 2 != 0) {
    console.log(
      "WARNING: expecting an even number of characters in the hexString"
    );
  }

  // check for some non-hex characters
  var bad = hexString.match(/[G-Z\s]/i);
  if (bad) {
    console.log("WARNING: found non-hex characters", bad);
  }

  // split the string into pairs of octets
  var pairs = hexString.match(/[\dA-F]{2}/gi);

  // convert the octets to integers
  var integers = pairs.map(function (s) {
    return parseInt(s, 16);
  });

  var array = new Uint8Array(integers);
  console.log(array);

  return array.buffer;
}
