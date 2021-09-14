module.exports = {
  purge: [],
  darkMode: false, // or 'media' or 'class'
  theme: {
    extend: {
      fontFamily: {
        montserrat: ["Montserrat"],
      },
    },
  },
  variants: {
    extend: {
      backgroundColor: ["active"],
      ringColor: ["hover"],
      opacity: ["disabled"],
    },
  },
  plugins: [],
};
