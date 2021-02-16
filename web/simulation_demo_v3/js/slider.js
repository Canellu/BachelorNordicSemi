var slider1 = document.querySelector("#slider1");
var slider2 = document.querySelector("#slider2");
var slider3 = document.querySelector("#slider3");
var s1 = document.querySelector(".s1");
var s2 = document.querySelector(".s2");
var s3 = document.querySelector(".s3");

slider1.oninput = () => {
  s1.innerHTML = slider1.value;
};
slider2.oninput = () => {
  s2.innerHTML = slider2.value;
};
slider3.oninput = () => {
  s3.innerHTML = slider3.value;
};

slider1.addEventListener("mousemove", () => {
  var x = slider1.value;
  var color =
    "linear-gradient(90deg, rgb(0, 194, 255)" +
    x +
    "%, rgb(200, 200, 200)" +
    x +
    "%";
  slider1.style.background = color;
});
slider2.addEventListener("mousemove", () => {
  var x = slider2.value;
  var color =
    "linear-gradient(90deg, rgb(0, 194, 255)" +
    x +
    "%, rgb(200, 200, 200)" +
    x +
    "%";
  slider2.style.background = color;
});
slider3.addEventListener("mousemove", () => {
  var x = slider3.value;
  var color =
    "linear-gradient(90deg, rgb(0, 194, 255)" +
    x +
    "%, rgb(200, 200, 200)" +
    x +
    "%";
  slider3.style.background = color;
});
