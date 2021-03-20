var toSignUp = document.querySelector("#toSignUp");
var toLogin = document.querySelector("#toLogin");
var cardContent = document.querySelector(".cardContent");

toSignUp.addEventListener("click", (e) => {
  e.preventDefault();
  cardContent.classList.add("flipCard");
});

toLogin.addEventListener("click", (e) => {
  e.preventDefault();
  cardContent.classList.remove("flipCard");
});
