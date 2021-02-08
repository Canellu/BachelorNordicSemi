var releaseVal = 0;
var releaseValOld = 0;
var slider = document.querySelector(".neumorphic-slider");
var sliderThumb = document.querySelector(".neumorphic-slider__thumb");
var sliderPopUp = document.querySelector(".neumorphic-slider__popover");

//Slider
function mMove(e) {
  var newVal = 0;
  if (e.clientX < slider.offsetLeft) {
    newVal = "0%";
  }
  else if (e.clientX > (slider.offsetLeft + slider.clientWidth - 10)) {
    newVal = "100%";
  }
  else {
    newVal = ((e.clientX - slider.offsetLeft) / (slider.clientWidth - 10) * 100).toFixed(0) + "%";
  }
  releaseVal = newVal;
  document.documentElement.style.setProperty("--value", newVal);
  sliderPopUp.textContent = newVal;
}//mouseMove

sliderThumb.addEventListener("mousedown", () => {
  document.addEventListener("mousemove", mMove);
  document.addEventListener('mouseup', () => {
    if (releaseVal != releaseValOld)
    {
      releaseValOld = releaseVal;
      updateDoc(`battery ${releaseVal}`);
    }
    document.removeEventListener('mousemove', mMove);
  });

});//Slider