document.write(`

<header class="z-50 lg:fixed top-0 bg-dark text-light h-14 sm:h-16 w-full">
  <div
    class="container flex justify-between m-auto h-full items-center px-2"
  >
    <!--  OASYS logo -->
    <a href="home.html" class="px-2 py-2">
      <h4 class="text-2xl font-bold">OASYS</h4>
    </a>

    <!-- Mobile Links Icons -->
    <div class="sm:hidden flex items-center font-semibold space-x-1">
      <a
        href="home.html"
        class="flex justify-center items-center bg-dark hover:bg-gray-100 hover:text-gray-800 duration-300 p-2 rounded-full"
      >
        <span class="material-icons"> home </span>
      </a>

      <a
        href="about.html"
        class="flex justify-center items-center bg-dark hover:bg-gray-100 hover:text-gray-800 duration-300 p-2 rounded-full"
      >
        <span class="material-icons"> info </span>
      </a>

      <a
        href="index.html"
        id="logout"
        class="flex justify-center items-center bg-dark hover:bg-gray-100 hover:text-gray-800 duration-300 p-2 rounded-full"
        onclick="logOut()"
      >
        <span class="material-icons"> logout </span>
      </a>
    </div>

    <!-- Navigation links -->
    <div class="hidden sm:flex items-center space-x-2 font-semibold">
      <a
        href="home.html"
        class="flex justify-center items-center bg-dark hover:bg-gray-100 hover:text-gray-800 duration-300 px-6 py-2 rounded-full w-32"
      >
        <h4>Devices</h4>
      </a>

      <a
        href="about.html"
        class="flex justify-center items-center bg-dark hover:bg-gray-100 hover:text-gray-800 duration-300 px-6 py-2 rounded-full w-32"
      >
        <h4>About</h4>
      </a>

      <a
        href="index.html"
        id="logout"
        class="flex justify-center items-center bg-dark hover:bg-gray-100 hover:text-gray-800 duration-300 px-6 py-2 rounded-full w-32"
        onclick="logOut()"
      >
        <h4>Logout</h4>
      </a>
    </div>
  </div>
</header>

`);
