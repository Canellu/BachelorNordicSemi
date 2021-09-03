let scrollDiv = document.querySelector(".scrollable");

scrollDiv.addEventListener("wheel", (e) => {
  e.preventDefault();
  let scrollTo = e.wheelDelta * -1;
  scrollDiv.scrollTop = scrollTo + scrollDiv.scrollTop;
});
