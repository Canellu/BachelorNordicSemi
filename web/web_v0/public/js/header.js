document.write(`<header class="z-50 fixed top-0 bg-dark text-light h-16 w-full">
<div
  class="container flex justify-between m-auto h-full items-center px-10"
>
  <a href="#"> <h4 class="text-2xl font-bold">OASYS</h4></a>
  <div class="w-5/12 grid grid-cols-3 font-semibold h-full items-center">
    <a href="#" class="headerBtn">
      <h4>Devices</h4>
    </a>

    <a href="#" class="headerBtn">
      <h4>About</h4>
    </a>

    <a id="logout" href="#" class="headerBtn" onclick="logOut()">
      <h4>Logout</h4>
    </a>
  </div>
</div>
</header>

  `);
