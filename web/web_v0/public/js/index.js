let toSignUp = document.querySelector("#toSignUp");
let toLogin = document.querySelector("#toLogin");
let cardContent = document.querySelector("#cardContent");

toSignUp.addEventListener("click", (e) => {
  e.preventDefault();
  cardContent.classList.add("flipCard");
});

toLogin.addEventListener("click", (e) => {
  e.preventDefault();
  cardContent.classList.remove("flipCard");
});

function validateLogIn(errorMsg) {
  let errorDiv = document.querySelector("#loginErrorMessage");
  errorDiv.innerHTML = errorMsg;
  errorDiv.classList.remove("hidden");
}
