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

// Check if user is logged in and redirect them
auth.onAuthStateChanged((user) => {
  if (user) {
    location.replace("home.html");
    console.log("Is logged in!");
  } else {
    // User is logged out
    console.log("Is logged out!");
  }
});

function validateLogIn(errorMsg) {
  let errorDiv = document.querySelector("#loginErrorMessage");
  errorDiv.innerHTML = errorMsg;
  errorDiv.classList.remove("hidden");
}
