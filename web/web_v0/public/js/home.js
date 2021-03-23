// Log out method
const logout = document.querySelector("#logout");
logout.addEventListener("click", async (e) => {
  await auth.signOut();
  location.replace("index.html");
});

async function createCard(uid, alias, sync, img) {
  let html = `  <div class="card">
  <img
    class="object-cover center w-full h-44"
    src=${img}
    alt=""
  />
  <div class="id flex-grow place-self-center mt-4">
    <h4 class="text-2xl font-semibold tracking-wider">ID: ${uid}</h4>
  </div>

  <div class="indicator flex justify-evenly w-full my-2">
    <span class="material-icons text-3xl"> battery_full </span>
    <span class="material-icons text-3xl"> error </span>
  </div>
  <div class="badge w-full left-2 bg-gray-800 text-white p-2">
    <!-- <i class="far fa-clock fa-lg mr-2"></i> -->
    <p class="tracking-wide text-center">Last sync: ${sync}</p>
  </div>

  <div
  class="badge absolute px-3 py-1 flex justify-center items-center bg-gray-800 top-4 left-4 rounded-lg font-semibold tracking-widest text-gray-200">${alias}</div>
</div>`;

  document.querySelector("#deviceGrid").innerHTML += html;
}

async function createAllCards() {
  const gliders = await db.collection("Gliders").get();
  gliders.forEach((glider) => {
    getGliderFields(glider.id);
  });
}

const images = [
  "assets/img/jellyfish.jpg",
  "assets/img/turtle.jpg",
  "assets/img/shark.jpg",
  "assets/img/water1.jpg",
  "assets/img/water2.jpg",
  "assets/img/fishies.jpg",
];
async function getGliderFields(gliderID) {
  const glider = db.collection("Gliders").doc(gliderID);
  let data = await glider.get();
  let uid = gliderID;
  let alias = data.data()["Alias"];
  let sync = data.data()["Last sync"];
  createCard(uid, alias, sync, images[randNum(0, 5)]);
}

createAllCards();
