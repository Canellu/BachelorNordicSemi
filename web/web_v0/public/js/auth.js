// Sign up method
const signUpForm = document.querySelector("#signUpForm");
if (signUpForm != null) {
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
}

//Sign in method
const loginForm = document.querySelector("#loginForm");
if (loginForm != null) {
  loginForm.addEventListener("submit", async (e) => {
    e.preventDefault();

    // get user info
    const email = loginForm["loginEmail"].value;
    const password = loginForm["loginPassword"].value;

    // log in user
    // const credentials = await auth.signInWithEmailAndPassword(email, password);
    try {
      const credentials = await auth.signInWithEmailAndPassword(
        email,
        password
      );
    } catch (error) {
      validateLogIn(error.message);
    }
  });
}

// Log out method
const logout = document.querySelector("#logout");
if (logout != null) {
  logout.addEventListener("click", async (e) => {
    await auth.signOut();
    // location.replace("index.html");
  });
}

// Check if user is logged in and redirect them
function userStateListener() {
  auth.onAuthStateChanged((user) => {
    if (user) {
      console.log("Is logged in!");
    } else {
      // User is logged out
      console.log("Is logged out!");
      location.replace("index.html");
    }
  });
}
