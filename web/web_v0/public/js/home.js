async function createCard(uid, sync) {
  let html = `  <div class="card">
  <img
    class="object-cover center w-full h-44"
    src="https://source.unsplash.com/random"
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
</div>`;

  document.querySelector("#deviceGrid").innerHTML += html;
}
