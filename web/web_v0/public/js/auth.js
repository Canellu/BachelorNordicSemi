auth.onAuthStateChanged((user) => {
  if (user) {
    location.replace("home.html");
  } else {
    // User is logged out
  }
});

// Sign up method
const signUpForm = document.querySelector("#signUpForm");
signUpForm.addEventListener("submit", async (e) => {
  e.preventDefault();
  // get user info
  const email = signUpForm["signUpEmail"].value;
  const password = signUpForm["signUpPassword"].value;
  // sign up user
  const credentials = await auth.createUserWithEmailAndPassword(
    email,
    password
  );
  signUpForm.reset();
});

//Sign in method
const loginForm = document.querySelector("#loginForm");
loginForm.addEventListener("submit", async (e) => {
  e.preventDefault();

  // get user info
  const email = loginForm["loginEmail"].value;
  const password = loginForm["loginPassword"].value;

  // log in user
  const credentials = await auth.signInWithEmailAndPassword(email, password);
  loginForm.reset();
  // Redirect to homepage
});
