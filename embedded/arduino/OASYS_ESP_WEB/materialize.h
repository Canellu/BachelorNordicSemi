const char web[] PROGMEM = R"====(
 <!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <!-- MATERIALIZE STYLES -->
    <style>
      .materialize-red {
        background-color: #e51c23 !important;
      }
      .materialize-red-text {
        color: #e51c23 !important;
      }
      .materialize-red.lighten-5 {
        background-color: #fdeaeb !important;
      }
      .materialize-red-text.text-lighten-5 {
        color: #fdeaeb !important;
      }
      .materialize-red.lighten-4 {
        background-color: #f8c1c3 !important;
      }
      .materialize-red-text.text-lighten-4 {
        color: #f8c1c3 !important;
      }
      .materialize-red.lighten-3 {
        background-color: #f3989b !important;
      }
      .materialize-red-text.text-lighten-3 {
        color: #f3989b !important;
      }
      .materialize-red.lighten-2 {
        background-color: #ee6e73 !important;
      }
      .materialize-red-text.text-lighten-2 {
        color: #ee6e73 !important;
      }
      .materialize-red.lighten-1 {
        background-color: #ea454b !important;
      }
      .materialize-red-text.text-lighten-1 {
        color: #ea454b !important;
      }
      .materialize-red.darken-1 {
        background-color: #d0181e !important;
      }
      .materialize-red-text.text-darken-1 {
        color: #d0181e !important;
      }
      .materialize-red.darken-2 {
        background-color: #b9151b !important;
      }
      .materialize-red-text.text-darken-2 {
        color: #b9151b !important;
      }
      .materialize-red.darken-3 {
        background-color: #a21318 !important;
      }
      .materialize-red-text.text-darken-3 {
        color: #a21318 !important;
      }
      .materialize-red.darken-4 {
        background-color: #8b1014 !important;
      }
      .materialize-red-text.text-darken-4 {
        color: #8b1014 !important;
      }
      .red {
        background-color: #f44336 !important;
      }
      .red-text {
        color: #f44336 !important;
      }
      .red.lighten-5 {
        background-color: #ffebee !important;
      }
      .red-text.text-lighten-5 {
        color: #ffebee !important;
      }
      .red.lighten-4 {
        background-color: #ffcdd2 !important;
      }
      .red-text.text-lighten-4 {
        color: #ffcdd2 !important;
      }
      .red.lighten-3 {
        background-color: #ef9a9a !important;
      }
      .red-text.text-lighten-3 {
        color: #ef9a9a !important;
      }
      .red.lighten-2 {
        background-color: #e57373 !important;
      }
      .red-text.text-lighten-2 {
        color: #e57373 !important;
      }
      .red.lighten-1 {
        background-color: #ef5350 !important;
      }
      .red-text.text-lighten-1 {
        color: #ef5350 !important;
      }
      .red.darken-1 {
        background-color: #e53935 !important;
      }
      .red-text.text-darken-1 {
        color: #e53935 !important;
      }
      .red.darken-2 {
        background-color: #d32f2f !important;
      }
      .red-text.text-darken-2 {
        color: #d32f2f !important;
      }
      .red.darken-3 {
        background-color: #c62828 !important;
      }
      .red-text.text-darken-3 {
        color: #c62828 !important;
      }
      .red.darken-4 {
        background-color: #b71c1c !important;
      }
      .red-text.text-darken-4 {
        color: #b71c1c !important;
      }
      .red.accent-1 {
        background-color: #ff8a80 !important;
      }
      .red-text.text-accent-1 {
        color: #ff8a80 !important;
      }
      .red.accent-2 {
        background-color: #ff5252 !important;
      }
      .red-text.text-accent-2 {
        color: #ff5252 !important;
      }
      .red.accent-3 {
        background-color: #ff1744 !important;
      }
      .red-text.text-accent-3 {
        color: #ff1744 !important;
      }
      .red.accent-4 {
        background-color: #d50000 !important;
      }
      .red-text.text-accent-4 {
        color: #d50000 !important;
      }
      .pink {
        background-color: #e91e63 !important;
      }
      .pink-text {
        color: #e91e63 !important;
      }
      .pink.lighten-5 {
        background-color: #fce4ec !important;
      }
      .pink-text.text-lighten-5 {
        color: #fce4ec !important;
      }
      .pink.lighten-4 {
        background-color: #f8bbd0 !important;
      }
      .pink-text.text-lighten-4 {
        color: #f8bbd0 !important;
      }
      .pink.lighten-3 {
        background-color: #f48fb1 !important;
      }
      .pink-text.text-lighten-3 {
        color: #f48fb1 !important;
      }
      .pink.lighten-2 {
        background-color: #f06292 !important;
      }
      .pink-text.text-lighten-2 {
        color: #f06292 !important;
      }
      .pink.lighten-1 {
        background-color: #ec407a !important;
      }
      .pink-text.text-lighten-1 {
        color: #ec407a !important;
      }
      .pink.darken-1 {
        background-color: #d81b60 !important;
      }
      .pink-text.text-darken-1 {
        color: #d81b60 !important;
      }
      .pink.darken-2 {
        background-color: #c2185b !important;
      }
      .pink-text.text-darken-2 {
        color: #c2185b !important;
      }
      .pink.darken-3 {
        background-color: #ad1457 !important;
      }
      .pink-text.text-darken-3 {
        color: #ad1457 !important;
      }
      .pink.darken-4 {
        background-color: #880e4f !important;
      }
      .pink-text.text-darken-4 {
        color: #880e4f !important;
      }
      .pink.accent-1 {
        background-color: #ff80ab !important;
      }
      .pink-text.text-accent-1 {
        color: #ff80ab !important;
      }
      .pink.accent-2 {
        background-color: #ff4081 !important;
      }
      .pink-text.text-accent-2 {
        color: #ff4081 !important;
      }
      .pink.accent-3 {
        background-color: #f50057 !important;
      }
      .pink-text.text-accent-3 {
        color: #f50057 !important;
      }
      .pink.accent-4 {
        background-color: #c51162 !important;
      }
      .pink-text.text-accent-4 {
        color: #c51162 !important;
      }
      .purple {
        background-color: #9c27b0 !important;
      }
      .purple-text {
        color: #9c27b0 !important;
      }
      .purple.lighten-5 {
        background-color: #f3e5f5 !important;
      }
      .purple-text.text-lighten-5 {
        color: #f3e5f5 !important;
      }
      .purple.lighten-4 {
        background-color: #e1bee7 !important;
      }
      .purple-text.text-lighten-4 {
        color: #e1bee7 !important;
      }
      .purple.lighten-3 {
        background-color: #ce93d8 !important;
      }
      .purple-text.text-lighten-3 {
        color: #ce93d8 !important;
      }
      .purple.lighten-2 {
        background-color: #ba68c8 !important;
      }
      .purple-text.text-lighten-2 {
        color: #ba68c8 !important;
      }
      .purple.lighten-1 {
        background-color: #ab47bc !important;
      }
      .purple-text.text-lighten-1 {
        color: #ab47bc !important;
      }
      .purple.darken-1 {
        background-color: #8e24aa !important;
      }
      .purple-text.text-darken-1 {
        color: #8e24aa !important;
      }
      .purple.darken-2 {
        background-color: #7b1fa2 !important;
      }
      .purple-text.text-darken-2 {
        color: #7b1fa2 !important;
      }
      .purple.darken-3 {
        background-color: #6a1b9a !important;
      }
      .purple-text.text-darken-3 {
        color: #6a1b9a !important;
      }
      .purple.darken-4 {
        background-color: #4a148c !important;
      }
      .purple-text.text-darken-4 {
        color: #4a148c !important;
      }
      .purple.accent-1 {
        background-color: #ea80fc !important;
      }
      .purple-text.text-accent-1 {
        color: #ea80fc !important;
      }
      .purple.accent-2 {
        background-color: #e040fb !important;
      }
      .purple-text.text-accent-2 {
        color: #e040fb !important;
      }
      .purple.accent-3 {
        background-color: #d500f9 !important;
      }
      .purple-text.text-accent-3 {
        color: #d500f9 !important;
      }
      .purple.accent-4 {
        background-color: #a0f !important;
      }
      .purple-text.text-accent-4 {
        color: #a0f !important;
      }
      .deep-purple {
        background-color: #673ab7 !important;
      }
      .deep-purple-text {
        color: #673ab7 !important;
      }
      .deep-purple.lighten-5 {
        background-color: #ede7f6 !important;
      }
      .deep-purple-text.text-lighten-5 {
        color: #ede7f6 !important;
      }
      .deep-purple.lighten-4 {
        background-color: #d1c4e9 !important;
      }
      .deep-purple-text.text-lighten-4 {
        color: #d1c4e9 !important;
      }
      .deep-purple.lighten-3 {
        background-color: #b39ddb !important;
      }
      .deep-purple-text.text-lighten-3 {
        color: #b39ddb !important;
      }
      .deep-purple.lighten-2 {
        background-color: #9575cd !important;
      }
      .deep-purple-text.text-lighten-2 {
        color: #9575cd !important;
      }
      .deep-purple.lighten-1 {
        background-color: #7e57c2 !important;
      }
      .deep-purple-text.text-lighten-1 {
        color: #7e57c2 !important;
      }
      .deep-purple.darken-1 {
        background-color: #5e35b1 !important;
      }
      .deep-purple-text.text-darken-1 {
        color: #5e35b1 !important;
      }
      .deep-purple.darken-2 {
        background-color: #512da8 !important;
      }
      .deep-purple-text.text-darken-2 {
        color: #512da8 !important;
      }
      .deep-purple.darken-3 {
        background-color: #4527a0 !important;
      }
      .deep-purple-text.text-darken-3 {
        color: #4527a0 !important;
      }
      .deep-purple.darken-4 {
        background-color: #311b92 !important;
      }
      .deep-purple-text.text-darken-4 {
        color: #311b92 !important;
      }
      .deep-purple.accent-1 {
        background-color: #b388ff !important;
      }
      .deep-purple-text.text-accent-1 {
        color: #b388ff !important;
      }
      .deep-purple.accent-2 {
        background-color: #7c4dff !important;
      }
      .deep-purple-text.text-accent-2 {
        color: #7c4dff !important;
      }
      .deep-purple.accent-3 {
        background-color: #651fff !important;
      }
      .deep-purple-text.text-accent-3 {
        color: #651fff !important;
      }
      .deep-purple.accent-4 {
        background-color: #6200ea !important;
      }
      .deep-purple-text.text-accent-4 {
        color: #6200ea !important;
      }
      .indigo {
        background-color: #3f51b5 !important;
      }
      .indigo-text {
        color: #3f51b5 !important;
      }
      .indigo.lighten-5 {
        background-color: #e8eaf6 !important;
      }
      .indigo-text.text-lighten-5 {
        color: #e8eaf6 !important;
      }
      .indigo.lighten-4 {
        background-color: #c5cae9 !important;
      }
      .indigo-text.text-lighten-4 {
        color: #c5cae9 !important;
      }
      .indigo.lighten-3 {
        background-color: #9fa8da !important;
      }
      .indigo-text.text-lighten-3 {
        color: #9fa8da !important;
      }
      .indigo.lighten-2 {
        background-color: #7986cb !important;
      }
      .indigo-text.text-lighten-2 {
        color: #7986cb !important;
      }
      .indigo.lighten-1 {
        background-color: #5c6bc0 !important;
      }
      .indigo-text.text-lighten-1 {
        color: #5c6bc0 !important;
      }
      .indigo.darken-1 {
        background-color: #3949ab !important;
      }
      .indigo-text.text-darken-1 {
        color: #3949ab !important;
      }
      .indigo.darken-2 {
        background-color: #303f9f !important;
      }
      .indigo-text.text-darken-2 {
        color: #303f9f !important;
      }
      .indigo.darken-3 {
        background-color: #283593 !important;
      }
      .indigo-text.text-darken-3 {
        color: #283593 !important;
      }
      .indigo.darken-4 {
        background-color: #1a237e !important;
      }
      .indigo-text.text-darken-4 {
        color: #1a237e !important;
      }
      .indigo.accent-1 {
        background-color: #8c9eff !important;
      }
      .indigo-text.text-accent-1 {
        color: #8c9eff !important;
      }
      .indigo.accent-2 {
        background-color: #536dfe !important;
      }
      .indigo-text.text-accent-2 {
        color: #536dfe !important;
      }
      .indigo.accent-3 {
        background-color: #3d5afe !important;
      }
      .indigo-text.text-accent-3 {
        color: #3d5afe !important;
      }
      .indigo.accent-4 {
        background-color: #304ffe !important;
      }
      .indigo-text.text-accent-4 {
        color: #304ffe !important;
      }
      .blue {
        background-color: #2196f3 !important;
      }
      .blue-text {
        color: #2196f3 !important;
      }
      .blue.lighten-5 {
        background-color: #e3f2fd !important;
      }
      .blue-text.text-lighten-5 {
        color: #e3f2fd !important;
      }
      .blue.lighten-4 {
        background-color: #bbdefb !important;
      }
      .blue-text.text-lighten-4 {
        color: #bbdefb !important;
      }
      .blue.lighten-3 {
        background-color: #90caf9 !important;
      }
      .blue-text.text-lighten-3 {
        color: #90caf9 !important;
      }
      .blue.lighten-2 {
        background-color: #64b5f6 !important;
      }
      .blue-text.text-lighten-2 {
        color: #64b5f6 !important;
      }
      .blue.lighten-1 {
        background-color: #42a5f5 !important;
      }
      .blue-text.text-lighten-1 {
        color: #42a5f5 !important;
      }
      .blue.darken-1 {
        background-color: #1e88e5 !important;
      }
      .blue-text.text-darken-1 {
        color: #1e88e5 !important;
      }
      .blue.darken-2 {
        background-color: #1976d2 !important;
      }
      .blue-text.text-darken-2 {
        color: #1976d2 !important;
      }
      .blue.darken-3 {
        background-color: #1565c0 !important;
      }
      .blue-text.text-darken-3 {
        color: #1565c0 !important;
      }
      .blue.darken-4 {
        background-color: #0d47a1 !important;
      }
      .blue-text.text-darken-4 {
        color: #0d47a1 !important;
      }
      .blue.accent-1 {
        background-color: #82b1ff !important;
      }
      .blue-text.text-accent-1 {
        color: #82b1ff !important;
      }
      .blue.accent-2 {
        background-color: #448aff !important;
      }
      .blue-text.text-accent-2 {
        color: #448aff !important;
      }
      .blue.accent-3 {
        background-color: #2979ff !important;
      }
      .blue-text.text-accent-3 {
        color: #2979ff !important;
      }
      .blue.accent-4 {
        background-color: #2962ff !important;
      }
      .blue-text.text-accent-4 {
        color: #2962ff !important;
      }
      .light-blue {
        background-color: #03a9f4 !important;
      }
      .light-blue-text {
        color: #03a9f4 !important;
      }
      .light-blue.lighten-5 {
        background-color: #e1f5fe !important;
      }
      .light-blue-text.text-lighten-5 {
        color: #e1f5fe !important;
      }
      .light-blue.lighten-4 {
        background-color: #b3e5fc !important;
      }
      .light-blue-text.text-lighten-4 {
        color: #b3e5fc !important;
      }
      .light-blue.lighten-3 {
        background-color: #81d4fa !important;
      }
      .light-blue-text.text-lighten-3 {
        color: #81d4fa !important;
      }
      .light-blue.lighten-2 {
        background-color: #4fc3f7 !important;
      }
      .light-blue-text.text-lighten-2 {
        color: #4fc3f7 !important;
      }
      .light-blue.lighten-1 {
        background-color: #29b6f6 !important;
      }
      .light-blue-text.text-lighten-1 {
        color: #29b6f6 !important;
      }
      .light-blue.darken-1 {
        background-color: #039be5 !important;
      }
      .light-blue-text.text-darken-1 {
        color: #039be5 !important;
      }
      .light-blue.darken-2 {
        background-color: #0288d1 !important;
      }
      .light-blue-text.text-darken-2 {
        color: #0288d1 !important;
      }
      .light-blue.darken-3 {
        background-color: #0277bd !important;
      }
      .light-blue-text.text-darken-3 {
        color: #0277bd !important;
      }
      .light-blue.darken-4 {
        background-color: #01579b !important;
      }
      .light-blue-text.text-darken-4 {
        color: #01579b !important;
      }
      .light-blue.accent-1 {
        background-color: #80d8ff !important;
      }
      .light-blue-text.text-accent-1 {
        color: #80d8ff !important;
      }
      .light-blue.accent-2 {
        background-color: #40c4ff !important;
      }
      .light-blue-text.text-accent-2 {
        color: #40c4ff !important;
      }
      .light-blue.accent-3 {
        background-color: #00b0ff !important;
      }
      .light-blue-text.text-accent-3 {
        color: #00b0ff !important;
      }
      .light-blue.accent-4 {
        background-color: #0091ea !important;
      }
      .light-blue-text.text-accent-4 {
        color: #0091ea !important;
      }
      .cyan {
        background-color: #00bcd4 !important;
      }
      .cyan-text {
        color: #00bcd4 !important;
      }
      .cyan.lighten-5 {
        background-color: #e0f7fa !important;
      }
      .cyan-text.text-lighten-5 {
        color: #e0f7fa !important;
      }
      .cyan.lighten-4 {
        background-color: #b2ebf2 !important;
      }
      .cyan-text.text-lighten-4 {
        color: #b2ebf2 !important;
      }
      .cyan.lighten-3 {
        background-color: #80deea !important;
      }
      .cyan-text.text-lighten-3 {
        color: #80deea !important;
      }
      .cyan.lighten-2 {
        background-color: #4dd0e1 !important;
      }
      .cyan-text.text-lighten-2 {
        color: #4dd0e1 !important;
      }
      .cyan.lighten-1 {
        background-color: #26c6da !important;
      }
      .cyan-text.text-lighten-1 {
        color: #26c6da !important;
      }
      .cyan.darken-1 {
        background-color: #00acc1 !important;
      }
      .cyan-text.text-darken-1 {
        color: #00acc1 !important;
      }
      .cyan.darken-2 {
        background-color: #0097a7 !important;
      }
      .cyan-text.text-darken-2 {
        color: #0097a7 !important;
      }
      .cyan.darken-3 {
        background-color: #00838f !important;
      }
      .cyan-text.text-darken-3 {
        color: #00838f !important;
      }
      .cyan.darken-4 {
        background-color: #006064 !important;
      }
      .cyan-text.text-darken-4 {
        color: #006064 !important;
      }
      .cyan.accent-1 {
        background-color: #84ffff !important;
      }
      .cyan-text.text-accent-1 {
        color: #84ffff !important;
      }
      .cyan.accent-2 {
        background-color: #18ffff !important;
      }
      .cyan-text.text-accent-2 {
        color: #18ffff !important;
      }
      .cyan.accent-3 {
        background-color: #00e5ff !important;
      }
      .cyan-text.text-accent-3 {
        color: #00e5ff !important;
      }
      .cyan.accent-4 {
        background-color: #00b8d4 !important;
      }
      .cyan-text.text-accent-4 {
        color: #00b8d4 !important;
      }
      .teal {
        background-color: #009688 !important;
      }
      .teal-text {
        color: #009688 !important;
      }
      .teal.lighten-5 {
        background-color: #e0f2f1 !important;
      }
      .teal-text.text-lighten-5 {
        color: #e0f2f1 !important;
      }
      .teal.lighten-4 {
        background-color: #b2dfdb !important;
      }
      .teal-text.text-lighten-4 {
        color: #b2dfdb !important;
      }
      .teal.lighten-3 {
        background-color: #80cbc4 !important;
      }
      .teal-text.text-lighten-3 {
        color: #80cbc4 !important;
      }
      .teal.lighten-2 {
        background-color: #4db6ac !important;
      }
      .teal-text.text-lighten-2 {
        color: #4db6ac !important;
      }
      .teal.lighten-1 {
        background-color: #26a69a !important;
      }
      .teal-text.text-lighten-1 {
        color: #26a69a !important;
      }
      .teal.darken-1 {
        background-color: #00897b !important;
      }
      .teal-text.text-darken-1 {
        color: #00897b !important;
      }
      .teal.darken-2 {
        background-color: #00796b !important;
      }
      .teal-text.text-darken-2 {
        color: #00796b !important;
      }
      .teal.darken-3 {
        background-color: #00695c !important;
      }
      .teal-text.text-darken-3 {
        color: #00695c !important;
      }
      .teal.darken-4 {
        background-color: #004d40 !important;
      }
      .teal-text.text-darken-4 {
        color: #004d40 !important;
      }
      .teal.accent-1 {
        background-color: #a7ffeb !important;
      }
      .teal-text.text-accent-1 {
        color: #a7ffeb !important;
      }
      .teal.accent-2 {
        background-color: #64ffda !important;
      }
      .teal-text.text-accent-2 {
        color: #64ffda !important;
      }
      .teal.accent-3 {
        background-color: #1de9b6 !important;
      }
      .teal-text.text-accent-3 {
        color: #1de9b6 !important;
      }
      .teal.accent-4 {
        background-color: #00bfa5 !important;
      }
      .teal-text.text-accent-4 {
        color: #00bfa5 !important;
      }
      .green {
        background-color: #4caf50 !important;
      }
      .green-text {
        color: #4caf50 !important;
      }
      .green.lighten-5 {
        background-color: #e8f5e9 !important;
      }
      .green-text.text-lighten-5 {
        color: #e8f5e9 !important;
      }
      .green.lighten-4 {
        background-color: #c8e6c9 !important;
      }
      .green-text.text-lighten-4 {
        color: #c8e6c9 !important;
      }
      .green.lighten-3 {
        background-color: #a5d6a7 !important;
      }
      .green-text.text-lighten-3 {
        color: #a5d6a7 !important;
      }
      .green.lighten-2 {
        background-color: #81c784 !important;
      }
      .green-text.text-lighten-2 {
        color: #81c784 !important;
      }
      .green.lighten-1 {
        background-color: #66bb6a !important;
      }
      .green-text.text-lighten-1 {
        color: #66bb6a !important;
      }
      .green.darken-1 {
        background-color: #43a047 !important;
      }
      .green-text.text-darken-1 {
        color: #43a047 !important;
      }
      .green.darken-2 {
        background-color: #388e3c !important;
      }
      .green-text.text-darken-2 {
        color: #388e3c !important;
      }
      .green.darken-3 {
        background-color: #2e7d32 !important;
      }
      .green-text.text-darken-3 {
        color: #2e7d32 !important;
      }
      .green.darken-4 {
        background-color: #1b5e20 !important;
      }
      .green-text.text-darken-4 {
        color: #1b5e20 !important;
      }
      .green.accent-1 {
        background-color: #b9f6ca !important;
      }
      .green-text.text-accent-1 {
        color: #b9f6ca !important;
      }
      .green.accent-2 {
        background-color: #69f0ae !important;
      }
      .green-text.text-accent-2 {
        color: #69f0ae !important;
      }
      .green.accent-3 {
        background-color: #00e676 !important;
      }
      .green-text.text-accent-3 {
        color: #00e676 !important;
      }
      .green.accent-4 {
        background-color: #00c853 !important;
      }
      .green-text.text-accent-4 {
        color: #00c853 !important;
      }
      .light-green {
        background-color: #8bc34a !important;
      }
      .light-green-text {
        color: #8bc34a !important;
      }
      .light-green.lighten-5 {
        background-color: #f1f8e9 !important;
      }
      .light-green-text.text-lighten-5 {
        color: #f1f8e9 !important;
      }
      .light-green.lighten-4 {
        background-color: #dcedc8 !important;
      }
      .light-green-text.text-lighten-4 {
        color: #dcedc8 !important;
      }
      .light-green.lighten-3 {
        background-color: #c5e1a5 !important;
      }
      .light-green-text.text-lighten-3 {
        color: #c5e1a5 !important;
      }
      .light-green.lighten-2 {
        background-color: #aed581 !important;
      }
      .light-green-text.text-lighten-2 {
        color: #aed581 !important;
      }
      .light-green.lighten-1 {
        background-color: #9ccc65 !important;
      }
      .light-green-text.text-lighten-1 {
        color: #9ccc65 !important;
      }
      .light-green.darken-1 {
        background-color: #7cb342 !important;
      }
      .light-green-text.text-darken-1 {
        color: #7cb342 !important;
      }
      .light-green.darken-2 {
        background-color: #689f38 !important;
      }
      .light-green-text.text-darken-2 {
        color: #689f38 !important;
      }
      .light-green.darken-3 {
        background-color: #558b2f !important;
      }
      .light-green-text.text-darken-3 {
        color: #558b2f !important;
      }
      .light-green.darken-4 {
        background-color: #33691e !important;
      }
      .light-green-text.text-darken-4 {
        color: #33691e !important;
      }
      .light-green.accent-1 {
        background-color: #ccff90 !important;
      }
      .light-green-text.text-accent-1 {
        color: #ccff90 !important;
      }
      .light-green.accent-2 {
        background-color: #b2ff59 !important;
      }
      .light-green-text.text-accent-2 {
        color: #b2ff59 !important;
      }
      .light-green.accent-3 {
        background-color: #76ff03 !important;
      }
      .light-green-text.text-accent-3 {
        color: #76ff03 !important;
      }
      .light-green.accent-4 {
        background-color: #64dd17 !important;
      }
      .light-green-text.text-accent-4 {
        color: #64dd17 !important;
      }
      .lime {
        background-color: #cddc39 !important;
      }
      .lime-text {
        color: #cddc39 !important;
      }
      .lime.lighten-5 {
        background-color: #f9fbe7 !important;
      }
      .lime-text.text-lighten-5 {
        color: #f9fbe7 !important;
      }
      .lime.lighten-4 {
        background-color: #f0f4c3 !important;
      }
      .lime-text.text-lighten-4 {
        color: #f0f4c3 !important;
      }
      .lime.lighten-3 {
        background-color: #e6ee9c !important;
      }
      .lime-text.text-lighten-3 {
        color: #e6ee9c !important;
      }
      .lime.lighten-2 {
        background-color: #dce775 !important;
      }
      .lime-text.text-lighten-2 {
        color: #dce775 !important;
      }
      .lime.lighten-1 {
        background-color: #d4e157 !important;
      }
      .lime-text.text-lighten-1 {
        color: #d4e157 !important;
      }
      .lime.darken-1 {
        background-color: #c0ca33 !important;
      }
      .lime-text.text-darken-1 {
        color: #c0ca33 !important;
      }
      .lime.darken-2 {
        background-color: #afb42b !important;
      }
      .lime-text.text-darken-2 {
        color: #afb42b !important;
      }
      .lime.darken-3 {
        background-color: #9e9d24 !important;
      }
      .lime-text.text-darken-3 {
        color: #9e9d24 !important;
      }
      .lime.darken-4 {
        background-color: #827717 !important;
      }
      .lime-text.text-darken-4 {
        color: #827717 !important;
      }
      .lime.accent-1 {
        background-color: #f4ff81 !important;
      }
      .lime-text.text-accent-1 {
        color: #f4ff81 !important;
      }
      .lime.accent-2 {
        background-color: #eeff41 !important;
      }
      .lime-text.text-accent-2 {
        color: #eeff41 !important;
      }
      .lime.accent-3 {
        background-color: #c6ff00 !important;
      }
      .lime-text.text-accent-3 {
        color: #c6ff00 !important;
      }
      .lime.accent-4 {
        background-color: #aeea00 !important;
      }
      .lime-text.text-accent-4 {
        color: #aeea00 !important;
      }
      .yellow {
        background-color: #ffeb3b !important;
      }
      .yellow-text {
        color: #ffeb3b !important;
      }
      .yellow.lighten-5 {
        background-color: #fffde7 !important;
      }
      .yellow-text.text-lighten-5 {
        color: #fffde7 !important;
      }
      .yellow.lighten-4 {
        background-color: #fff9c4 !important;
      }
      .yellow-text.text-lighten-4 {
        color: #fff9c4 !important;
      }
      .yellow.lighten-3 {
        background-color: #fff59d !important;
      }
      .yellow-text.text-lighten-3 {
        color: #fff59d !important;
      }
      .yellow.lighten-2 {
        background-color: #fff176 !important;
      }
      .yellow-text.text-lighten-2 {
        color: #fff176 !important;
      }
      .yellow.lighten-1 {
        background-color: #ffee58 !important;
      }
      .yellow-text.text-lighten-1 {
        color: #ffee58 !important;
      }
      .yellow.darken-1 {
        background-color: #fdd835 !important;
      }
      .yellow-text.text-darken-1 {
        color: #fdd835 !important;
      }
      .yellow.darken-2 {
        background-color: #fbc02d !important;
      }
      .yellow-text.text-darken-2 {
        color: #fbc02d !important;
      }
      .yellow.darken-3 {
        background-color: #f9a825 !important;
      }
      .yellow-text.text-darken-3 {
        color: #f9a825 !important;
      }
      .yellow.darken-4 {
        background-color: #f57f17 !important;
      }
      .yellow-text.text-darken-4 {
        color: #f57f17 !important;
      }
      .yellow.accent-1 {
        background-color: #ffff8d !important;
      }
      .yellow-text.text-accent-1 {
        color: #ffff8d !important;
      }
      .yellow.accent-2 {
        background-color: #ff0 !important;
      }
      .yellow-text.text-accent-2 {
        color: #ff0 !important;
      }
      .yellow.accent-3 {
        background-color: #ffea00 !important;
      }
      .yellow-text.text-accent-3 {
        color: #ffea00 !important;
      }
      .yellow.accent-4 {
        background-color: #ffd600 !important;
      }
      .yellow-text.text-accent-4 {
        color: #ffd600 !important;
      }
      .amber {
        background-color: #ffc107 !important;
      }
      .amber-text {
        color: #ffc107 !important;
      }
      .amber.lighten-5 {
        background-color: #fff8e1 !important;
      }
      .amber-text.text-lighten-5 {
        color: #fff8e1 !important;
      }
      .amber.lighten-4 {
        background-color: #ffecb3 !important;
      }
      .amber-text.text-lighten-4 {
        color: #ffecb3 !important;
      }
      .amber.lighten-3 {
        background-color: #ffe082 !important;
      }
      .amber-text.text-lighten-3 {
        color: #ffe082 !important;
      }
      .amber.lighten-2 {
        background-color: #ffd54f !important;
      }
      .amber-text.text-lighten-2 {
        color: #ffd54f !important;
      }
      .amber.lighten-1 {
        background-color: #ffca28 !important;
      }
      .amber-text.text-lighten-1 {
        color: #ffca28 !important;
      }
      .amber.darken-1 {
        background-color: #ffb300 !important;
      }
      .amber-text.text-darken-1 {
        color: #ffb300 !important;
      }
      .amber.darken-2 {
        background-color: #ffa000 !important;
      }
      .amber-text.text-darken-2 {
        color: #ffa000 !important;
      }
      .amber.darken-3 {
        background-color: #ff8f00 !important;
      }
      .amber-text.text-darken-3 {
        color: #ff8f00 !important;
      }
      .amber.darken-4 {
        background-color: #ff6f00 !important;
      }
      .amber-text.text-darken-4 {
        color: #ff6f00 !important;
      }
      .amber.accent-1 {
        background-color: #ffe57f !important;
      }
      .amber-text.text-accent-1 {
        color: #ffe57f !important;
      }
      .amber.accent-2 {
        background-color: #ffd740 !important;
      }
      .amber-text.text-accent-2 {
        color: #ffd740 !important;
      }
      .amber.accent-3 {
        background-color: #ffc400 !important;
      }
      .amber-text.text-accent-3 {
        color: #ffc400 !important;
      }
      .amber.accent-4 {
        background-color: #ffab00 !important;
      }
      .amber-text.text-accent-4 {
        color: #ffab00 !important;
      }
      .orange {
        background-color: #ff9800 !important;
      }
      .orange-text {
        color: #ff9800 !important;
      }
      .orange.lighten-5 {
        background-color: #fff3e0 !important;
      }
      .orange-text.text-lighten-5 {
        color: #fff3e0 !important;
      }
      .orange.lighten-4 {
        background-color: #ffe0b2 !important;
      }
      .orange-text.text-lighten-4 {
        color: #ffe0b2 !important;
      }
      .orange.lighten-3 {
        background-color: #ffcc80 !important;
      }
      .orange-text.text-lighten-3 {
        color: #ffcc80 !important;
      }
      .orange.lighten-2 {
        background-color: #ffb74d !important;
      }
      .orange-text.text-lighten-2 {
        color: #ffb74d !important;
      }
      .orange.lighten-1 {
        background-color: #ffa726 !important;
      }
      .orange-text.text-lighten-1 {
        color: #ffa726 !important;
      }
      .orange.darken-1 {
        background-color: #fb8c00 !important;
      }
      .orange-text.text-darken-1 {
        color: #fb8c00 !important;
      }
      .orange.darken-2 {
        background-color: #f57c00 !important;
      }
      .orange-text.text-darken-2 {
        color: #f57c00 !important;
      }
      .orange.darken-3 {
        background-color: #ef6c00 !important;
      }
      .orange-text.text-darken-3 {
        color: #ef6c00 !important;
      }
      .orange.darken-4 {
        background-color: #e65100 !important;
      }
      .orange-text.text-darken-4 {
        color: #e65100 !important;
      }
      .orange.accent-1 {
        background-color: #ffd180 !important;
      }
      .orange-text.text-accent-1 {
        color: #ffd180 !important;
      }
      .orange.accent-2 {
        background-color: #ffab40 !important;
      }
      .orange-text.text-accent-2 {
        color: #ffab40 !important;
      }
      .orange.accent-3 {
        background-color: #ff9100 !important;
      }
      .orange-text.text-accent-3 {
        color: #ff9100 !important;
      }
      .orange.accent-4 {
        background-color: #ff6d00 !important;
      }
      .orange-text.text-accent-4 {
        color: #ff6d00 !important;
      }
      .deep-orange {
        background-color: #ff5722 !important;
      }
      .deep-orange-text {
        color: #ff5722 !important;
      }
      .deep-orange.lighten-5 {
        background-color: #fbe9e7 !important;
      }
      .deep-orange-text.text-lighten-5 {
        color: #fbe9e7 !important;
      }
      .deep-orange.lighten-4 {
        background-color: #ffccbc !important;
      }
      .deep-orange-text.text-lighten-4 {
        color: #ffccbc !important;
      }
      .deep-orange.lighten-3 {
        background-color: #ffab91 !important;
      }
      .deep-orange-text.text-lighten-3 {
        color: #ffab91 !important;
      }
      .deep-orange.lighten-2 {
        background-color: #ff8a65 !important;
      }
      .deep-orange-text.text-lighten-2 {
        color: #ff8a65 !important;
      }
      .deep-orange.lighten-1 {
        background-color: #ff7043 !important;
      }
      .deep-orange-text.text-lighten-1 {
        color: #ff7043 !important;
      }
      .deep-orange.darken-1 {
        background-color: #f4511e !important;
      }
      .deep-orange-text.text-darken-1 {
        color: #f4511e !important;
      }
      .deep-orange.darken-2 {
        background-color: #e64a19 !important;
      }
      .deep-orange-text.text-darken-2 {
        color: #e64a19 !important;
      }
      .deep-orange.darken-3 {
        background-color: #d84315 !important;
      }
      .deep-orange-text.text-darken-3 {
        color: #d84315 !important;
      }
      .deep-orange.darken-4 {
        background-color: #bf360c !important;
      }
      .deep-orange-text.text-darken-4 {
        color: #bf360c !important;
      }
      .deep-orange.accent-1 {
        background-color: #ff9e80 !important;
      }
      .deep-orange-text.text-accent-1 {
        color: #ff9e80 !important;
      }
      .deep-orange.accent-2 {
        background-color: #ff6e40 !important;
      }
      .deep-orange-text.text-accent-2 {
        color: #ff6e40 !important;
      }
      .deep-orange.accent-3 {
        background-color: #ff3d00 !important;
      }
      .deep-orange-text.text-accent-3 {
        color: #ff3d00 !important;
      }
      .deep-orange.accent-4 {
        background-color: #dd2c00 !important;
      }
      .deep-orange-text.text-accent-4 {
        color: #dd2c00 !important;
      }
      .brown {
        background-color: #795548 !important;
      }
      .brown-text {
        color: #795548 !important;
      }
      .brown.lighten-5 {
        background-color: #efebe9 !important;
      }
      .brown-text.text-lighten-5 {
        color: #efebe9 !important;
      }
      .brown.lighten-4 {
        background-color: #d7ccc8 !important;
      }
      .brown-text.text-lighten-4 {
        color: #d7ccc8 !important;
      }
      .brown.lighten-3 {
        background-color: #bcaaa4 !important;
      }
      .brown-text.text-lighten-3 {
        color: #bcaaa4 !important;
      }
      .brown.lighten-2 {
        background-color: #a1887f !important;
      }
      .brown-text.text-lighten-2 {
        color: #a1887f !important;
      }
      .brown.lighten-1 {
        background-color: #8d6e63 !important;
      }
      .brown-text.text-lighten-1 {
        color: #8d6e63 !important;
      }
      .brown.darken-1 {
        background-color: #6d4c41 !important;
      }
      .brown-text.text-darken-1 {
        color: #6d4c41 !important;
      }
      .brown.darken-2 {
        background-color: #5d4037 !important;
      }
      .brown-text.text-darken-2 {
        color: #5d4037 !important;
      }
      .brown.darken-3 {
        background-color: #4e342e !important;
      }
      .brown-text.text-darken-3 {
        color: #4e342e !important;
      }
      .brown.darken-4 {
        background-color: #3e2723 !important;
      }
      .brown-text.text-darken-4 {
        color: #3e2723 !important;
      }
      .blue-grey {
        background-color: #607d8b !important;
      }
      .blue-grey-text {
        color: #607d8b !important;
      }
      .blue-grey.lighten-5 {
        background-color: #eceff1 !important;
      }
      .blue-grey-text.text-lighten-5 {
        color: #eceff1 !important;
      }
      .blue-grey.lighten-4 {
        background-color: #cfd8dc !important;
      }
      .blue-grey-text.text-lighten-4 {
        color: #cfd8dc !important;
      }
      .blue-grey.lighten-3 {
        background-color: #b0bec5 !important;
      }
      .blue-grey-text.text-lighten-3 {
        color: #b0bec5 !important;
      }
      .blue-grey.lighten-2 {
        background-color: #90a4ae !important;
      }
      .blue-grey-text.text-lighten-2 {
        color: #90a4ae !important;
      }
      .blue-grey.lighten-1 {
        background-color: #78909c !important;
      }
      .blue-grey-text.text-lighten-1 {
        color: #78909c !important;
      }
      .blue-grey.darken-1 {
        background-color: #546e7a !important;
      }
      .blue-grey-text.text-darken-1 {
        color: #546e7a !important;
      }
      .blue-grey.darken-2 {
        background-color: #455a64 !important;
      }
      .blue-grey-text.text-darken-2 {
        color: #455a64 !important;
      }
      .blue-grey.darken-3 {
        background-color: #37474f !important;
      }
      .blue-grey-text.text-darken-3 {
        color: #37474f !important;
      }
      .blue-grey.darken-4 {
        background-color: #263238 !important;
      }
      .blue-grey-text.text-darken-4 {
        color: #263238 !important;
      }
      .grey {
        background-color: #9e9e9e !important;
      }
      .grey-text {
        color: #9e9e9e !important;
      }
      .grey.lighten-5 {
        background-color: #fafafa !important;
      }
      .grey-text.text-lighten-5 {
        color: #fafafa !important;
      }
      .grey.lighten-4 {
        background-color: #f5f5f5 !important;
      }
      .grey-text.text-lighten-4 {
        color: #f5f5f5 !important;
      }
      .grey.lighten-3 {
        background-color: #eee !important;
      }
      .grey-text.text-lighten-3 {
        color: #eee !important;
      }
      .grey.lighten-2 {
        background-color: #e0e0e0 !important;
      }
      .grey-text.text-lighten-2 {
        color: #e0e0e0 !important;
      }
      .grey.lighten-1 {
        background-color: #bdbdbd !important;
      }
      .grey-text.text-lighten-1 {
        color: #bdbdbd !important;
      }
      .grey.darken-1 {
        background-color: #757575 !important;
      }
      .grey-text.text-darken-1 {
        color: #757575 !important;
      }
      .grey.darken-2 {
        background-color: #616161 !important;
      }
      .grey-text.text-darken-2 {
        color: #616161 !important;
      }
      .grey.darken-3 {
        background-color: #424242 !important;
      }
      .grey-text.text-darken-3 {
        color: #424242 !important;
      }
      .grey.darken-4 {
        background-color: #212121 !important;
      }
      .grey-text.text-darken-4 {
        color: #212121 !important;
      }
      .black {
        background-color: #000 !important;
      }
      .black-text {
        color: #000 !important;
      }
      .white {
        background-color: #fff !important;
      }
      .white-text {
        color: #fff !important;
      }
      .transparent {
        background-color: rgba(0, 0, 0, 0) !important;
      }
      .transparent-text {
        color: rgba(0, 0, 0, 0) !important;
      } /*! normalize.css v7.0.0 | MIT License | github.com/necolas/normalize.css */
      html {
        line-height: 1.15;
        -ms-text-size-adjust: 100%;
        -webkit-text-size-adjust: 100%;
      }
      body {
        margin: 0;
      }
      article,
      aside,
      footer,
      header,
      nav,
      section {
        display: block;
      }
      h1 {
        font-size: 2em;
        margin: 0.67em 0;
      }
      figcaption,
      figure,
      main {
        display: block;
      }
      figure {
        margin: 1em 40px;
      }
      hr {
        -webkit-box-sizing: content-box;
        box-sizing: content-box;
        height: 0;
        overflow: visible;
      }
      pre {
        font-family: monospace, monospace;
        font-size: 1em;
      }
      a {
        background-color: transparent;
        -webkit-text-decoration-skip: objects;
      }
      abbr[title] {
        border-bottom: none;
        text-decoration: underline;
        -webkit-text-decoration: underline dotted;
        -moz-text-decoration: underline dotted;
        text-decoration: underline dotted;
      }
      b,
      strong {
        font-weight: inherit;
      }
      b,
      strong {
        font-weight: bolder;
      }
      code,
      kbd,
      samp {
        font-family: monospace, monospace;
        font-size: 1em;
      }
      dfn {
        font-style: italic;
      }
      mark {
        background-color: #ff0;
        color: #000;
      }
      small {
        font-size: 80%;
      }
      sub,
      sup {
        font-size: 75%;
        line-height: 0;
        position: relative;
        vertical-align: baseline;
      }
      sub {
        bottom: -0.25em;
      }
      sup {
        top: -0.5em;
      }
      audio,
      video {
        display: inline-block;
      }
      audio:not([controls]) {
        display: none;
        height: 0;
      }
      img {
        border-style: none;
      }
      svg:not(:root) {
        overflow: hidden;
      }
      button,
      input,
      optgroup,
      select,
      textarea {
        font-family: sans-serif;
        font-size: 100%;
        line-height: 1.15;
        margin: 0;
      }
      button,
      input {
        overflow: visible;
      }
      button,
      select {
        text-transform: none;
      }
      button,
      html [type="button"],
      [type="reset"],
      [type="submit"] {
        -webkit-appearance: button;
      }
      button::-moz-focus-inner,
      [type="button"]::-moz-focus-inner,
      [type="reset"]::-moz-focus-inner,
      [type="submit"]::-moz-focus-inner {
        border-style: none;
        padding: 0;
      }
      button:-moz-focusring,
      [type="button"]:-moz-focusring,
      [type="reset"]:-moz-focusring,
      [type="submit"]:-moz-focusring {
        outline: 1px dotted ButtonText;
      }
      fieldset {
        padding: 0.35em 0.75em 0.625em;
      }
      legend {
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
        color: inherit;
        display: table;
        max-width: 100%;
        padding: 0;
        white-space: normal;
      }
      progress {
        display: inline-block;
        vertical-align: baseline;
      }
      textarea {
        overflow: auto;
      }
      [type="checkbox"],
      [type="radio"] {
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
        padding: 0;
      }
      [type="number"]::-webkit-inner-spin-button,
      [type="number"]::-webkit-outer-spin-button {
        height: auto;
      }
      [type="search"] {
        -webkit-appearance: textfield;
        outline-offset: -2px;
      }
      [type="search"]::-webkit-search-cancel-button,
      [type="search"]::-webkit-search-decoration {
        -webkit-appearance: none;
      }
      ::-webkit-file-upload-button {
        -webkit-appearance: button;
        font: inherit;
      }
      details,
      menu {
        display: block;
      }
      summary {
        display: list-item;
      }
      canvas {
        display: inline-block;
      }
      template {
        display: none;
      }
      [hidden] {
        display: none;
      }
      html {
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
      }
      *,
      *:before,
      *:after {
        -webkit-box-sizing: inherit;
        box-sizing: inherit;
      }
      button,
      input,
      optgroup,
      select,
      textarea {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
          Oxygen-Sans, Ubuntu, Cantarell, "Helvetica Neue", sans-serif;
      }
      ul:not(.browser-default) {
        padding-left: 0;
        list-style-type: none;
      }
      ul:not(.browser-default) > li {
        list-style-type: none;
      }
      a {
        color: #039be5;
        text-decoration: none;
        -webkit-tap-highlight-color: transparent;
      }
      .valign-wrapper {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-align: center;
        -webkit-align-items: center;
        -ms-flex-align: center;
        align-items: center;
      }
      .clearfix {
        clear: both;
      }
      .z-depth-0 {
        -webkit-box-shadow: none !important;
        box-shadow: none !important;
      }
      .z-depth-1,
      nav,
      .card-panel,
      .card,
      .toast,
      .btn,
      .btn-large,
      .btn-small,
      .btn-floating,
      .dropdown-content,
      .collapsible,
      .sidenav {
        -webkit-box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14),
          0 3px 1px -2px rgba(0, 0, 0, 0.12), 0 1px 5px 0 rgba(0, 0, 0, 0.2);
        box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14),
          0 3px 1px -2px rgba(0, 0, 0, 0.12), 0 1px 5px 0 rgba(0, 0, 0, 0.2);
      }
      .z-depth-1-half,
      .btn:hover,
      .btn-large:hover,
      .btn-small:hover,
      .btn-floating:hover {
        -webkit-box-shadow: 0 3px 3px 0 rgba(0, 0, 0, 0.14),
          0 1px 7px 0 rgba(0, 0, 0, 0.12), 0 3px 1px -1px rgba(0, 0, 0, 0.2);
        box-shadow: 0 3px 3px 0 rgba(0, 0, 0, 0.14),
          0 1px 7px 0 rgba(0, 0, 0, 0.12), 0 3px 1px -1px rgba(0, 0, 0, 0.2);
      }
      .z-depth-2 {
        -webkit-box-shadow: 0 4px 5px 0 rgba(0, 0, 0, 0.14),
          0 1px 10px 0 rgba(0, 0, 0, 0.12), 0 2px 4px -1px rgba(0, 0, 0, 0.3);
        box-shadow: 0 4px 5px 0 rgba(0, 0, 0, 0.14),
          0 1px 10px 0 rgba(0, 0, 0, 0.12), 0 2px 4px -1px rgba(0, 0, 0, 0.3);
      }
      .z-depth-3 {
        -webkit-box-shadow: 0 8px 17px 2px rgba(0, 0, 0, 0.14),
          0 3px 14px 2px rgba(0, 0, 0, 0.12), 0 5px 5px -3px rgba(0, 0, 0, 0.2);
        box-shadow: 0 8px 17px 2px rgba(0, 0, 0, 0.14),
          0 3px 14px 2px rgba(0, 0, 0, 0.12), 0 5px 5px -3px rgba(0, 0, 0, 0.2);
      }
      .z-depth-4 {
        -webkit-box-shadow: 0 16px 24px 2px rgba(0, 0, 0, 0.14),
          0 6px 30px 5px rgba(0, 0, 0, 0.12), 0 8px 10px -7px rgba(0, 0, 0, 0.2);
        box-shadow: 0 16px 24px 2px rgba(0, 0, 0, 0.14),
          0 6px 30px 5px rgba(0, 0, 0, 0.12), 0 8px 10px -7px rgba(0, 0, 0, 0.2);
      }
      .z-depth-5,
      .modal {
        -webkit-box-shadow: 0 24px 38px 3px rgba(0, 0, 0, 0.14),
          0 9px 46px 8px rgba(0, 0, 0, 0.12),
          0 11px 15px -7px rgba(0, 0, 0, 0.2);
        box-shadow: 0 24px 38px 3px rgba(0, 0, 0, 0.14),
          0 9px 46px 8px rgba(0, 0, 0, 0.12),
          0 11px 15px -7px rgba(0, 0, 0, 0.2);
      }
      .hoverable {
        -webkit-transition: -webkit-box-shadow 0.25s;
        transition: -webkit-box-shadow 0.25s;
        transition: box-shadow 0.25s;
        transition: box-shadow 0.25s, -webkit-box-shadow 0.25s;
      }
      .hoverable:hover {
        -webkit-box-shadow: 0 8px 17px 0 rgba(0, 0, 0, 0.2),
          0 6px 20px 0 rgba(0, 0, 0, 0.19);
        box-shadow: 0 8px 17px 0 rgba(0, 0, 0, 0.2),
          0 6px 20px 0 rgba(0, 0, 0, 0.19);
      }
      .divider {
        height: 1px;
        overflow: hidden;
        background-color: #e0e0e0;
      }
      blockquote {
        margin: 20px 0;
        padding-left: 1.5rem;
        border-left: 5px solid #ee6e73;
      }
      i {
        line-height: inherit;
      }
      i.left {
        float: left;
        margin-right: 15px;
      }
      i.right {
        float: right;
        margin-left: 15px;
      }
      i.tiny {
        font-size: 1rem;
      }
      i.small {
        font-size: 2rem;
      }
      i.medium {
        font-size: 4rem;
      }
      i.large {
        font-size: 6rem;
      }
      img.responsive-img,
      video.responsive-video {
        max-width: 100%;
        height: auto;
      }
      .pagination li {
        display: inline-block;
        border-radius: 2px;
        text-align: center;
        vertical-align: top;
        height: 30px;
      }
      .pagination li a {
        color: #444;
        display: inline-block;
        font-size: 1.2rem;
        padding: 0 10px;
        line-height: 30px;
      }
      .pagination li.active a {
        color: #fff;
      }
      .pagination li.active {
        background-color: #ee6e73;
      }
      .pagination li.disabled a {
        cursor: default;
        color: #999;
      }
      .pagination li i {
        font-size: 2rem;
      }
      .pagination li.pages ul li {
        display: inline-block;
        float: none;
      }
      @media only screen and (max-width: 992px) {
        .pagination {
          width: 100%;
        }
        .pagination li.prev,
        .pagination li.next {
          width: 10%;
        }
        .pagination li.pages {
          width: 80%;
          overflow: hidden;
          white-space: nowrap;
        }
      }
      .breadcrumb {
        font-size: 18px;
        color: rgba(255, 255, 255, 0.7);
      }
      .breadcrumb i,
      .breadcrumb [class^="mdi-"],
      .breadcrumb [class*="mdi-"],
      .breadcrumb i.material-icons {
        display: inline-block;
        float: left;
        font-size: 24px;
      }
      .breadcrumb:before {
        content: "\E5CC";
        color: rgba(255, 255, 255, 0.7);
        vertical-align: top;
        display: inline-block;
        font-family: "Material Icons";
        font-weight: normal;
        font-style: normal;
        font-size: 25px;
        margin: 0 10px 0 8px;
        -webkit-font-smoothing: antialiased;
      }
      .breadcrumb:first-child:before {
        display: none;
      }
      .breadcrumb:last-child {
        color: #fff;
      }
      .parallax-container {
        position: relative;
        overflow: hidden;
        height: 500px;
      }
      .parallax-container .parallax {
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        z-index: -1;
      }
      .parallax-container .parallax img {
        opacity: 0;
        position: absolute;
        left: 50%;
        bottom: 0;
        min-width: 100%;
        min-height: 100%;
        -webkit-transform: translate3d(0, 0, 0);
        transform: translate3d(0, 0, 0);
        -webkit-transform: translateX(-50%);
        transform: translateX(-50%);
      }
      .pin-top,
      .pin-bottom {
        position: relative;
      }
      .pinned {
        position: fixed !important;
      }
      ul.staggered-list li {
        opacity: 0;
      }
      .fade-in {
        opacity: 0;
        -webkit-transform-origin: 0 50%;
        transform-origin: 0 50%;
      }
      @media only screen and (max-width: 600px) {
        .hide-on-small-only,
        .hide-on-small-and-down {
          display: none !important;
        }
      }
      @media only screen and (max-width: 992px) {
        .hide-on-med-and-down {
          display: none !important;
        }
      }
      @media only screen and (min-width: 601px) {
        .hide-on-med-and-up {
          display: none !important;
        }
      }
      @media only screen and (min-width: 600px) and (max-width: 992px) {
        .hide-on-med-only {
          display: none !important;
        }
      }
      @media only screen and (min-width: 993px) {
        .hide-on-large-only {
          display: none !important;
        }
      }
      @media only screen and (min-width: 1201px) {
        .hide-on-extra-large-only {
          display: none !important;
        }
      }
      @media only screen and (min-width: 1201px) {
        .show-on-extra-large {
          display: block !important;
        }
      }
      @media only screen and (min-width: 993px) {
        .show-on-large {
          display: block !important;
        }
      }
      @media only screen and (min-width: 600px) and (max-width: 992px) {
        .show-on-medium {
          display: block !important;
        }
      }
      @media only screen and (max-width: 600px) {
        .show-on-small {
          display: block !important;
        }
      }
      @media only screen and (min-width: 601px) {
        .show-on-medium-and-up {
          display: block !important;
        }
      }
      @media only screen and (max-width: 992px) {
        .show-on-medium-and-down {
          display: block !important;
        }
      }
      @media only screen and (max-width: 600px) {
        .center-on-small-only {
          text-align: center;
        }
      }
      .page-footer {
        padding-top: 20px;
        color: #fff;
        background-color: #ee6e73;
      }
      .page-footer .footer-copyright {
        overflow: hidden;
        min-height: 50px;
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-align: center;
        -webkit-align-items: center;
        -ms-flex-align: center;
        align-items: center;
        -webkit-box-pack: justify;
        -webkit-justify-content: space-between;
        -ms-flex-pack: justify;
        justify-content: space-between;
        padding: 10px 0px;
        color: rgba(255, 255, 255, 0.8);
        background-color: rgba(51, 51, 51, 0.08);
      }
      table,
      th,
      td {
        border: none;
      }
      table {
        width: 100%;
        display: table;
        border-collapse: collapse;
        border-spacing: 0;
      }
      table.striped tr {
        border-bottom: none;
      }
      table.striped > tbody > tr:nth-child(odd) {
        background-color: rgba(242, 242, 242, 0.5);
      }
      table.striped > tbody > tr > td {
        border-radius: 0;
      }
      table.highlight > tbody > tr {
        -webkit-transition: background-color 0.25s ease;
        transition: background-color 0.25s ease;
      }
      table.highlight > tbody > tr:hover {
        background-color: rgba(242, 242, 242, 0.5);
      }
      table.centered thead tr th,
      table.centered tbody tr td {
        text-align: center;
      }
      tr {
        border-bottom: 1px solid rgba(0, 0, 0, 0.12);
      }
      td,
      th {
        padding: 15px 5px;
        display: table-cell;
        text-align: left;
        vertical-align: middle;
        border-radius: 2px;
      }
      @media only screen and (max-width: 992px) {
        table.responsive-table {
          width: 100%;
          border-collapse: collapse;
          border-spacing: 0;
          display: block;
          position: relative;
        }
        table.responsive-table td:empty:before {
          content: "\00a0";
        }
        table.responsive-table th,
        table.responsive-table td {
          margin: 0;
          vertical-align: top;
        }
        table.responsive-table th {
          text-align: left;
        }
        table.responsive-table thead {
          display: block;
          float: left;
        }
        table.responsive-table thead tr {
          display: block;
          padding: 0 10px 0 0;
        }
        table.responsive-table thead tr th::before {
          content: "\00a0";
        }
        table.responsive-table tbody {
          display: block;
          width: auto;
          position: relative;
          overflow-x: auto;
          white-space: nowrap;
        }
        table.responsive-table tbody tr {
          display: inline-block;
          vertical-align: top;
        }
        table.responsive-table th {
          display: block;
          text-align: right;
        }
        table.responsive-table td {
          display: block;
          min-height: 1.25em;
          text-align: left;
        }
        table.responsive-table tr {
          border-bottom: none;
          padding: 0 10px;
        }
        table.responsive-table thead {
          border: 0;
          border-right: 1px solid rgba(0, 0, 0, 0.12);
        }
      }
      .collection {
        margin: 0.5rem 0 1rem 0;
        border: 1px solid #e0e0e0;
        border-radius: 2px;
        overflow: hidden;
        position: relative;
      }
      .collection .collection-item {
        background-color: #fff;
        line-height: 1.5rem;
        padding: 10px 20px;
        margin: 0;
        border-bottom: 1px solid #e0e0e0;
      }
      .collection .collection-item.avatar {
        min-height: 84px;
        padding-left: 72px;
        position: relative;
      }
      .collection .collection-item.avatar:not(.circle-clipper) > .circle,
      .collection .collection-item.avatar :not(.circle-clipper) > .circle {
        position: absolute;
        width: 42px;
        height: 42px;
        overflow: hidden;
        left: 15px;
        display: inline-block;
        vertical-align: middle;
      }
      .collection .collection-item.avatar i.circle {
        font-size: 18px;
        line-height: 42px;
        color: #fff;
        background-color: #999;
        text-align: center;
      }
      .collection .collection-item.avatar .title {
        font-size: 16px;
      }
      .collection .collection-item.avatar p {
        margin: 0;
      }
      .collection .collection-item.avatar .secondary-content {
        position: absolute;
        top: 16px;
        right: 16px;
      }
      .collection .collection-item:last-child {
        border-bottom: none;
      }
      .collection .collection-item.active {
        background-color: #26a69a;
        color: #eafaf9;
      }
      .collection .collection-item.active .secondary-content {
        color: #fff;
      }
      .collection a.collection-item {
        display: block;
        -webkit-transition: 0.25s;
        transition: 0.25s;
        color: #26a69a;
      }
      .collection a.collection-item:not(.active):hover {
        background-color: #ddd;
      }
      .collection.with-header .collection-header {
        background-color: #fff;
        border-bottom: 1px solid #e0e0e0;
        padding: 10px 20px;
      }
      .collection.with-header .collection-item {
        padding-left: 30px;
      }
      .collection.with-header .collection-item.avatar {
        padding-left: 72px;
      }
      .secondary-content {
        float: right;
        color: #26a69a;
      }
      .collapsible .collection {
        margin: 0;
        border: none;
      }
      .video-container {
        position: relative;
        padding-bottom: 56.25%;
        height: 0;
        overflow: hidden;
      }
      .video-container iframe,
      .video-container object,
      .video-container embed {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
      }
      .progress {
        position: relative;
        height: 4px;
        display: block;
        width: 100%;
        background-color: #acece6;
        border-radius: 2px;
        margin: 0.5rem 0 1rem 0;
        overflow: hidden;
      }
      .progress .determinate {
        position: absolute;
        top: 0;
        left: 0;
        bottom: 0;
        background-color: #26a69a;
        -webkit-transition: width 0.3s linear;
        transition: width 0.3s linear;
      }
      .progress .indeterminate {
        background-color: #26a69a;
      }
      .progress .indeterminate:before {
        content: "";
        position: absolute;
        background-color: inherit;
        top: 0;
        left: 0;
        bottom: 0;
        will-change: left, right;
        -webkit-animation: indeterminate 2.1s
          cubic-bezier(0.65, 0.815, 0.735, 0.395) infinite;
        animation: indeterminate 2.1s cubic-bezier(0.65, 0.815, 0.735, 0.395)
          infinite;
      }
      .progress .indeterminate:after {
        content: "";
        position: absolute;
        background-color: inherit;
        top: 0;
        left: 0;
        bottom: 0;
        will-change: left, right;
        -webkit-animation: indeterminate-short 2.1s
          cubic-bezier(0.165, 0.84, 0.44, 1) infinite;
        animation: indeterminate-short 2.1s cubic-bezier(0.165, 0.84, 0.44, 1)
          infinite;
        -webkit-animation-delay: 1.15s;
        animation-delay: 1.15s;
      }
      @-webkit-keyframes indeterminate {
        0% {
          left: -35%;
          right: 100%;
        }
        60% {
          left: 100%;
          right: -90%;
        }
        100% {
          left: 100%;
          right: -90%;
        }
      }
      @keyframes indeterminate {
        0% {
          left: -35%;
          right: 100%;
        }
        60% {
          left: 100%;
          right: -90%;
        }
        100% {
          left: 100%;
          right: -90%;
        }
      }
      @-webkit-keyframes indeterminate-short {
        0% {
          left: -200%;
          right: 100%;
        }
        60% {
          left: 107%;
          right: -8%;
        }
        100% {
          left: 107%;
          right: -8%;
        }
      }
      @keyframes indeterminate-short {
        0% {
          left: -200%;
          right: 100%;
        }
        60% {
          left: 107%;
          right: -8%;
        }
        100% {
          left: 107%;
          right: -8%;
        }
      }
      .hide {
        display: none !important;
      }
      .left-align {
        text-align: left;
      }
      .right-align {
        text-align: right;
      }
      .center,
      .center-align {
        text-align: center;
      }
      .left {
        float: left !important;
      }
      .right {
        float: right !important;
      }
      .no-select,
      input[type="range"],
      input[type="range"] + .thumb {
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      .circle {
        border-radius: 50%;
      }
      .center-block {
        display: block;
        margin-left: auto;
        margin-right: auto;
      }
      .truncate {
        display: block;
        white-space: nowrap;
        overflow: hidden;
        text-overflow: ellipsis;
      }
      .no-padding {
        padding: 0 !important;
      }
      span.badge {
        min-width: 3rem;
        padding: 0 6px;
        margin-left: 14px;
        text-align: center;
        font-size: 1rem;
        line-height: 22px;
        height: 22px;
        color: #757575;
        float: right;
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
      }
      span.badge.new {
        font-weight: 300;
        font-size: 0.8rem;
        color: #fff;
        background-color: #26a69a;
        border-radius: 2px;
      }
      span.badge.new:after {
        content: " new";
      }
      span.badge[data-badge-caption]::after {
        content: " " attr(data-badge-caption);
      }
      nav ul a span.badge {
        display: inline-block;
        float: none;
        margin-left: 4px;
        line-height: 22px;
        height: 22px;
        -webkit-font-smoothing: auto;
      }
      .collection-item span.badge {
        margin-top: calc(0.75rem - 11px);
      }
      .collapsible span.badge {
        margin-left: auto;
      }
      .sidenav span.badge {
        margin-top: calc(24px - 11px);
      }
      table span.badge {
        display: inline-block;
        float: none;
        margin-left: auto;
      }
      .material-icons {
        text-rendering: optimizeLegibility;
        -webkit-font-feature-settings: "liga";
        -moz-font-feature-settings: "liga";
        font-feature-settings: "liga";
      }
      .container {
        margin: 0 auto;
        max-width: 1280px;
        width: 90%;
      }
      @media only screen and (min-width: 601px) {
        .container {
          width: 85%;
        }
      }
      @media only screen and (min-width: 993px) {
        .container {
          width: 70%;
        }
      }
      .col .row {
        margin-left: -0.75rem;
        margin-right: -0.75rem;
      }
      .section {
        padding-top: 1rem;
        padding-bottom: 1rem;
      }
      .section.no-pad {
        padding: 0;
      }
      .section.no-pad-bot {
        padding-bottom: 0;
      }
      .section.no-pad-top {
        padding-top: 0;
      }
      .row {
        margin-left: auto;
        margin-right: auto;
        margin-bottom: 20px;
      }
      .row:after {
        content: "";
        display: table;
        clear: both;
      }
      .row .col {
        float: left;
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
        padding: 0 0.75rem;
        min-height: 1px;
      }
      .row .col[class*="push-"],
      .row .col[class*="pull-"] {
        position: relative;
      }
      .row .col.s1 {
        width: 8.3333333333%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s2 {
        width: 16.6666666667%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s3 {
        width: 25%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s4 {
        width: 33.3333333333%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s5 {
        width: 41.6666666667%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s6 {
        width: 50%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s7 {
        width: 58.3333333333%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s8 {
        width: 66.6666666667%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s9 {
        width: 75%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s10 {
        width: 83.3333333333%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s11 {
        width: 91.6666666667%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.s12 {
        width: 100%;
        margin-left: auto;
        left: auto;
        right: auto;
      }
      .row .col.offset-s1 {
        margin-left: 8.3333333333%;
      }
      .row .col.pull-s1 {
        right: 8.3333333333%;
      }
      .row .col.push-s1 {
        left: 8.3333333333%;
      }
      .row .col.offset-s2 {
        margin-left: 16.6666666667%;
      }
      .row .col.pull-s2 {
        right: 16.6666666667%;
      }
      .row .col.push-s2 {
        left: 16.6666666667%;
      }
      .row .col.offset-s3 {
        margin-left: 25%;
      }
      .row .col.pull-s3 {
        right: 25%;
      }
      .row .col.push-s3 {
        left: 25%;
      }
      .row .col.offset-s4 {
        margin-left: 33.3333333333%;
      }
      .row .col.pull-s4 {
        right: 33.3333333333%;
      }
      .row .col.push-s4 {
        left: 33.3333333333%;
      }
      .row .col.offset-s5 {
        margin-left: 41.6666666667%;
      }
      .row .col.pull-s5 {
        right: 41.6666666667%;
      }
      .row .col.push-s5 {
        left: 41.6666666667%;
      }
      .row .col.offset-s6 {
        margin-left: 50%;
      }
      .row .col.pull-s6 {
        right: 50%;
      }
      .row .col.push-s6 {
        left: 50%;
      }
      .row .col.offset-s7 {
        margin-left: 58.3333333333%;
      }
      .row .col.pull-s7 {
        right: 58.3333333333%;
      }
      .row .col.push-s7 {
        left: 58.3333333333%;
      }
      .row .col.offset-s8 {
        margin-left: 66.6666666667%;
      }
      .row .col.pull-s8 {
        right: 66.6666666667%;
      }
      .row .col.push-s8 {
        left: 66.6666666667%;
      }
      .row .col.offset-s9 {
        margin-left: 75%;
      }
      .row .col.pull-s9 {
        right: 75%;
      }
      .row .col.push-s9 {
        left: 75%;
      }
      .row .col.offset-s10 {
        margin-left: 83.3333333333%;
      }
      .row .col.pull-s10 {
        right: 83.3333333333%;
      }
      .row .col.push-s10 {
        left: 83.3333333333%;
      }
      .row .col.offset-s11 {
        margin-left: 91.6666666667%;
      }
      .row .col.pull-s11 {
        right: 91.6666666667%;
      }
      .row .col.push-s11 {
        left: 91.6666666667%;
      }
      .row .col.offset-s12 {
        margin-left: 100%;
      }
      .row .col.pull-s12 {
        right: 100%;
      }
      .row .col.push-s12 {
        left: 100%;
      }
      @media only screen and (min-width: 601px) {
        .row .col.m1 {
          width: 8.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m2 {
          width: 16.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m3 {
          width: 25%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m4 {
          width: 33.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m5 {
          width: 41.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m6 {
          width: 50%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m7 {
          width: 58.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m8 {
          width: 66.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m9 {
          width: 75%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m10 {
          width: 83.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m11 {
          width: 91.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.m12 {
          width: 100%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.offset-m1 {
          margin-left: 8.3333333333%;
        }
        .row .col.pull-m1 {
          right: 8.3333333333%;
        }
        .row .col.push-m1 {
          left: 8.3333333333%;
        }
        .row .col.offset-m2 {
          margin-left: 16.6666666667%;
        }
        .row .col.pull-m2 {
          right: 16.6666666667%;
        }
        .row .col.push-m2 {
          left: 16.6666666667%;
        }
        .row .col.offset-m3 {
          margin-left: 25%;
        }
        .row .col.pull-m3 {
          right: 25%;
        }
        .row .col.push-m3 {
          left: 25%;
        }
        .row .col.offset-m4 {
          margin-left: 33.3333333333%;
        }
        .row .col.pull-m4 {
          right: 33.3333333333%;
        }
        .row .col.push-m4 {
          left: 33.3333333333%;
        }
        .row .col.offset-m5 {
          margin-left: 41.6666666667%;
        }
        .row .col.pull-m5 {
          right: 41.6666666667%;
        }
        .row .col.push-m5 {
          left: 41.6666666667%;
        }
        .row .col.offset-m6 {
          margin-left: 50%;
        }
        .row .col.pull-m6 {
          right: 50%;
        }
        .row .col.push-m6 {
          left: 50%;
        }
        .row .col.offset-m7 {
          margin-left: 58.3333333333%;
        }
        .row .col.pull-m7 {
          right: 58.3333333333%;
        }
        .row .col.push-m7 {
          left: 58.3333333333%;
        }
        .row .col.offset-m8 {
          margin-left: 66.6666666667%;
        }
        .row .col.pull-m8 {
          right: 66.6666666667%;
        }
        .row .col.push-m8 {
          left: 66.6666666667%;
        }
        .row .col.offset-m9 {
          margin-left: 75%;
        }
        .row .col.pull-m9 {
          right: 75%;
        }
        .row .col.push-m9 {
          left: 75%;
        }
        .row .col.offset-m10 {
          margin-left: 83.3333333333%;
        }
        .row .col.pull-m10 {
          right: 83.3333333333%;
        }
        .row .col.push-m10 {
          left: 83.3333333333%;
        }
        .row .col.offset-m11 {
          margin-left: 91.6666666667%;
        }
        .row .col.pull-m11 {
          right: 91.6666666667%;
        }
        .row .col.push-m11 {
          left: 91.6666666667%;
        }
        .row .col.offset-m12 {
          margin-left: 100%;
        }
        .row .col.pull-m12 {
          right: 100%;
        }
        .row .col.push-m12 {
          left: 100%;
        }
      }
      @media only screen and (min-width: 993px) {
        .row .col.l1 {
          width: 8.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l2 {
          width: 16.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l3 {
          width: 25%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l4 {
          width: 33.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l5 {
          width: 41.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l6 {
          width: 50%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l7 {
          width: 58.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l8 {
          width: 66.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l9 {
          width: 75%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l10 {
          width: 83.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l11 {
          width: 91.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.l12 {
          width: 100%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.offset-l1 {
          margin-left: 8.3333333333%;
        }
        .row .col.pull-l1 {
          right: 8.3333333333%;
        }
        .row .col.push-l1 {
          left: 8.3333333333%;
        }
        .row .col.offset-l2 {
          margin-left: 16.6666666667%;
        }
        .row .col.pull-l2 {
          right: 16.6666666667%;
        }
        .row .col.push-l2 {
          left: 16.6666666667%;
        }
        .row .col.offset-l3 {
          margin-left: 25%;
        }
        .row .col.pull-l3 {
          right: 25%;
        }
        .row .col.push-l3 {
          left: 25%;
        }
        .row .col.offset-l4 {
          margin-left: 33.3333333333%;
        }
        .row .col.pull-l4 {
          right: 33.3333333333%;
        }
        .row .col.push-l4 {
          left: 33.3333333333%;
        }
        .row .col.offset-l5 {
          margin-left: 41.6666666667%;
        }
        .row .col.pull-l5 {
          right: 41.6666666667%;
        }
        .row .col.push-l5 {
          left: 41.6666666667%;
        }
        .row .col.offset-l6 {
          margin-left: 50%;
        }
        .row .col.pull-l6 {
          right: 50%;
        }
        .row .col.push-l6 {
          left: 50%;
        }
        .row .col.offset-l7 {
          margin-left: 58.3333333333%;
        }
        .row .col.pull-l7 {
          right: 58.3333333333%;
        }
        .row .col.push-l7 {
          left: 58.3333333333%;
        }
        .row .col.offset-l8 {
          margin-left: 66.6666666667%;
        }
        .row .col.pull-l8 {
          right: 66.6666666667%;
        }
        .row .col.push-l8 {
          left: 66.6666666667%;
        }
        .row .col.offset-l9 {
          margin-left: 75%;
        }
        .row .col.pull-l9 {
          right: 75%;
        }
        .row .col.push-l9 {
          left: 75%;
        }
        .row .col.offset-l10 {
          margin-left: 83.3333333333%;
        }
        .row .col.pull-l10 {
          right: 83.3333333333%;
        }
        .row .col.push-l10 {
          left: 83.3333333333%;
        }
        .row .col.offset-l11 {
          margin-left: 91.6666666667%;
        }
        .row .col.pull-l11 {
          right: 91.6666666667%;
        }
        .row .col.push-l11 {
          left: 91.6666666667%;
        }
        .row .col.offset-l12 {
          margin-left: 100%;
        }
        .row .col.pull-l12 {
          right: 100%;
        }
        .row .col.push-l12 {
          left: 100%;
        }
      }
      @media only screen and (min-width: 1201px) {
        .row .col.xl1 {
          width: 8.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl2 {
          width: 16.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl3 {
          width: 25%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl4 {
          width: 33.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl5 {
          width: 41.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl6 {
          width: 50%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl7 {
          width: 58.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl8 {
          width: 66.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl9 {
          width: 75%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl10 {
          width: 83.3333333333%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl11 {
          width: 91.6666666667%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.xl12 {
          width: 100%;
          margin-left: auto;
          left: auto;
          right: auto;
        }
        .row .col.offset-xl1 {
          margin-left: 8.3333333333%;
        }
        .row .col.pull-xl1 {
          right: 8.3333333333%;
        }
        .row .col.push-xl1 {
          left: 8.3333333333%;
        }
        .row .col.offset-xl2 {
          margin-left: 16.6666666667%;
        }
        .row .col.pull-xl2 {
          right: 16.6666666667%;
        }
        .row .col.push-xl2 {
          left: 16.6666666667%;
        }
        .row .col.offset-xl3 {
          margin-left: 25%;
        }
        .row .col.pull-xl3 {
          right: 25%;
        }
        .row .col.push-xl3 {
          left: 25%;
        }
        .row .col.offset-xl4 {
          margin-left: 33.3333333333%;
        }
        .row .col.pull-xl4 {
          right: 33.3333333333%;
        }
        .row .col.push-xl4 {
          left: 33.3333333333%;
        }
        .row .col.offset-xl5 {
          margin-left: 41.6666666667%;
        }
        .row .col.pull-xl5 {
          right: 41.6666666667%;
        }
        .row .col.push-xl5 {
          left: 41.6666666667%;
        }
        .row .col.offset-xl6 {
          margin-left: 50%;
        }
        .row .col.pull-xl6 {
          right: 50%;
        }
        .row .col.push-xl6 {
          left: 50%;
        }
        .row .col.offset-xl7 {
          margin-left: 58.3333333333%;
        }
        .row .col.pull-xl7 {
          right: 58.3333333333%;
        }
        .row .col.push-xl7 {
          left: 58.3333333333%;
        }
        .row .col.offset-xl8 {
          margin-left: 66.6666666667%;
        }
        .row .col.pull-xl8 {
          right: 66.6666666667%;
        }
        .row .col.push-xl8 {
          left: 66.6666666667%;
        }
        .row .col.offset-xl9 {
          margin-left: 75%;
        }
        .row .col.pull-xl9 {
          right: 75%;
        }
        .row .col.push-xl9 {
          left: 75%;
        }
        .row .col.offset-xl10 {
          margin-left: 83.3333333333%;
        }
        .row .col.pull-xl10 {
          right: 83.3333333333%;
        }
        .row .col.push-xl10 {
          left: 83.3333333333%;
        }
        .row .col.offset-xl11 {
          margin-left: 91.6666666667%;
        }
        .row .col.pull-xl11 {
          right: 91.6666666667%;
        }
        .row .col.push-xl11 {
          left: 91.6666666667%;
        }
        .row .col.offset-xl12 {
          margin-left: 100%;
        }
        .row .col.pull-xl12 {
          right: 100%;
        }
        .row .col.push-xl12 {
          left: 100%;
        }
      }
      nav {
        color: #fff;
        background-color: #ee6e73;
        width: 100%;
        height: 56px;
        line-height: 56px;
      }
      nav.nav-extended {
        height: auto;
      }
      nav.nav-extended .nav-wrapper {
        min-height: 56px;
        height: auto;
      }
      nav.nav-extended .nav-content {
        position: relative;
        line-height: normal;
      }
      nav a {
        color: #fff;
      }
      nav i,
      nav [class^="mdi-"],
      nav [class*="mdi-"],
      nav i.material-icons {
        display: block;
        font-size: 24px;
        height: 56px;
        line-height: 56px;
      }
      nav .nav-wrapper {
        position: relative;
        height: 100%;
      }
      @media only screen and (min-width: 993px) {
        nav a.sidenav-trigger {
          display: none;
        }
      }
      nav .sidenav-trigger {
        float: left;
        position: relative;
        z-index: 1;
        height: 56px;
        margin: 0 18px;
      }
      nav .sidenav-trigger i {
        height: 56px;
        line-height: 56px;
      }
      nav .brand-logo {
        position: absolute;
        color: #fff;
        display: inline-block;
        font-size: 2.1rem;
        padding: 0;
      }
      nav .brand-logo.center {
        left: 50%;
        -webkit-transform: translateX(-50%);
        transform: translateX(-50%);
      }
      @media only screen and (max-width: 992px) {
        nav .brand-logo {
          left: 50%;
          -webkit-transform: translateX(-50%);
          transform: translateX(-50%);
        }
        nav .brand-logo.left,
        nav .brand-logo.right {
          padding: 0;
          -webkit-transform: none;
          transform: none;
        }
        nav .brand-logo.left {
          left: 0.5rem;
        }
        nav .brand-logo.right {
          right: 0.5rem;
          left: auto;
        }
      }
      nav .brand-logo.right {
        right: 0.5rem;
        padding: 0;
      }
      nav .brand-logo i,
      nav .brand-logo [class^="mdi-"],
      nav .brand-logo [class*="mdi-"],
      nav .brand-logo i.material-icons {
        float: left;
        margin-right: 15px;
      }
      nav .nav-title {
        display: inline-block;
        font-size: 32px;
        padding: 28px 0;
      }
      nav ul {
        margin: 0;
      }
      nav ul li {
        -webkit-transition: background-color 0.3s;
        transition: background-color 0.3s;
        float: left;
        padding: 0;
      }
      nav ul li.active {
        background-color: rgba(0, 0, 0, 0.1);
      }
      nav ul a {
        -webkit-transition: background-color 0.3s;
        transition: background-color 0.3s;
        font-size: 1rem;
        color: #fff;
        display: block;
        padding: 0 15px;
        cursor: pointer;
      }
      nav ul a.btn,
      nav ul a.btn-large,
      nav ul a.btn-small,
      nav ul a.btn-large,
      nav ul a.btn-flat,
      nav ul a.btn-floating {
        margin-top: -2px;
        margin-left: 15px;
        margin-right: 15px;
      }
      nav ul a.btn > .material-icons,
      nav ul a.btn-large > .material-icons,
      nav ul a.btn-small > .material-icons,
      nav ul a.btn-large > .material-icons,
      nav ul a.btn-flat > .material-icons,
      nav ul a.btn-floating > .material-icons {
        height: inherit;
        line-height: inherit;
      }
      nav ul a:hover {
        background-color: rgba(0, 0, 0, 0.1);
      }
      nav ul.left {
        float: left;
      }
      nav form {
        height: 100%;
      }
      nav .input-field {
        margin: 0;
        height: 100%;
      }
      nav .input-field input {
        height: 100%;
        font-size: 1.2rem;
        border: none;
        padding-left: 2rem;
      }
      nav .input-field input:focus,
      nav .input-field input[type="text"]:valid,
      nav .input-field input[type="password"]:valid,
      nav .input-field input[type="email"]:valid,
      nav .input-field input[type="url"]:valid,
      nav .input-field input[type="date"]:valid {
        border: none;
        -webkit-box-shadow: none;
        box-shadow: none;
      }
      nav .input-field label {
        top: 0;
        left: 0;
      }
      nav .input-field label i {
        color: rgba(255, 255, 255, 0.7);
        -webkit-transition: color 0.3s;
        transition: color 0.3s;
      }
      nav .input-field label.active i {
        color: #fff;
      }
      .navbar-fixed {
        position: relative;
        height: 56px;
        z-index: 997;
      }
      .navbar-fixed nav {
        position: fixed;
      }
      @media only screen and (min-width: 601px) {
        nav.nav-extended .nav-wrapper {
          min-height: 64px;
        }
        nav,
        nav .nav-wrapper i,
        nav a.sidenav-trigger,
        nav a.sidenav-trigger i {
          height: 64px;
          line-height: 64px;
        }
        .navbar-fixed {
          height: 64px;
        }
      }
      a {
        text-decoration: none;
      }
      html {
        line-height: 1.5;
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
          Oxygen-Sans, Ubuntu, Cantarell, "Helvetica Neue", sans-serif;
        font-weight: normal;
        color: rgba(0, 0, 0, 0.87);
      }
      @media only screen and (min-width: 0) {
        html {
          font-size: 14px;
        }
      }
      @media only screen and (min-width: 992px) {
        html {
          font-size: 14.5px;
        }
      }
      @media only screen and (min-width: 1200px) {
        html {
          font-size: 15px;
        }
      }
      h1,
      h2,
      h3,
      h4,
      h5,
      h6 {
        font-weight: 400;
        line-height: 1.3;
      }
      h1 a,
      h2 a,
      h3 a,
      h4 a,
      h5 a,
      h6 a {
        font-weight: inherit;
      }
      h1 {
        font-size: 4.2rem;
        line-height: 110%;
        margin: 2.8rem 0 1.68rem 0;
      }
      h2 {
        font-size: 3.56rem;
        line-height: 110%;
        margin: 2.3733333333rem 0 1.424rem 0;
      }
      h3 {
        font-size: 2.92rem;
        line-height: 110%;
        margin: 1.9466666667rem 0 1.168rem 0;
      }
      h4 {
        font-size: 2.28rem;
        line-height: 110%;
        margin: 1.52rem 0 0.912rem 0;
      }
      h5 {
        font-size: 1.64rem;
        line-height: 110%;
        margin: 1.0933333333rem 0 0.656rem 0;
      }
      h6 {
        font-size: 1.15rem;
        line-height: 110%;
        margin: 0.7666666667rem 0 0.46rem 0;
      }
      em {
        font-style: italic;
      }
      strong {
        font-weight: 500;
      }
      small {
        font-size: 75%;
      }
      .light {
        font-weight: 300;
      }
      .thin {
        font-weight: 200;
      }
      @media only screen and (min-width: 360px) {
        .flow-text {
          font-size: 1.2rem;
        }
      }
      @media only screen and (min-width: 390px) {
        .flow-text {
          font-size: 1.224rem;
        }
      }
      @media only screen and (min-width: 420px) {
        .flow-text {
          font-size: 1.248rem;
        }
      }
      @media only screen and (min-width: 450px) {
        .flow-text {
          font-size: 1.272rem;
        }
      }
      @media only screen and (min-width: 480px) {
        .flow-text {
          font-size: 1.296rem;
        }
      }
      @media only screen and (min-width: 510px) {
        .flow-text {
          font-size: 1.32rem;
        }
      }
      @media only screen and (min-width: 540px) {
        .flow-text {
          font-size: 1.344rem;
        }
      }
      @media only screen and (min-width: 570px) {
        .flow-text {
          font-size: 1.368rem;
        }
      }
      @media only screen and (min-width: 600px) {
        .flow-text {
          font-size: 1.392rem;
        }
      }
      @media only screen and (min-width: 630px) {
        .flow-text {
          font-size: 1.416rem;
        }
      }
      @media only screen and (min-width: 660px) {
        .flow-text {
          font-size: 1.44rem;
        }
      }
      @media only screen and (min-width: 690px) {
        .flow-text {
          font-size: 1.464rem;
        }
      }
      @media only screen and (min-width: 720px) {
        .flow-text {
          font-size: 1.488rem;
        }
      }
      @media only screen and (min-width: 750px) {
        .flow-text {
          font-size: 1.512rem;
        }
      }
      @media only screen and (min-width: 780px) {
        .flow-text {
          font-size: 1.536rem;
        }
      }
      @media only screen and (min-width: 810px) {
        .flow-text {
          font-size: 1.56rem;
        }
      }
      @media only screen and (min-width: 840px) {
        .flow-text {
          font-size: 1.584rem;
        }
      }
      @media only screen and (min-width: 870px) {
        .flow-text {
          font-size: 1.608rem;
        }
      }
      @media only screen and (min-width: 900px) {
        .flow-text {
          font-size: 1.632rem;
        }
      }
      @media only screen and (min-width: 930px) {
        .flow-text {
          font-size: 1.656rem;
        }
      }
      @media only screen and (min-width: 960px) {
        .flow-text {
          font-size: 1.68rem;
        }
      }
      @media only screen and (max-width: 360px) {
        .flow-text {
          font-size: 1.2rem;
        }
      }
      .scale-transition {
        -webkit-transition: -webkit-transform 0.3s
          cubic-bezier(0.53, 0.01, 0.36, 1.63) !important;
        transition: -webkit-transform 0.3s cubic-bezier(0.53, 0.01, 0.36, 1.63) !important;
        transition: transform 0.3s cubic-bezier(0.53, 0.01, 0.36, 1.63) !important;
        transition: transform 0.3s cubic-bezier(0.53, 0.01, 0.36, 1.63),
          -webkit-transform 0.3s cubic-bezier(0.53, 0.01, 0.36, 1.63) !important;
      }
      .scale-transition.scale-out {
        -webkit-transform: scale(0);
        transform: scale(0);
        -webkit-transition: -webkit-transform 0.2s !important;
        transition: -webkit-transform 0.2s !important;
        transition: transform 0.2s !important;
        transition: transform 0.2s, -webkit-transform 0.2s !important;
      }
      .scale-transition.scale-in {
        -webkit-transform: scale(1);
        transform: scale(1);
      }
      .card-panel {
        -webkit-transition: -webkit-box-shadow 0.25s;
        transition: -webkit-box-shadow 0.25s;
        transition: box-shadow 0.25s;
        transition: box-shadow 0.25s, -webkit-box-shadow 0.25s;
        padding: 24px;
        margin: 0.5rem 0 1rem 0;
        border-radius: 2px;
        background-color: #fff;
      }
      .card {
        position: relative;
        margin: 0.5rem 0 1rem 0;
        background-color: #fff;
        -webkit-transition: -webkit-box-shadow 0.25s;
        transition: -webkit-box-shadow 0.25s;
        transition: box-shadow 0.25s;
        transition: box-shadow 0.25s, -webkit-box-shadow 0.25s;
        border-radius: 2px;
      }
      .card .card-title {
        font-size: 24px;
        font-weight: 300;
      }
      .card .card-title.activator {
        cursor: pointer;
      }
      .card.small,
      .card.medium,
      .card.large {
        position: relative;
      }
      .card.small .card-image,
      .card.medium .card-image,
      .card.large .card-image {
        max-height: 60%;
        overflow: hidden;
      }
      .card.small .card-image + .card-content,
      .card.medium .card-image + .card-content,
      .card.large .card-image + .card-content {
        max-height: 40%;
      }
      .card.small .card-content,
      .card.medium .card-content,
      .card.large .card-content {
        max-height: 100%;
        overflow: hidden;
      }
      .card.small .card-action,
      .card.medium .card-action,
      .card.large .card-action {
        position: absolute;
        bottom: 0;
        left: 0;
        right: 0;
      }
      .card.small {
        height: 300px;
      }
      .card.medium {
        height: 400px;
      }
      .card.large {
        height: 500px;
      }
      .card.horizontal {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
      }
      .card.horizontal.small .card-image,
      .card.horizontal.medium .card-image,
      .card.horizontal.large .card-image {
        height: 100%;
        max-height: none;
        overflow: visible;
      }
      .card.horizontal.small .card-image img,
      .card.horizontal.medium .card-image img,
      .card.horizontal.large .card-image img {
        height: 100%;
      }
      .card.horizontal .card-image {
        max-width: 50%;
      }
      .card.horizontal .card-image img {
        border-radius: 2px 0 0 2px;
        max-width: 100%;
        width: auto;
      }
      .card.horizontal .card-stacked {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-orient: vertical;
        -webkit-box-direction: normal;
        -webkit-flex-direction: column;
        -ms-flex-direction: column;
        flex-direction: column;
        -webkit-box-flex: 1;
        -webkit-flex: 1;
        -ms-flex: 1;
        flex: 1;
        position: relative;
      }
      .card.horizontal .card-stacked .card-content {
        -webkit-box-flex: 1;
        -webkit-flex-grow: 1;
        -ms-flex-positive: 1;
        flex-grow: 1;
      }
      .card.sticky-action .card-action {
        z-index: 2;
      }
      .card.sticky-action .card-reveal {
        z-index: 1;
        padding-bottom: 64px;
      }
      .card .card-image {
        position: relative;
      }
      .card .card-image img {
        display: block;
        border-radius: 2px 2px 0 0;
        position: relative;
        left: 0;
        right: 0;
        top: 0;
        bottom: 0;
        width: 100%;
      }
      .card .card-image .card-title {
        color: #fff;
        position: absolute;
        bottom: 0;
        left: 0;
        max-width: 100%;
        padding: 24px;
      }
      .card .card-content {
        padding: 24px;
        border-radius: 0 0 2px 2px;
      }
      .card .card-content p {
        margin: 0;
      }
      .card .card-content .card-title {
        display: block;
        line-height: 32px;
        margin-bottom: 8px;
      }
      .card .card-content .card-title i {
        line-height: 32px;
      }
      .card .card-action {
        background-color: inherit;
        border-top: 1px solid rgba(160, 160, 160, 0.2);
        position: relative;
        padding: 16px 24px;
      }
      .card .card-action:last-child {
        border-radius: 0 0 2px 2px;
      }
      .card
        .card-action
        a:not(.btn):not(.btn-large):not(.btn-small):not(.btn-large):not(.btn-floating) {
        color: #ffab40;
        margin-right: 24px;
        -webkit-transition: color 0.3s ease;
        transition: color 0.3s ease;
        text-transform: uppercase;
      }
      .card
        .card-action
        a:not(.btn):not(.btn-large):not(.btn-small):not(.btn-large):not(.btn-floating):hover {
        color: #ffd8a6;
      }
      .card .card-reveal {
        padding: 24px;
        position: absolute;
        background-color: #fff;
        width: 100%;
        overflow-y: auto;
        left: 0;
        top: 100%;
        height: 100%;
        z-index: 3;
        display: none;
      }
      .card .card-reveal .card-title {
        cursor: pointer;
        display: block;
      }
      #toast-container {
        display: block;
        position: fixed;
        z-index: 10000;
      }
      @media only screen and (max-width: 600px) {
        #toast-container {
          min-width: 100%;
          bottom: 0%;
        }
      }
      @media only screen and (min-width: 601px) and (max-width: 992px) {
        #toast-container {
          left: 5%;
          bottom: 7%;
          max-width: 90%;
        }
      }
      @media only screen and (min-width: 993px) {
        #toast-container {
          top: 10%;
          right: 7%;
          max-width: 86%;
        }
      }
      .toast {
        border-radius: 2px;
        top: 35px;
        width: auto;
        margin-top: 10px;
        position: relative;
        max-width: 100%;
        height: auto;
        min-height: 48px;
        line-height: 1.5em;
        background-color: #323232;
        padding: 10px 25px;
        font-size: 1.1rem;
        font-weight: 300;
        color: #fff;
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-align: center;
        -webkit-align-items: center;
        -ms-flex-align: center;
        align-items: center;
        -webkit-box-pack: justify;
        -webkit-justify-content: space-between;
        -ms-flex-pack: justify;
        justify-content: space-between;
        cursor: default;
      }
      .toast .toast-action {
        color: #eeff41;
        font-weight: 500;
        margin-right: -25px;
        margin-left: 3rem;
      }
      .toast.rounded {
        border-radius: 24px;
      }
      @media only screen and (max-width: 600px) {
        .toast {
          width: 100%;
          border-radius: 0;
        }
      }
      .tabs {
        position: relative;
        overflow-x: auto;
        overflow-y: hidden;
        height: 48px;
        width: 100%;
        background-color: #fff;
        margin: 0 auto;
        white-space: nowrap;
      }
      .tabs.tabs-transparent {
        background-color: transparent;
      }
      .tabs.tabs-transparent .tab a,
      .tabs.tabs-transparent .tab.disabled a,
      .tabs.tabs-transparent .tab.disabled a:hover {
        color: rgba(255, 255, 255, 0.7);
      }
      .tabs.tabs-transparent .tab a:hover,
      .tabs.tabs-transparent .tab a.active {
        color: #fff;
      }
      .tabs.tabs-transparent .indicator {
        background-color: #fff;
      }
      .tabs.tabs-fixed-width {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
      }
      .tabs.tabs-fixed-width .tab {
        -webkit-box-flex: 1;
        -webkit-flex-grow: 1;
        -ms-flex-positive: 1;
        flex-grow: 1;
      }
      .tabs .tab {
        display: inline-block;
        text-align: center;
        line-height: 48px;
        height: 48px;
        padding: 0;
        margin: 0;
        text-transform: uppercase;
      }
      .tabs .tab a {
        color: rgba(238, 110, 115, 0.7);
        display: block;
        width: 100%;
        height: 100%;
        padding: 0 24px;
        font-size: 14px;
        text-overflow: ellipsis;
        overflow: hidden;
        -webkit-transition: color 0.28s ease, background-color 0.28s ease;
        transition: color 0.28s ease, background-color 0.28s ease;
      }
      .tabs .tab a:focus,
      .tabs .tab a:focus.active {
        background-color: rgba(246, 178, 181, 0.2);
        outline: none;
      }
      .tabs .tab a:hover,
      .tabs .tab a.active {
        background-color: transparent;
        color: #ee6e73;
      }
      .tabs .tab.disabled a,
      .tabs .tab.disabled a:hover {
        color: rgba(238, 110, 115, 0.4);
        cursor: default;
      }
      .tabs .indicator {
        position: absolute;
        bottom: 0;
        height: 2px;
        background-color: #f6b2b5;
        will-change: left, right;
      }
      @media only screen and (max-width: 992px) {
        .tabs {
          display: -webkit-box;
          display: -webkit-flex;
          display: -ms-flexbox;
          display: flex;
        }
        .tabs .tab {
          -webkit-box-flex: 1;
          -webkit-flex-grow: 1;
          -ms-flex-positive: 1;
          flex-grow: 1;
        }
        .tabs .tab a {
          padding: 0 12px;
        }
      }
      .material-tooltip {
        padding: 10px 8px;
        font-size: 1rem;
        z-index: 2000;
        background-color: transparent;
        border-radius: 2px;
        color: #fff;
        min-height: 36px;
        line-height: 120%;
        opacity: 0;
        position: absolute;
        text-align: center;
        max-width: calc(100% - 4px);
        overflow: hidden;
        left: 0;
        top: 0;
        pointer-events: none;
        visibility: hidden;
        background-color: #323232;
      }
      .backdrop {
        position: absolute;
        opacity: 0;
        height: 7px;
        width: 14px;
        border-radius: 0 0 50% 50%;
        background-color: #323232;
        z-index: -1;
        -webkit-transform-origin: 50% 0%;
        transform-origin: 50% 0%;
        visibility: hidden;
      }
      .btn,
      .btn-large,
      .btn-small,
      .btn-flat {
        border: none;
        border-radius: 2px;
        display: inline-block;
        height: 36px;
        line-height: 36px;
        padding: 0 16px;
        text-transform: uppercase;
        vertical-align: middle;
        -webkit-tap-highlight-color: transparent;
      }
      .btn.disabled,
      .disabled.btn-large,
      .disabled.btn-small,
      .btn-floating.disabled,
      .btn-large.disabled,
      .btn-small.disabled,
      .btn-flat.disabled,
      .btn:disabled,
      .btn-large:disabled,
      .btn-small:disabled,
      .btn-floating:disabled,
      .btn-large:disabled,
      .btn-small:disabled,
      .btn-flat:disabled,
      .btn[disabled],
      .btn-large[disabled],
      .btn-small[disabled],
      .btn-floating[disabled],
      .btn-large[disabled],
      .btn-small[disabled],
      .btn-flat[disabled] {
        pointer-events: none;
        background-color: #dfdfdf !important;
        -webkit-box-shadow: none;
        box-shadow: none;
        color: #9f9f9f !important;
        cursor: default;
      }
      .btn.disabled:hover,
      .disabled.btn-large:hover,
      .disabled.btn-small:hover,
      .btn-floating.disabled:hover,
      .btn-large.disabled:hover,
      .btn-small.disabled:hover,
      .btn-flat.disabled:hover,
      .btn:disabled:hover,
      .btn-large:disabled:hover,
      .btn-small:disabled:hover,
      .btn-floating:disabled:hover,
      .btn-large:disabled:hover,
      .btn-small:disabled:hover,
      .btn-flat:disabled:hover,
      .btn[disabled]:hover,
      .btn-large[disabled]:hover,
      .btn-small[disabled]:hover,
      .btn-floating[disabled]:hover,
      .btn-large[disabled]:hover,
      .btn-small[disabled]:hover,
      .btn-flat[disabled]:hover {
        background-color: #dfdfdf !important;
        color: #9f9f9f !important;
      }
      .btn,
      .btn-large,
      .btn-small,
      .btn-floating,
      .btn-large,
      .btn-small,
      .btn-flat {
        font-size: 14px;
        outline: 0;
      }
      .btn i,
      .btn-large i,
      .btn-small i,
      .btn-floating i,
      .btn-large i,
      .btn-small i,
      .btn-flat i {
        font-size: 1.3rem;
        line-height: inherit;
      }
      .btn:focus,
      .btn-large:focus,
      .btn-small:focus,
      .btn-floating:focus {
        background-color: #1d7d74;
      }
      .btn,
      .btn-large,
      .btn-small {
        text-decoration: none;
        color: #fff;
        background-color: #26a69a;
        text-align: center;
        letter-spacing: 0.5px;
        -webkit-transition: background-color 0.2s ease-out;
        transition: background-color 0.2s ease-out;
        cursor: pointer;
      }
      .btn:hover,
      .btn-large:hover,
      .btn-small:hover {
        background-color: #2bbbad;
      }
      .btn-floating {
        display: inline-block;
        color: #fff;
        position: relative;
        overflow: hidden;
        z-index: 1;
        width: 40px;
        height: 40px;
        line-height: 40px;
        padding: 0;
        background-color: #26a69a;
        border-radius: 50%;
        -webkit-transition: background-color 0.3s;
        transition: background-color 0.3s;
        cursor: pointer;
        vertical-align: middle;
      }
      .btn-floating:hover {
        background-color: #26a69a;
      }
      .btn-floating:before {
        border-radius: 0;
      }
      .btn-floating.btn-large {
        width: 56px;
        height: 56px;
        padding: 0;
      }
      .btn-floating.btn-large.halfway-fab {
        bottom: -28px;
      }
      .btn-floating.btn-large i {
        line-height: 56px;
      }
      .btn-floating.btn-small {
        width: 32.4px;
        height: 32.4px;
      }
      .btn-floating.btn-small.halfway-fab {
        bottom: -16.2px;
      }
      .btn-floating.btn-small i {
        line-height: 32.4px;
      }
      .btn-floating.halfway-fab {
        position: absolute;
        right: 24px;
        bottom: -20px;
      }
      .btn-floating.halfway-fab.left {
        right: auto;
        left: 24px;
      }
      .btn-floating i {
        width: inherit;
        display: inline-block;
        text-align: center;
        color: #fff;
        font-size: 1.6rem;
        line-height: 40px;
      }
      button.btn-floating {
        border: none;
      }
      .fixed-action-btn {
        position: fixed;
        right: 23px;
        bottom: 23px;
        padding-top: 15px;
        margin-bottom: 0;
        z-index: 997;
      }
      .fixed-action-btn.active ul {
        visibility: visible;
      }
      .fixed-action-btn.direction-left,
      .fixed-action-btn.direction-right {
        padding: 0 0 0 15px;
      }
      .fixed-action-btn.direction-left ul,
      .fixed-action-btn.direction-right ul {
        text-align: right;
        right: 64px;
        top: 50%;
        -webkit-transform: translateY(-50%);
        transform: translateY(-50%);
        height: 100%;
        left: auto;
        width: 500px;
      }
      .fixed-action-btn.direction-left ul li,
      .fixed-action-btn.direction-right ul li {
        display: inline-block;
        margin: 7.5px 15px 0 0;
      }
      .fixed-action-btn.direction-right {
        padding: 0 15px 0 0;
      }
      .fixed-action-btn.direction-right ul {
        text-align: left;
        direction: rtl;
        left: 64px;
        right: auto;
      }
      .fixed-action-btn.direction-right ul li {
        margin: 7.5px 0 0 15px;
      }
      .fixed-action-btn.direction-bottom {
        padding: 0 0 15px 0;
      }
      .fixed-action-btn.direction-bottom ul {
        top: 64px;
        bottom: auto;
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-orient: vertical;
        -webkit-box-direction: reverse;
        -webkit-flex-direction: column-reverse;
        -ms-flex-direction: column-reverse;
        flex-direction: column-reverse;
      }
      .fixed-action-btn.direction-bottom ul li {
        margin: 15px 0 0 0;
      }
      .fixed-action-btn.toolbar {
        padding: 0;
        height: 56px;
      }
      .fixed-action-btn.toolbar.active > a i {
        opacity: 0;
      }
      .fixed-action-btn.toolbar ul {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        top: 0;
        bottom: 0;
        z-index: 1;
      }
      .fixed-action-btn.toolbar ul li {
        -webkit-box-flex: 1;
        -webkit-flex: 1;
        -ms-flex: 1;
        flex: 1;
        display: inline-block;
        margin: 0;
        height: 100%;
        -webkit-transition: none;
        transition: none;
      }
      .fixed-action-btn.toolbar ul li a {
        display: block;
        overflow: hidden;
        position: relative;
        width: 100%;
        height: 100%;
        background-color: transparent;
        -webkit-box-shadow: none;
        box-shadow: none;
        color: #fff;
        line-height: 56px;
        z-index: 1;
      }
      .fixed-action-btn.toolbar ul li a i {
        line-height: inherit;
      }
      .fixed-action-btn ul {
        left: 0;
        right: 0;
        text-align: center;
        position: absolute;
        bottom: 64px;
        margin: 0;
        visibility: hidden;
      }
      .fixed-action-btn ul li {
        margin-bottom: 15px;
      }
      .fixed-action-btn ul a.btn-floating {
        opacity: 0;
      }
      .fixed-action-btn .fab-backdrop {
        position: absolute;
        top: 0;
        left: 0;
        z-index: -1;
        width: 40px;
        height: 40px;
        background-color: #26a69a;
        border-radius: 50%;
        -webkit-transform: scale(0);
        transform: scale(0);
      }
      .btn-flat {
        -webkit-box-shadow: none;
        box-shadow: none;
        background-color: transparent;
        color: #343434;
        cursor: pointer;
        -webkit-transition: background-color 0.2s;
        transition: background-color 0.2s;
      }
      .btn-flat:focus,
      .btn-flat:hover {
        -webkit-box-shadow: none;
        box-shadow: none;
      }
      .btn-flat:focus {
        background-color: rgba(0, 0, 0, 0.1);
      }
      .btn-flat.disabled,
      .btn-flat.btn-flat[disabled] {
        background-color: transparent !important;
        color: #b3b2b2 !important;
        cursor: default;
      }
      .btn-large {
        height: 54px;
        line-height: 54px;
        font-size: 15px;
        padding: 0 28px;
      }
      .btn-large i {
        font-size: 1.6rem;
      }
      .btn-small {
        height: 32.4px;
        line-height: 32.4px;
        font-size: 13px;
      }
      .btn-small i {
        font-size: 1.2rem;
      }
      .btn-block {
        display: block;
      }
      .dropdown-content {
        background-color: #fff;
        margin: 0;
        display: none;
        min-width: 100px;
        overflow-y: auto;
        opacity: 0;
        position: absolute;
        left: 0;
        top: 0;
        z-index: 9999;
        -webkit-transform-origin: 0 0;
        transform-origin: 0 0;
      }
      .dropdown-content:focus {
        outline: 0;
      }
      .dropdown-content li {
        clear: both;
        color: rgba(0, 0, 0, 0.87);
        cursor: pointer;
        min-height: 50px;
        line-height: 1.5rem;
        width: 100%;
        text-align: left;
      }
      .dropdown-content li:hover,
      .dropdown-content li.active {
        background-color: #eee;
      }
      .dropdown-content li:focus {
        outline: none;
      }
      .dropdown-content li.divider {
        min-height: 0;
        height: 1px;
      }
      .dropdown-content li > a,
      .dropdown-content li > span {
        font-size: 16px;
        color: #26a69a;
        display: block;
        line-height: 22px;
        padding: 14px 16px;
      }
      .dropdown-content li > span > label {
        top: 1px;
        left: 0;
        height: 18px;
      }
      .dropdown-content li > a > i {
        height: inherit;
        line-height: inherit;
        float: left;
        margin: 0 24px 0 0;
        width: 24px;
      }
      body.keyboard-focused .dropdown-content li:focus {
        background-color: #dadada;
      }
      .input-field.col .dropdown-content [type="checkbox"] + label {
        top: 1px;
        left: 0;
        height: 18px;
        -webkit-transform: none;
        transform: none;
      }
      .dropdown-trigger {
        cursor: pointer;
      } /*!
 * Waves v0.6.0
 * http://fian.my.id/Waves
 *
 * Copyright 2014 Alfiana E. Sibuea and other contributors
 * Released under the MIT license
 * https://github.com/fians/Waves/blob/master/LICENSE
 */
      .waves-effect {
        position: relative;
        cursor: pointer;
        display: inline-block;
        overflow: hidden;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: transparent;
        vertical-align: middle;
        z-index: 1;
        -webkit-transition: 0.3s ease-out;
        transition: 0.3s ease-out;
      }
      .waves-effect .waves-ripple {
        position: absolute;
        border-radius: 50%;
        width: 20px;
        height: 20px;
        margin-top: -10px;
        margin-left: -10px;
        opacity: 0;
        background: rgba(0, 0, 0, 0.2);
        -webkit-transition: all 0.7s ease-out;
        transition: all 0.7s ease-out;
        -webkit-transition-property: opacity, -webkit-transform;
        transition-property: opacity, -webkit-transform;
        transition-property: transform, opacity;
        transition-property: transform, opacity, -webkit-transform;
        -webkit-transform: scale(0);
        transform: scale(0);
        pointer-events: none;
      }
      .waves-effect.waves-light .waves-ripple {
        background-color: rgba(255, 255, 255, 0.45);
      }
      .waves-effect.waves-red .waves-ripple {
        background-color: rgba(244, 67, 54, 0.7);
      }
      .waves-effect.waves-yellow .waves-ripple {
        background-color: rgba(255, 235, 59, 0.7);
      }
      .waves-effect.waves-orange .waves-ripple {
        background-color: rgba(255, 152, 0, 0.7);
      }
      .waves-effect.waves-purple .waves-ripple {
        background-color: rgba(156, 39, 176, 0.7);
      }
      .waves-effect.waves-green .waves-ripple {
        background-color: rgba(76, 175, 80, 0.7);
      }
      .waves-effect.waves-teal .waves-ripple {
        background-color: rgba(0, 150, 136, 0.7);
      }
      .waves-effect input[type="button"],
      .waves-effect input[type="reset"],
      .waves-effect input[type="submit"] {
        border: 0;
        font-style: normal;
        font-size: inherit;
        text-transform: inherit;
        background: none;
      }
      .waves-effect img {
        position: relative;
        z-index: -1;
      }
      .waves-notransition {
        -webkit-transition: none !important;
        transition: none !important;
      }
      .waves-circle {
        -webkit-transform: translateZ(0);
        transform: translateZ(0);
        -webkit-mask-image: -webkit-radial-gradient(
          circle,
          white 100%,
          black 100%
        );
      }
      .waves-input-wrapper {
        border-radius: 0.2em;
        vertical-align: bottom;
      }
      .waves-input-wrapper .waves-button-input {
        position: relative;
        top: 0;
        left: 0;
        z-index: 1;
      }
      .waves-circle {
        text-align: center;
        width: 2.5em;
        height: 2.5em;
        line-height: 2.5em;
        border-radius: 50%;
        -webkit-mask-image: none;
      }
      .waves-block {
        display: block;
      }
      .waves-effect .waves-ripple {
        z-index: -1;
      }
      .modal {
        display: none;
        position: fixed;
        left: 0;
        right: 0;
        background-color: #fafafa;
        padding: 0;
        max-height: 70%;
        width: 55%;
        margin: auto;
        overflow-y: auto;
        border-radius: 2px;
        will-change: top, opacity;
      }
      .modal:focus {
        outline: none;
      }
      @media only screen and (max-width: 992px) {
        .modal {
          width: 80%;
        }
      }
      .modal h1,
      .modal h2,
      .modal h3,
      .modal h4 {
        margin-top: 0;
      }
      .modal .modal-content {
        padding: 24px;
      }
      .modal .modal-close {
        cursor: pointer;
      }
      .modal .modal-footer {
        border-radius: 0 0 2px 2px;
        background-color: #fafafa;
        padding: 4px 6px;
        height: 56px;
        width: 100%;
        text-align: right;
      }
      .modal .modal-footer .btn,
      .modal .modal-footer .btn-large,
      .modal .modal-footer .btn-small,
      .modal .modal-footer .btn-flat {
        margin: 6px 0;
      }
      .modal-overlay {
        position: fixed;
        z-index: 999;
        top: -25%;
        left: 0;
        bottom: 0;
        right: 0;
        height: 125%;
        width: 100%;
        background: #000;
        display: none;
        will-change: opacity;
      }
      .modal.modal-fixed-footer {
        padding: 0;
        height: 70%;
      }
      .modal.modal-fixed-footer .modal-content {
        position: absolute;
        height: calc(100% - 56px);
        max-height: 100%;
        width: 100%;
        overflow-y: auto;
      }
      .modal.modal-fixed-footer .modal-footer {
        border-top: 1px solid rgba(0, 0, 0, 0.1);
        position: absolute;
        bottom: 0;
      }
      .modal.bottom-sheet {
        top: auto;
        bottom: -100%;
        margin: 0;
        width: 100%;
        max-height: 45%;
        border-radius: 0;
        will-change: bottom, opacity;
      }
      .collapsible {
        border-top: 1px solid #ddd;
        border-right: 1px solid #ddd;
        border-left: 1px solid #ddd;
        margin: 0.5rem 0 1rem 0;
      }
      .collapsible-header {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        cursor: pointer;
        -webkit-tap-highlight-color: transparent;
        line-height: 1.5;
        padding: 1rem;
        background-color: #fff;
        border-bottom: 1px solid #ddd;
      }
      .collapsible-header:focus {
        outline: 0;
      }
      .collapsible-header i {
        width: 2rem;
        font-size: 1.6rem;
        display: inline-block;
        text-align: center;
        margin-right: 1rem;
      }
      .keyboard-focused .collapsible-header:focus {
        background-color: #eee;
      }
      .collapsible-body {
        display: none;
        border-bottom: 1px solid #ddd;
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
        padding: 2rem;
      }
      .sidenav .collapsible,
      .sidenav.fixed .collapsible {
        border: none;
        -webkit-box-shadow: none;
        box-shadow: none;
      }
      .sidenav .collapsible li,
      .sidenav.fixed .collapsible li {
        padding: 0;
      }
      .sidenav .collapsible-header,
      .sidenav.fixed .collapsible-header {
        background-color: transparent;
        border: none;
        line-height: inherit;
        height: inherit;
        padding: 0 16px;
      }
      .sidenav .collapsible-header:hover,
      .sidenav.fixed .collapsible-header:hover {
        background-color: rgba(0, 0, 0, 0.05);
      }
      .sidenav .collapsible-header i,
      .sidenav.fixed .collapsible-header i {
        line-height: inherit;
      }
      .sidenav .collapsible-body,
      .sidenav.fixed .collapsible-body {
        border: 0;
        background-color: #fff;
      }
      .sidenav .collapsible-body li a,
      .sidenav.fixed .collapsible-body li a {
        padding: 0 23.5px 0 31px;
      }
      .collapsible.popout {
        border: none;
        -webkit-box-shadow: none;
        box-shadow: none;
      }
      .collapsible.popout > li {
        -webkit-box-shadow: 0 2px 5px 0 rgba(0, 0, 0, 0.16),
          0 2px 10px 0 rgba(0, 0, 0, 0.12);
        box-shadow: 0 2px 5px 0 rgba(0, 0, 0, 0.16),
          0 2px 10px 0 rgba(0, 0, 0, 0.12);
        margin: 0 24px;
        -webkit-transition: margin 0.35s cubic-bezier(0.25, 0.46, 0.45, 0.94);
        transition: margin 0.35s cubic-bezier(0.25, 0.46, 0.45, 0.94);
      }
      .collapsible.popout > li.active {
        -webkit-box-shadow: 0 5px 11px 0 rgba(0, 0, 0, 0.18),
          0 4px 15px 0 rgba(0, 0, 0, 0.15);
        box-shadow: 0 5px 11px 0 rgba(0, 0, 0, 0.18),
          0 4px 15px 0 rgba(0, 0, 0, 0.15);
        margin: 16px 0;
      }
      .chip {
        display: inline-block;
        height: 32px;
        font-size: 13px;
        font-weight: 500;
        color: rgba(0, 0, 0, 0.6);
        line-height: 32px;
        padding: 0 12px;
        border-radius: 16px;
        background-color: #e4e4e4;
        margin-bottom: 5px;
        margin-right: 5px;
      }
      .chip:focus {
        outline: none;
        background-color: #26a69a;
        color: #fff;
      }
      .chip > img {
        float: left;
        margin: 0 8px 0 -12px;
        height: 32px;
        width: 32px;
        border-radius: 50%;
      }
      .chip .close {
        cursor: pointer;
        float: right;
        font-size: 16px;
        line-height: 32px;
        padding-left: 8px;
      }
      .chips {
        border: none;
        border-bottom: 1px solid #9e9e9e;
        -webkit-box-shadow: none;
        box-shadow: none;
        margin: 0 0 8px 0;
        min-height: 45px;
        outline: none;
        -webkit-transition: all 0.3s;
        transition: all 0.3s;
      }
      .chips.focus {
        border-bottom: 1px solid #26a69a;
        -webkit-box-shadow: 0 1px 0 0 #26a69a;
        box-shadow: 0 1px 0 0 #26a69a;
      }
      .chips:hover {
        cursor: text;
      }
      .chips .input {
        background: none;
        border: 0;
        color: rgba(0, 0, 0, 0.6);
        display: inline-block;
        font-size: 16px;
        height: 3rem;
        line-height: 32px;
        outline: 0;
        margin: 0;
        padding: 0 !important;
        width: 120px !important;
      }
      .chips .input:focus {
        border: 0 !important;
        -webkit-box-shadow: none !important;
        box-shadow: none !important;
      }
      .chips .autocomplete-content {
        margin-top: 0;
        margin-bottom: 0;
      }
      .prefix ~ .chips {
        margin-left: 3rem;
        width: 92%;
        width: calc(100% - 3rem);
      }
      .chips:empty ~ label {
        font-size: 0.8rem;
        -webkit-transform: translateY(-140%);
        transform: translateY(-140%);
      }
      .materialboxed {
        display: block;
        cursor: -webkit-zoom-in;
        cursor: zoom-in;
        position: relative;
        -webkit-transition: opacity 0.4s;
        transition: opacity 0.4s;
        -webkit-backface-visibility: hidden;
      }
      .materialboxed:hover:not(.active) {
        opacity: 0.8;
      }
      .materialboxed.active {
        cursor: -webkit-zoom-out;
        cursor: zoom-out;
      }
      #materialbox-overlay {
        position: fixed;
        top: 0;
        right: 0;
        bottom: 0;
        left: 0;
        background-color: #292929;
        z-index: 1000;
        will-change: opacity;
      }
      .materialbox-caption {
        position: fixed;
        display: none;
        color: #fff;
        line-height: 50px;
        bottom: 0;
        left: 0;
        width: 100%;
        text-align: center;
        padding: 0% 15%;
        height: 50px;
        z-index: 1000;
        -webkit-font-smoothing: antialiased;
      }
      select:focus {
        outline: 1px solid #c9f3ef;
      }
      button:focus {
        outline: none;
        background-color: #2ab7a9;
      }
      label {
        font-size: 0.8rem;
        color: #9e9e9e;
      }
      ::-webkit-input-placeholder {
        color: #d1d1d1;
      }
      ::-moz-placeholder {
        color: #d1d1d1;
      }
      :-ms-input-placeholder {
        color: #d1d1d1;
      }
      ::-ms-input-placeholder {
        color: #d1d1d1;
      }
      ::placeholder {
        color: #d1d1d1;
      }
      input:not([type]),
      input[type="text"]:not(.browser-default),
      input[type="password"]:not(.browser-default),
      input[type="email"]:not(.browser-default),
      input[type="url"]:not(.browser-default),
      input[type="time"]:not(.browser-default),
      input[type="date"]:not(.browser-default),
      input[type="datetime"]:not(.browser-default),
      input[type="datetime-local"]:not(.browser-default),
      input[type="tel"]:not(.browser-default),
      input[type="number"]:not(.browser-default),
      input[type="search"]:not(.browser-default),
      textarea.materialize-textarea {
        background-color: transparent;
        border: none;
        border-bottom: 1px solid #9e9e9e;
        border-radius: 0;
        outline: none;
        height: 3rem;
        width: 100%;
        font-size: 16px;
        margin: 0 0 8px 0;
        padding: 0;
        -webkit-box-shadow: none;
        box-shadow: none;
        -webkit-box-sizing: content-box;
        box-sizing: content-box;
        -webkit-transition: border 0.3s, -webkit-box-shadow 0.3s;
        transition: border 0.3s, -webkit-box-shadow 0.3s;
        transition: box-shadow 0.3s, border 0.3s;
        transition: box-shadow 0.3s, border 0.3s, -webkit-box-shadow 0.3s;
      }
      input:not([type]):disabled,
      input:not([type])[readonly="readonly"],
      input[type="text"]:not(.browser-default):disabled,
      input[type="text"]:not(.browser-default)[readonly="readonly"],
      input[type="password"]:not(.browser-default):disabled,
      input[type="password"]:not(.browser-default)[readonly="readonly"],
      input[type="email"]:not(.browser-default):disabled,
      input[type="email"]:not(.browser-default)[readonly="readonly"],
      input[type="url"]:not(.browser-default):disabled,
      input[type="url"]:not(.browser-default)[readonly="readonly"],
      input[type="time"]:not(.browser-default):disabled,
      input[type="time"]:not(.browser-default)[readonly="readonly"],
      input[type="date"]:not(.browser-default):disabled,
      input[type="date"]:not(.browser-default)[readonly="readonly"],
      input[type="datetime"]:not(.browser-default):disabled,
      input[type="datetime"]:not(.browser-default)[readonly="readonly"],
      input[type="datetime-local"]:not(.browser-default):disabled,
      input[type="datetime-local"]:not(.browser-default)[readonly="readonly"],
      input[type="tel"]:not(.browser-default):disabled,
      input[type="tel"]:not(.browser-default)[readonly="readonly"],
      input[type="number"]:not(.browser-default):disabled,
      input[type="number"]:not(.browser-default)[readonly="readonly"],
      input[type="search"]:not(.browser-default):disabled,
      input[type="search"]:not(.browser-default)[readonly="readonly"],
      textarea.materialize-textarea:disabled,
      textarea.materialize-textarea[readonly="readonly"] {
        color: rgba(0, 0, 0, 0.42);
        border-bottom: 1px dotted rgba(0, 0, 0, 0.42);
      }
      input:not([type]):disabled + label,
      input:not([type])[readonly="readonly"] + label,
      input[type="text"]:not(.browser-default):disabled + label,
      input[type="text"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="password"]:not(.browser-default):disabled + label,
      input[type="password"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="email"]:not(.browser-default):disabled + label,
      input[type="email"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="url"]:not(.browser-default):disabled + label,
      input[type="url"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="time"]:not(.browser-default):disabled + label,
      input[type="time"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="date"]:not(.browser-default):disabled + label,
      input[type="date"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="datetime"]:not(.browser-default):disabled + label,
      input[type="datetime"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="datetime-local"]:not(.browser-default):disabled + label,
      input[type="datetime-local"]:not(.browser-default)[readonly="readonly"]
        + label,
      input[type="tel"]:not(.browser-default):disabled + label,
      input[type="tel"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="number"]:not(.browser-default):disabled + label,
      input[type="number"]:not(.browser-default)[readonly="readonly"] + label,
      input[type="search"]:not(.browser-default):disabled + label,
      input[type="search"]:not(.browser-default)[readonly="readonly"] + label,
      textarea.materialize-textarea:disabled + label,
      textarea.materialize-textarea[readonly="readonly"] + label {
        color: rgba(0, 0, 0, 0.42);
      }
      input:not([type]):focus:not([readonly]),
      input[type="text"]:not(.browser-default):focus:not([readonly]),
      input[type="password"]:not(.browser-default):focus:not([readonly]),
      input[type="email"]:not(.browser-default):focus:not([readonly]),
      input[type="url"]:not(.browser-default):focus:not([readonly]),
      input[type="time"]:not(.browser-default):focus:not([readonly]),
      input[type="date"]:not(.browser-default):focus:not([readonly]),
      input[type="datetime"]:not(.browser-default):focus:not([readonly]),
      input[type="datetime-local"]:not(.browser-default):focus:not([readonly]),
      input[type="tel"]:not(.browser-default):focus:not([readonly]),
      input[type="number"]:not(.browser-default):focus:not([readonly]),
      input[type="search"]:not(.browser-default):focus:not([readonly]),
      textarea.materialize-textarea:focus:not([readonly]) {
        border-bottom: 1px solid #26a69a;
        -webkit-box-shadow: 0 1px 0 0 #26a69a;
        box-shadow: 0 1px 0 0 #26a69a;
      }
      input:not([type]):focus:not([readonly]) + label,
      input[type="text"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="password"]:not(.browser-default):focus:not([readonly])
        + label,
      input[type="email"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="url"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="time"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="date"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="datetime"]:not(.browser-default):focus:not([readonly])
        + label,
      input[type="datetime-local"]:not(.browser-default):focus:not([readonly])
        + label,
      input[type="tel"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="number"]:not(.browser-default):focus:not([readonly]) + label,
      input[type="search"]:not(.browser-default):focus:not([readonly]) + label,
      textarea.materialize-textarea:focus:not([readonly]) + label {
        color: #26a69a;
      }
      input:not([type]):focus.valid ~ label,
      input[type="text"]:not(.browser-default):focus.valid ~ label,
      input[type="password"]:not(.browser-default):focus.valid ~ label,
      input[type="email"]:not(.browser-default):focus.valid ~ label,
      input[type="url"]:not(.browser-default):focus.valid ~ label,
      input[type="time"]:not(.browser-default):focus.valid ~ label,
      input[type="date"]:not(.browser-default):focus.valid ~ label,
      input[type="datetime"]:not(.browser-default):focus.valid ~ label,
      input[type="datetime-local"]:not(.browser-default):focus.valid ~ label,
      input[type="tel"]:not(.browser-default):focus.valid ~ label,
      input[type="number"]:not(.browser-default):focus.valid ~ label,
      input[type="search"]:not(.browser-default):focus.valid ~ label,
      textarea.materialize-textarea:focus.valid ~ label {
        color: #4caf50;
      }
      input:not([type]):focus.invalid ~ label,
      input[type="text"]:not(.browser-default):focus.invalid ~ label,
      input[type="password"]:not(.browser-default):focus.invalid ~ label,
      input[type="email"]:not(.browser-default):focus.invalid ~ label,
      input[type="url"]:not(.browser-default):focus.invalid ~ label,
      input[type="time"]:not(.browser-default):focus.invalid ~ label,
      input[type="date"]:not(.browser-default):focus.invalid ~ label,
      input[type="datetime"]:not(.browser-default):focus.invalid ~ label,
      input[type="datetime-local"]:not(.browser-default):focus.invalid ~ label,
      input[type="tel"]:not(.browser-default):focus.invalid ~ label,
      input[type="number"]:not(.browser-default):focus.invalid ~ label,
      input[type="search"]:not(.browser-default):focus.invalid ~ label,
      textarea.materialize-textarea:focus.invalid ~ label {
        color: #f44336;
      }
      input:not([type]).validate + label,
      input[type="text"]:not(.browser-default).validate + label,
      input[type="password"]:not(.browser-default).validate + label,
      input[type="email"]:not(.browser-default).validate + label,
      input[type="url"]:not(.browser-default).validate + label,
      input[type="time"]:not(.browser-default).validate + label,
      input[type="date"]:not(.browser-default).validate + label,
      input[type="datetime"]:not(.browser-default).validate + label,
      input[type="datetime-local"]:not(.browser-default).validate + label,
      input[type="tel"]:not(.browser-default).validate + label,
      input[type="number"]:not(.browser-default).validate + label,
      input[type="search"]:not(.browser-default).validate + label,
      textarea.materialize-textarea.validate + label {
        width: 100%;
      }
      input.valid:not([type]),
      input.valid:not([type]):focus,
      input.valid[type="text"]:not(.browser-default),
      input.valid[type="text"]:not(.browser-default):focus,
      input.valid[type="password"]:not(.browser-default),
      input.valid[type="password"]:not(.browser-default):focus,
      input.valid[type="email"]:not(.browser-default),
      input.valid[type="email"]:not(.browser-default):focus,
      input.valid[type="url"]:not(.browser-default),
      input.valid[type="url"]:not(.browser-default):focus,
      input.valid[type="time"]:not(.browser-default),
      input.valid[type="time"]:not(.browser-default):focus,
      input.valid[type="date"]:not(.browser-default),
      input.valid[type="date"]:not(.browser-default):focus,
      input.valid[type="datetime"]:not(.browser-default),
      input.valid[type="datetime"]:not(.browser-default):focus,
      input.valid[type="datetime-local"]:not(.browser-default),
      input.valid[type="datetime-local"]:not(.browser-default):focus,
      input.valid[type="tel"]:not(.browser-default),
      input.valid[type="tel"]:not(.browser-default):focus,
      input.valid[type="number"]:not(.browser-default),
      input.valid[type="number"]:not(.browser-default):focus,
      input.valid[type="search"]:not(.browser-default),
      input.valid[type="search"]:not(.browser-default):focus,
      textarea.materialize-textarea.valid,
      textarea.materialize-textarea.valid:focus,
      .select-wrapper.valid > input.select-dropdown {
        border-bottom: 1px solid #4caf50;
        -webkit-box-shadow: 0 1px 0 0 #4caf50;
        box-shadow: 0 1px 0 0 #4caf50;
      }
      input.invalid:not([type]),
      input.invalid:not([type]):focus,
      input.invalid[type="text"]:not(.browser-default),
      input.invalid[type="text"]:not(.browser-default):focus,
      input.invalid[type="password"]:not(.browser-default),
      input.invalid[type="password"]:not(.browser-default):focus,
      input.invalid[type="email"]:not(.browser-default),
      input.invalid[type="email"]:not(.browser-default):focus,
      input.invalid[type="url"]:not(.browser-default),
      input.invalid[type="url"]:not(.browser-default):focus,
      input.invalid[type="time"]:not(.browser-default),
      input.invalid[type="time"]:not(.browser-default):focus,
      input.invalid[type="date"]:not(.browser-default),
      input.invalid[type="date"]:not(.browser-default):focus,
      input.invalid[type="datetime"]:not(.browser-default),
      input.invalid[type="datetime"]:not(.browser-default):focus,
      input.invalid[type="datetime-local"]:not(.browser-default),
      input.invalid[type="datetime-local"]:not(.browser-default):focus,
      input.invalid[type="tel"]:not(.browser-default),
      input.invalid[type="tel"]:not(.browser-default):focus,
      input.invalid[type="number"]:not(.browser-default),
      input.invalid[type="number"]:not(.browser-default):focus,
      input.invalid[type="search"]:not(.browser-default),
      input.invalid[type="search"]:not(.browser-default):focus,
      textarea.materialize-textarea.invalid,
      textarea.materialize-textarea.invalid:focus,
      .select-wrapper.invalid > input.select-dropdown,
      .select-wrapper.invalid > input.select-dropdown:focus {
        border-bottom: 1px solid #f44336;
        -webkit-box-shadow: 0 1px 0 0 #f44336;
        box-shadow: 0 1px 0 0 #f44336;
      }
      input:not([type]).valid ~ .helper-text[data-success],
      input:not([type]):focus.valid ~ .helper-text[data-success],
      input:not([type]).invalid ~ .helper-text[data-error],
      input:not([type]):focus.invalid ~ .helper-text[data-error],
      input[type="text"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="text"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="text"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="text"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="password"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="password"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="password"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="password"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="email"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="email"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="email"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="email"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="url"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="url"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="url"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="url"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="time"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="time"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="time"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="time"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="date"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="date"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="date"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="date"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="datetime"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="datetime"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="datetime"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="datetime"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="datetime-local"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="datetime-local"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="datetime-local"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="datetime-local"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="tel"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="tel"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="tel"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="tel"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="number"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="number"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="number"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="number"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      input[type="search"]:not(.browser-default).valid
        ~ .helper-text[data-success],
      input[type="search"]:not(.browser-default):focus.valid
        ~ .helper-text[data-success],
      input[type="search"]:not(.browser-default).invalid
        ~ .helper-text[data-error],
      input[type="search"]:not(.browser-default):focus.invalid
        ~ .helper-text[data-error],
      textarea.materialize-textarea.valid ~ .helper-text[data-success],
      textarea.materialize-textarea:focus.valid ~ .helper-text[data-success],
      textarea.materialize-textarea.invalid ~ .helper-text[data-error],
      textarea.materialize-textarea:focus.invalid ~ .helper-text[data-error],
      .select-wrapper.valid .helper-text[data-success],
      .select-wrapper.invalid ~ .helper-text[data-error] {
        color: transparent;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        pointer-events: none;
      }
      input:not([type]).valid ~ .helper-text:after,
      input:not([type]):focus.valid ~ .helper-text:after,
      input[type="text"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="text"]:not(.browser-default):focus.valid ~ .helper-text:after,
      input[type="password"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="password"]:not(.browser-default):focus.valid
        ~ .helper-text:after,
      input[type="email"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="email"]:not(.browser-default):focus.valid
        ~ .helper-text:after,
      input[type="url"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="url"]:not(.browser-default):focus.valid ~ .helper-text:after,
      input[type="time"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="time"]:not(.browser-default):focus.valid ~ .helper-text:after,
      input[type="date"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="date"]:not(.browser-default):focus.valid ~ .helper-text:after,
      input[type="datetime"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="datetime"]:not(.browser-default):focus.valid
        ~ .helper-text:after,
      input[type="datetime-local"]:not(.browser-default).valid
        ~ .helper-text:after,
      input[type="datetime-local"]:not(.browser-default):focus.valid
        ~ .helper-text:after,
      input[type="tel"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="tel"]:not(.browser-default):focus.valid ~ .helper-text:after,
      input[type="number"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="number"]:not(.browser-default):focus.valid
        ~ .helper-text:after,
      input[type="search"]:not(.browser-default).valid ~ .helper-text:after,
      input[type="search"]:not(.browser-default):focus.valid
        ~ .helper-text:after,
      textarea.materialize-textarea.valid ~ .helper-text:after,
      textarea.materialize-textarea:focus.valid ~ .helper-text:after,
      .select-wrapper.valid ~ .helper-text:after {
        content: attr(data-success);
        color: #4caf50;
      }
      input:not([type]).invalid ~ .helper-text:after,
      input:not([type]):focus.invalid ~ .helper-text:after,
      input[type="text"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="text"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="password"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="password"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="email"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="email"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="url"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="url"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="time"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="time"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="date"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="date"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="datetime"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="datetime"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="datetime-local"]:not(.browser-default).invalid
        ~ .helper-text:after,
      input[type="datetime-local"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="tel"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="tel"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="number"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="number"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      input[type="search"]:not(.browser-default).invalid ~ .helper-text:after,
      input[type="search"]:not(.browser-default):focus.invalid
        ~ .helper-text:after,
      textarea.materialize-textarea.invalid ~ .helper-text:after,
      textarea.materialize-textarea:focus.invalid ~ .helper-text:after,
      .select-wrapper.invalid ~ .helper-text:after {
        content: attr(data-error);
        color: #f44336;
      }
      input:not([type]) + label:after,
      input[type="text"]:not(.browser-default) + label:after,
      input[type="password"]:not(.browser-default) + label:after,
      input[type="email"]:not(.browser-default) + label:after,
      input[type="url"]:not(.browser-default) + label:after,
      input[type="time"]:not(.browser-default) + label:after,
      input[type="date"]:not(.browser-default) + label:after,
      input[type="datetime"]:not(.browser-default) + label:after,
      input[type="datetime-local"]:not(.browser-default) + label:after,
      input[type="tel"]:not(.browser-default) + label:after,
      input[type="number"]:not(.browser-default) + label:after,
      input[type="search"]:not(.browser-default) + label:after,
      textarea.materialize-textarea + label:after,
      .select-wrapper + label:after {
        display: block;
        content: "";
        position: absolute;
        top: 100%;
        left: 0;
        opacity: 0;
        -webkit-transition: 0.2s opacity ease-out, 0.2s color ease-out;
        transition: 0.2s opacity ease-out, 0.2s color ease-out;
      }
      .input-field {
        position: relative;
        margin-top: 1rem;
        margin-bottom: 1rem;
      }
      .input-field.inline {
        display: inline-block;
        vertical-align: middle;
        margin-left: 5px;
      }
      .input-field.inline input,
      .input-field.inline .select-dropdown {
        margin-bottom: 1rem;
      }
      .input-field.col label {
        left: 0.75rem;
      }
      .input-field.col .prefix ~ label,
      .input-field.col .prefix ~ .validate ~ label {
        width: calc(100% - 3rem - 1.5rem);
      }
      .input-field > label {
        color: #9e9e9e;
        position: absolute;
        top: 0;
        left: 0;
        font-size: 1rem;
        cursor: text;
        -webkit-transition: color 0.2s ease-out, -webkit-transform 0.2s ease-out;
        transition: color 0.2s ease-out, -webkit-transform 0.2s ease-out;
        transition: transform 0.2s ease-out, color 0.2s ease-out;
        transition: transform 0.2s ease-out, color 0.2s ease-out,
          -webkit-transform 0.2s ease-out;
        -webkit-transform-origin: 0% 100%;
        transform-origin: 0% 100%;
        text-align: initial;
        -webkit-transform: translateY(12px);
        transform: translateY(12px);
      }
      .input-field > label:not(.label-icon).active {
        -webkit-transform: translateY(-14px) scale(0.8);
        transform: translateY(-14px) scale(0.8);
        -webkit-transform-origin: 0 0;
        transform-origin: 0 0;
      }
      .input-field
        > input[type]:-webkit-autofill:not(.browser-default):not([type="search"])
        + label,
      .input-field > input[type="date"]:not(.browser-default) + label,
      .input-field > input[type="time"]:not(.browser-default) + label {
        -webkit-transform: translateY(-14px) scale(0.8);
        transform: translateY(-14px) scale(0.8);
        -webkit-transform-origin: 0 0;
        transform-origin: 0 0;
      }
      .input-field .helper-text {
        position: relative;
        min-height: 18px;
        display: block;
        font-size: 12px;
        color: rgba(0, 0, 0, 0.54);
      }
      .input-field .helper-text::after {
        opacity: 1;
        position: absolute;
        top: 0;
        left: 0;
      }
      .input-field .prefix {
        position: absolute;
        width: 3rem;
        font-size: 2rem;
        -webkit-transition: color 0.2s;
        transition: color 0.2s;
        top: 0.5rem;
      }
      .input-field .prefix.active {
        color: #26a69a;
      }
      .input-field .prefix ~ input,
      .input-field .prefix ~ textarea,
      .input-field .prefix ~ label,
      .input-field .prefix ~ .validate ~ label,
      .input-field .prefix ~ .helper-text,
      .input-field .prefix ~ .autocomplete-content {
        margin-left: 3rem;
        width: 92%;
        width: calc(100% - 3rem);
      }
      .input-field .prefix ~ label {
        margin-left: 3rem;
      }
      @media only screen and (max-width: 992px) {
        .input-field .prefix ~ input {
          width: 86%;
          width: calc(100% - 3rem);
        }
      }
      @media only screen and (max-width: 600px) {
        .input-field .prefix ~ input {
          width: 80%;
          width: calc(100% - 3rem);
        }
      }
      .input-field input[type="search"] {
        display: block;
        line-height: inherit;
        -webkit-transition: 0.3s background-color;
        transition: 0.3s background-color;
      }
      .nav-wrapper .input-field input[type="search"] {
        height: inherit;
        padding-left: 4rem;
        width: calc(100% - 4rem);
        border: 0;
        -webkit-box-shadow: none;
        box-shadow: none;
      }
      .input-field input[type="search"]:focus:not(.browser-default) {
        background-color: #fff;
        border: 0;
        -webkit-box-shadow: none;
        box-shadow: none;
        color: #444;
      }
      .input-field input[type="search"]:focus:not(.browser-default) + label i,
      .input-field
        input[type="search"]:focus:not(.browser-default)
        ~ .mdi-navigation-close,
      .input-field
        input[type="search"]:focus:not(.browser-default)
        ~ .material-icons {
        color: #444;
      }
      .input-field input[type="search"] + .label-icon {
        -webkit-transform: none;
        transform: none;
        left: 1rem;
      }
      .input-field input[type="search"] ~ .mdi-navigation-close,
      .input-field input[type="search"] ~ .material-icons {
        position: absolute;
        top: 0;
        right: 1rem;
        color: transparent;
        cursor: pointer;
        font-size: 2rem;
        -webkit-transition: 0.3s color;
        transition: 0.3s color;
      }
      textarea {
        width: 100%;
        height: 3rem;
        background-color: transparent;
      }
      textarea.materialize-textarea {
        line-height: normal;
        overflow-y: hidden;
        padding: 0.8rem 0 0.8rem 0;
        resize: none;
        min-height: 3rem;
        -webkit-box-sizing: border-box;
        box-sizing: border-box;
      }
      .hiddendiv {
        visibility: hidden;
        white-space: pre-wrap;
        word-wrap: break-word;
        overflow-wrap: break-word;
        padding-top: 1.2rem;
        position: absolute;
        top: 0;
        z-index: -1;
      }
      .autocomplete-content li .highlight {
        color: #444;
      }
      .autocomplete-content li img {
        height: 40px;
        width: 40px;
        margin: 5px 15px;
      }
      .character-counter {
        min-height: 18px;
      }
      [type="radio"]:not(:checked),
      [type="radio"]:checked {
        position: absolute;
        opacity: 0;
        pointer-events: none;
      }
      [type="radio"]:not(:checked) + span,
      [type="radio"]:checked + span {
        position: relative;
        padding-left: 35px;
        cursor: pointer;
        display: inline-block;
        height: 25px;
        line-height: 25px;
        font-size: 1rem;
        -webkit-transition: 0.28s ease;
        transition: 0.28s ease;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      [type="radio"] + span:before,
      [type="radio"] + span:after {
        content: "";
        position: absolute;
        left: 0;
        top: 0;
        margin: 4px;
        width: 16px;
        height: 16px;
        z-index: 0;
        -webkit-transition: 0.28s ease;
        transition: 0.28s ease;
      }
      [type="radio"]:not(:checked) + span:before,
      [type="radio"]:not(:checked) + span:after,
      [type="radio"]:checked + span:before,
      [type="radio"]:checked + span:after,
      [type="radio"].with-gap:checked + span:before,
      [type="radio"].with-gap:checked + span:after {
        border-radius: 50%;
      }
      [type="radio"]:not(:checked) + span:before,
      [type="radio"]:not(:checked) + span:after {
        border: 2px solid #5a5a5a;
      }
      [type="radio"]:not(:checked) + span:after {
        -webkit-transform: scale(0);
        transform: scale(0);
      }
      [type="radio"]:checked + span:before {
        border: 2px solid transparent;
      }
      [type="radio"]:checked + span:after,
      [type="radio"].with-gap:checked + span:before,
      [type="radio"].with-gap:checked + span:after {
        border: 2px solid #26a69a;
      }
      [type="radio"]:checked + span:after,
      [type="radio"].with-gap:checked + span:after {
        background-color: #26a69a;
      }
      [type="radio"]:checked + span:after {
        -webkit-transform: scale(1.02);
        transform: scale(1.02);
      }
      [type="radio"].with-gap:checked + span:after {
        -webkit-transform: scale(0.5);
        transform: scale(0.5);
      }
      [type="radio"].tabbed:focus + span:before {
        -webkit-box-shadow: 0 0 0 10px rgba(0, 0, 0, 0.1);
        box-shadow: 0 0 0 10px rgba(0, 0, 0, 0.1);
      }
      [type="radio"].with-gap:disabled:checked + span:before {
        border: 2px solid rgba(0, 0, 0, 0.42);
      }
      [type="radio"].with-gap:disabled:checked + span:after {
        border: none;
        background-color: rgba(0, 0, 0, 0.42);
      }
      [type="radio"]:disabled:not(:checked) + span:before,
      [type="radio"]:disabled:checked + span:before {
        background-color: transparent;
        border-color: rgba(0, 0, 0, 0.42);
      }
      [type="radio"]:disabled + span {
        color: rgba(0, 0, 0, 0.42);
      }
      [type="radio"]:disabled:not(:checked) + span:before {
        border-color: rgba(0, 0, 0, 0.42);
      }
      [type="radio"]:disabled:checked + span:after {
        background-color: rgba(0, 0, 0, 0.42);
        border-color: #949494;
      }
      [type="checkbox"]:not(:checked),
      [type="checkbox"]:checked {
        position: absolute;
        opacity: 0;
        pointer-events: none;
      }
      [type="checkbox"] + span:not(.lever) {
        position: relative;
        padding-left: 35px;
        cursor: pointer;
        display: inline-block;
        height: 25px;
        line-height: 25px;
        font-size: 1rem;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      [type="checkbox"] + span:not(.lever):before,
      [type="checkbox"]:not(.filled-in) + span:not(.lever):after {
        content: "";
        position: absolute;
        top: 0;
        left: 0;
        width: 18px;
        height: 18px;
        z-index: 0;
        border: 2px solid #5a5a5a;
        border-radius: 1px;
        margin-top: 3px;
        -webkit-transition: 0.2s;
        transition: 0.2s;
      }
      [type="checkbox"]:not(.filled-in) + span:not(.lever):after {
        border: 0;
        -webkit-transform: scale(0);
        transform: scale(0);
      }
      [type="checkbox"]:not(:checked):disabled + span:not(.lever):before {
        border: none;
        background-color: rgba(0, 0, 0, 0.42);
      }
      [type="checkbox"].tabbed:focus + span:not(.lever):after {
        -webkit-transform: scale(1);
        transform: scale(1);
        border: 0;
        border-radius: 50%;
        -webkit-box-shadow: 0 0 0 10px rgba(0, 0, 0, 0.1);
        box-shadow: 0 0 0 10px rgba(0, 0, 0, 0.1);
        background-color: rgba(0, 0, 0, 0.1);
      }
      [type="checkbox"]:checked + span:not(.lever):before {
        top: -4px;
        left: -5px;
        width: 12px;
        height: 22px;
        border-top: 2px solid transparent;
        border-left: 2px solid transparent;
        border-right: 2px solid #26a69a;
        border-bottom: 2px solid #26a69a;
        -webkit-transform: rotate(40deg);
        transform: rotate(40deg);
        -webkit-backface-visibility: hidden;
        backface-visibility: hidden;
        -webkit-transform-origin: 100% 100%;
        transform-origin: 100% 100%;
      }
      [type="checkbox"]:checked:disabled + span:before {
        border-right: 2px solid rgba(0, 0, 0, 0.42);
        border-bottom: 2px solid rgba(0, 0, 0, 0.42);
      }
      [type="checkbox"]:indeterminate + span:not(.lever):before {
        top: -11px;
        left: -12px;
        width: 10px;
        height: 22px;
        border-top: none;
        border-left: none;
        border-right: 2px solid #26a69a;
        border-bottom: none;
        -webkit-transform: rotate(90deg);
        transform: rotate(90deg);
        -webkit-backface-visibility: hidden;
        backface-visibility: hidden;
        -webkit-transform-origin: 100% 100%;
        transform-origin: 100% 100%;
      }
      [type="checkbox"]:indeterminate:disabled + span:not(.lever):before {
        border-right: 2px solid rgba(0, 0, 0, 0.42);
        background-color: transparent;
      }
      [type="checkbox"].filled-in + span:not(.lever):after {
        border-radius: 2px;
      }
      [type="checkbox"].filled-in + span:not(.lever):before,
      [type="checkbox"].filled-in + span:not(.lever):after {
        content: "";
        left: 0;
        position: absolute;
        -webkit-transition: border 0.25s, background-color 0.25s,
          width 0.2s 0.1s, height 0.2s 0.1s, top 0.2s 0.1s, left 0.2s 0.1s;
        transition: border 0.25s, background-color 0.25s, width 0.2s 0.1s,
          height 0.2s 0.1s, top 0.2s 0.1s, left 0.2s 0.1s;
        z-index: 1;
      }
      [type="checkbox"].filled-in:not(:checked) + span:not(.lever):before {
        width: 0;
        height: 0;
        border: 3px solid transparent;
        left: 6px;
        top: 10px;
        -webkit-transform: rotateZ(37deg);
        transform: rotateZ(37deg);
        -webkit-transform-origin: 100% 100%;
        transform-origin: 100% 100%;
      }
      [type="checkbox"].filled-in:not(:checked) + span:not(.lever):after {
        height: 20px;
        width: 20px;
        background-color: transparent;
        border: 2px solid #5a5a5a;
        top: 0px;
        z-index: 0;
      }
      [type="checkbox"].filled-in:checked + span:not(.lever):before {
        top: 0;
        left: 1px;
        width: 8px;
        height: 13px;
        border-top: 2px solid transparent;
        border-left: 2px solid transparent;
        border-right: 2px solid #fff;
        border-bottom: 2px solid #fff;
        -webkit-transform: rotateZ(37deg);
        transform: rotateZ(37deg);
        -webkit-transform-origin: 100% 100%;
        transform-origin: 100% 100%;
      }
      [type="checkbox"].filled-in:checked + span:not(.lever):after {
        top: 0;
        width: 20px;
        height: 20px;
        border: 2px solid #26a69a;
        background-color: #26a69a;
        z-index: 0;
      }
      [type="checkbox"].filled-in.tabbed:focus + span:not(.lever):after {
        border-radius: 2px;
        border-color: #5a5a5a;
        background-color: rgba(0, 0, 0, 0.1);
      }
      [type="checkbox"].filled-in.tabbed:checked:focus
        + span:not(.lever):after {
        border-radius: 2px;
        background-color: #26a69a;
        border-color: #26a69a;
      }
      [type="checkbox"].filled-in:disabled:not(:checked)
        + span:not(.lever):before {
        background-color: transparent;
        border: 2px solid transparent;
      }
      [type="checkbox"].filled-in:disabled:not(:checked)
        + span:not(.lever):after {
        border-color: transparent;
        background-color: #949494;
      }
      [type="checkbox"].filled-in:disabled:checked + span:not(.lever):before {
        background-color: transparent;
      }
      [type="checkbox"].filled-in:disabled:checked + span:not(.lever):after {
        background-color: #949494;
        border-color: #949494;
      }
      .switch,
      .switch * {
        -webkit-tap-highlight-color: transparent;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      .switch label {
        cursor: pointer;
      }
      .switch label input[type="checkbox"] {
        opacity: 0;
        width: 0;
        height: 0;
      }
      .switch label input[type="checkbox"]:checked + .lever {
        background-color: #84c7c1;
      }
      .switch label input[type="checkbox"]:checked + .lever:before,
      .switch label input[type="checkbox"]:checked + .lever:after {
        left: 18px;
      }
      .switch label input[type="checkbox"]:checked + .lever:after {
        background-color: #26a69a;
      }
      .switch label .lever {
        content: "";
        display: inline-block;
        position: relative;
        width: 36px;
        height: 14px;
        background-color: rgba(0, 0, 0, 0.38);
        border-radius: 15px;
        margin-right: 10px;
        -webkit-transition: background 0.3s ease;
        transition: background 0.3s ease;
        vertical-align: middle;
        margin: 0 16px;
      }
      .switch label .lever:before,
      .switch label .lever:after {
        content: "";
        position: absolute;
        display: inline-block;
        width: 20px;
        height: 20px;
        border-radius: 50%;
        left: 0;
        top: -3px;
        -webkit-transition: left 0.3s ease, background 0.3s ease,
          -webkit-box-shadow 0.1s ease, -webkit-transform 0.1s ease;
        transition: left 0.3s ease, background 0.3s ease,
          -webkit-box-shadow 0.1s ease, -webkit-transform 0.1s ease;
        transition: left 0.3s ease, background 0.3s ease, box-shadow 0.1s ease,
          transform 0.1s ease;
        transition: left 0.3s ease, background 0.3s ease, box-shadow 0.1s ease,
          transform 0.1s ease, -webkit-box-shadow 0.1s ease,
          -webkit-transform 0.1s ease;
      }
      .switch label .lever:before {
        background-color: rgba(38, 166, 154, 0.15);
      }
      .switch label .lever:after {
        background-color: #f1f1f1;
        -webkit-box-shadow: 0px 3px 1px -2px rgba(0, 0, 0, 0.2),
          0px 2px 2px 0px rgba(0, 0, 0, 0.14),
          0px 1px 5px 0px rgba(0, 0, 0, 0.12);
        box-shadow: 0px 3px 1px -2px rgba(0, 0, 0, 0.2),
          0px 2px 2px 0px rgba(0, 0, 0, 0.14),
          0px 1px 5px 0px rgba(0, 0, 0, 0.12);
      }
      input[type="checkbox"]:checked:not(:disabled) ~ .lever:active::before,
      input[type="checkbox"]:checked:not(:disabled).tabbed:focus
        ~ .lever::before {
        -webkit-transform: scale(2.4);
        transform: scale(2.4);
        background-color: rgba(38, 166, 154, 0.15);
      }
      input[type="checkbox"]:not(:disabled) ~ .lever:active:before,
      input[type="checkbox"]:not(:disabled).tabbed:focus ~ .lever::before {
        -webkit-transform: scale(2.4);
        transform: scale(2.4);
        background-color: rgba(0, 0, 0, 0.08);
      }
      .switch input[type="checkbox"][disabled] + .lever {
        cursor: default;
        background-color: rgba(0, 0, 0, 0.12);
      }
      .switch label input[type="checkbox"][disabled] + .lever:after,
      .switch label input[type="checkbox"][disabled]:checked + .lever:after {
        background-color: #949494;
      }
      select {
        display: none;
      }
      select.browser-default {
        display: block;
      }
      select {
        background-color: rgba(255, 255, 255, 0.9);
        width: 100%;
        padding: 5px;
        border: 1px solid #f2f2f2;
        border-radius: 2px;
        height: 3rem;
      }
      .select-label {
        position: absolute;
      }
      .select-wrapper {
        position: relative;
      }
      .select-wrapper.valid + label,
      .select-wrapper.invalid + label {
        width: 100%;
        pointer-events: none;
      }
      .select-wrapper input.select-dropdown {
        position: relative;
        cursor: pointer;
        background-color: transparent;
        border: none;
        border-bottom: 1px solid #9e9e9e;
        outline: none;
        height: 3rem;
        line-height: 3rem;
        width: 100%;
        font-size: 16px;
        margin: 0 0 8px 0;
        padding: 0;
        display: block;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        z-index: 1;
      }
      .select-wrapper input.select-dropdown:focus {
        border-bottom: 1px solid #26a69a;
      }
      .select-wrapper .caret {
        position: absolute;
        right: 0;
        top: 0;
        bottom: 0;
        margin: auto 0;
        z-index: 0;
        fill: rgba(0, 0, 0, 0.87);
      }
      .select-wrapper + label {
        position: absolute;
        top: -26px;
        font-size: 0.8rem;
      }
      select:disabled {
        color: rgba(0, 0, 0, 0.42);
      }
      .select-wrapper.disabled + label {
        color: rgba(0, 0, 0, 0.42);
      }
      .select-wrapper.disabled .caret {
        fill: rgba(0, 0, 0, 0.42);
      }
      .select-wrapper input.select-dropdown:disabled {
        color: rgba(0, 0, 0, 0.42);
        cursor: default;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      .select-wrapper i {
        color: rgba(0, 0, 0, 0.3);
      }
      .select-dropdown li.disabled,
      .select-dropdown li.disabled > span,
      .select-dropdown li.optgroup {
        color: rgba(0, 0, 0, 0.3);
        background-color: transparent;
      }
      body.keyboard-focused .select-dropdown.dropdown-content li:focus {
        background-color: rgba(0, 0, 0, 0.08);
      }
      .select-dropdown.dropdown-content li:hover {
        background-color: rgba(0, 0, 0, 0.08);
      }
      .select-dropdown.dropdown-content li.selected {
        background-color: rgba(0, 0, 0, 0.03);
      }
      .prefix ~ .select-wrapper {
        margin-left: 3rem;
        width: 92%;
        width: calc(100% - 3rem);
      }
      .prefix ~ label {
        margin-left: 3rem;
      }
      .select-dropdown li img {
        height: 40px;
        width: 40px;
        margin: 5px 15px;
        float: right;
      }
      .select-dropdown li.optgroup {
        border-top: 1px solid #eee;
      }
      .select-dropdown li.optgroup.selected > span {
        color: rgba(0, 0, 0, 0.7);
      }
      .select-dropdown li.optgroup > span {
        color: rgba(0, 0, 0, 0.4);
      }
      .select-dropdown li.optgroup ~ li.optgroup-option {
        padding-left: 1rem;
      }
      .file-field {
        position: relative;
      }
      .file-field .file-path-wrapper {
        overflow: hidden;
        padding-left: 10px;
      }
      .file-field input.file-path {
        width: 100%;
      }
      .file-field .btn,
      .file-field .btn-large,
      .file-field .btn-small {
        float: left;
        height: 3rem;
        line-height: 3rem;
      }
      .file-field span {
        cursor: pointer;
      }
      .file-field input[type="file"] {
        position: absolute;
        top: 0;
        right: 0;
        left: 0;
        bottom: 0;
        width: 100%;
        margin: 0;
        padding: 0;
        font-size: 20px;
        cursor: pointer;
        opacity: 0;
        filter: alpha(opacity=0);
      }
      .file-field input[type="file"]::-webkit-file-upload-button {
        display: none;
      }
      .range-field {
        position: relative;
      }
      input[type="range"],
      input[type="range"] + .thumb {
        cursor: pointer;
      }
      input[type="range"] {
        position: relative;
        background-color: transparent;
        border: none;
        outline: none;
        width: 100%;
        margin: 15px 0;
        padding: 0;
      }
      input[type="range"]:focus {
        outline: none;
      }
      input[type="range"] + .thumb {
        position: absolute;
        top: 10px;
        left: 0;
        border: none;
        height: 0;
        width: 0;
        border-radius: 50%;
        background-color: #26a69a;
        margin-left: 7px;
        -webkit-transform-origin: 50% 50%;
        transform-origin: 50% 50%;
        -webkit-transform: rotate(-45deg);
        transform: rotate(-45deg);
      }
      input[type="range"] + .thumb .value {
        display: block;
        width: 30px;
        text-align: center;
        color: #26a69a;
        font-size: 0;
        -webkit-transform: rotate(45deg);
        transform: rotate(45deg);
      }
      input[type="range"] + .thumb.active {
        border-radius: 50% 50% 50% 0;
      }
      input[type="range"] + .thumb.active .value {
        color: #fff;
        margin-left: -1px;
        margin-top: 8px;
        font-size: 10px;
      }
      input[type="range"] {
        -webkit-appearance: none;
      }
      input[type="range"]::-webkit-slider-runnable-track {
        height: 3px;
        background: #c2c0c2;
        border: none;
      }
      input[type="range"]::-webkit-slider-thumb {
        border: none;
        height: 14px;
        width: 14px;
        border-radius: 50%;
        background: #26a69a;
        -webkit-transition: -webkit-box-shadow 0.3s;
        transition: -webkit-box-shadow 0.3s;
        transition: box-shadow 0.3s;
        transition: box-shadow 0.3s, -webkit-box-shadow 0.3s;
        -webkit-appearance: none;
        background-color: #26a69a;
        -webkit-transform-origin: 50% 50%;
        transform-origin: 50% 50%;
        margin: -5px 0 0 0;
      }
      .keyboard-focused
        input[type="range"]:focus:not(.active)::-webkit-slider-thumb {
        -webkit-box-shadow: 0 0 0 10px rgba(38, 166, 154, 0.26);
        box-shadow: 0 0 0 10px rgba(38, 166, 154, 0.26);
      }
      input[type="range"] {
        border: 1px solid white;
      }
      input[type="range"]::-moz-range-track {
        height: 3px;
        background: #c2c0c2;
        border: none;
      }
      input[type="range"]::-moz-focus-inner {
        border: 0;
      }
      input[type="range"]::-moz-range-thumb {
        border: none;
        height: 14px;
        width: 14px;
        border-radius: 50%;
        background: #26a69a;
        -webkit-transition: -webkit-box-shadow 0.3s;
        transition: -webkit-box-shadow 0.3s;
        transition: box-shadow 0.3s;
        transition: box-shadow 0.3s, -webkit-box-shadow 0.3s;
        margin-top: -5px;
      }
      input[type="range"]:-moz-focusring {
        outline: 1px solid #fff;
        outline-offset: -1px;
      }
      .keyboard-focused
        input[type="range"]:focus:not(.active)::-moz-range-thumb {
        box-shadow: 0 0 0 10px rgba(38, 166, 154, 0.26);
      }
      input[type="range"]::-ms-track {
        height: 3px;
        background: transparent;
        border-color: transparent;
        border-width: 6px 0;
        color: transparent;
      }
      input[type="range"]::-ms-fill-lower {
        background: #777;
      }
      input[type="range"]::-ms-fill-upper {
        background: #ddd;
      }
      input[type="range"]::-ms-thumb {
        border: none;
        height: 14px;
        width: 14px;
        border-radius: 50%;
        background: #26a69a;
        -webkit-transition: -webkit-box-shadow 0.3s;
        transition: -webkit-box-shadow 0.3s;
        transition: box-shadow 0.3s;
        transition: box-shadow 0.3s, -webkit-box-shadow 0.3s;
      }
      .keyboard-focused input[type="range"]:focus:not(.active)::-ms-thumb {
        box-shadow: 0 0 0 10px rgba(38, 166, 154, 0.26);
      }
      .table-of-contents.fixed {
        position: fixed;
      }
      .table-of-contents li {
        padding: 2px 0;
      }
      .table-of-contents a {
        display: inline-block;
        font-weight: 300;
        color: #757575;
        padding-left: 16px;
        height: 1.5rem;
        line-height: 1.5rem;
        letter-spacing: 0.4;
        display: inline-block;
      }
      .table-of-contents a:hover {
        color: #a8a8a8;
        padding-left: 15px;
        border-left: 1px solid #ee6e73;
      }
      .table-of-contents a.active {
        font-weight: 500;
        padding-left: 14px;
        border-left: 2px solid #ee6e73;
      }
      .sidenav {
        position: fixed;
        width: 300px;
        left: 0;
        top: 0;
        margin: 0;
        -webkit-transform: translateX(-100%);
        transform: translateX(-100%);
        height: 100%;
        height: calc(100% + 60px);
        height: -moz-calc(100%);
        padding-bottom: 60px;
        background-color: #fff;
        z-index: 999;
        overflow-y: auto;
        will-change: transform;
        -webkit-backface-visibility: hidden;
        backface-visibility: hidden;
        -webkit-transform: translateX(-105%);
        transform: translateX(-105%);
      }
      .sidenav.right-aligned {
        right: 0;
        -webkit-transform: translateX(105%);
        transform: translateX(105%);
        left: auto;
        -webkit-transform: translateX(100%);
        transform: translateX(100%);
      }
      .sidenav .collapsible {
        margin: 0;
      }
      .sidenav li {
        float: none;
        line-height: 48px;
      }
      .sidenav li.active {
        background-color: rgba(0, 0, 0, 0.05);
      }
      .sidenav li > a {
        color: rgba(0, 0, 0, 0.87);
        display: block;
        font-size: 14px;
        font-weight: 500;
        height: 48px;
        line-height: 48px;
        padding: 0 32px;
      }
      .sidenav li > a:hover {
        background-color: rgba(0, 0, 0, 0.05);
      }
      .sidenav li > a.btn,
      .sidenav li > a.btn-large,
      .sidenav li > a.btn-small,
      .sidenav li > a.btn-large,
      .sidenav li > a.btn-flat,
      .sidenav li > a.btn-floating {
        margin: 10px 15px;
      }
      .sidenav li > a.btn,
      .sidenav li > a.btn-large,
      .sidenav li > a.btn-small,
      .sidenav li > a.btn-large,
      .sidenav li > a.btn-floating {
        color: #fff;
      }
      .sidenav li > a.btn-flat {
        color: #343434;
      }
      .sidenav li > a.btn:hover,
      .sidenav li > a.btn-large:hover,
      .sidenav li > a.btn-small:hover,
      .sidenav li > a.btn-large:hover {
        background-color: #2bbbad;
      }
      .sidenav li > a.btn-floating:hover {
        background-color: #26a69a;
      }
      .sidenav li > a > i,
      .sidenav li > a > [class^="mdi-"],
      .sidenav li > a li > a > [class*="mdi-"],
      .sidenav li > a > i.material-icons {
        float: left;
        height: 48px;
        line-height: 48px;
        margin: 0 32px 0 0;
        width: 24px;
        color: rgba(0, 0, 0, 0.54);
      }
      .sidenav .divider {
        margin: 8px 0 0 0;
      }
      .sidenav .subheader {
        cursor: initial;
        pointer-events: none;
        color: rgba(0, 0, 0, 0.54);
        font-size: 14px;
        font-weight: 500;
        line-height: 48px;
      }
      .sidenav .subheader:hover {
        background-color: transparent;
      }
      .sidenav .user-view {
        position: relative;
        padding: 32px 32px 0;
        margin-bottom: 8px;
      }
      .sidenav .user-view > a {
        height: auto;
        padding: 0;
      }
      .sidenav .user-view > a:hover {
        background-color: transparent;
      }
      .sidenav .user-view .background {
        overflow: hidden;
        position: absolute;
        top: 0;
        right: 0;
        bottom: 0;
        left: 0;
        z-index: -1;
      }
      .sidenav .user-view .circle,
      .sidenav .user-view .name,
      .sidenav .user-view .email {
        display: block;
      }
      .sidenav .user-view .circle {
        height: 64px;
        width: 64px;
      }
      .sidenav .user-view .name,
      .sidenav .user-view .email {
        font-size: 14px;
        line-height: 24px;
      }
      .sidenav .user-view .name {
        margin-top: 16px;
        font-weight: 500;
      }
      .sidenav .user-view .email {
        padding-bottom: 16px;
        font-weight: 400;
      }
      .drag-target {
        height: 100%;
        width: 10px;
        position: fixed;
        top: 0;
        z-index: 998;
      }
      .drag-target.right-aligned {
        right: 0;
      }
      .sidenav.sidenav-fixed {
        left: 0;
        -webkit-transform: translateX(0);
        transform: translateX(0);
        position: fixed;
      }
      .sidenav.sidenav-fixed.right-aligned {
        right: 0;
        left: auto;
      }
      @media only screen and (max-width: 992px) {
        .sidenav.sidenav-fixed {
          -webkit-transform: translateX(-105%);
          transform: translateX(-105%);
        }
        .sidenav.sidenav-fixed.right-aligned {
          -webkit-transform: translateX(105%);
          transform: translateX(105%);
        }
        .sidenav > a {
          padding: 0 16px;
        }
        .sidenav .user-view {
          padding: 16px 16px 0;
        }
      }
      .sidenav .collapsible-body > ul:not(.collapsible) > li.active,
      .sidenav.sidenav-fixed
        .collapsible-body
        > ul:not(.collapsible)
        > li.active {
        background-color: #ee6e73;
      }
      .sidenav .collapsible-body > ul:not(.collapsible) > li.active a,
      .sidenav.sidenav-fixed
        .collapsible-body
        > ul:not(.collapsible)
        > li.active
        a {
        color: #fff;
      }
      .sidenav .collapsible-body {
        padding: 0;
      }
      .sidenav-overlay {
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        opacity: 0;
        height: 120vh;
        background-color: rgba(0, 0, 0, 0.5);
        z-index: 997;
        display: none;
      }
      .preloader-wrapper {
        display: inline-block;
        position: relative;
        width: 50px;
        height: 50px;
      }
      .preloader-wrapper.small {
        width: 36px;
        height: 36px;
      }
      .preloader-wrapper.big {
        width: 64px;
        height: 64px;
      }
      .preloader-wrapper.active {
        -webkit-animation: container-rotate 1568ms linear infinite;
        animation: container-rotate 1568ms linear infinite;
      }
      @-webkit-keyframes container-rotate {
        to {
          -webkit-transform: rotate(360deg);
        }
      }
      @keyframes container-rotate {
        to {
          -webkit-transform: rotate(360deg);
          transform: rotate(360deg);
        }
      }
      .spinner-layer {
        position: absolute;
        width: 100%;
        height: 100%;
        opacity: 0;
        border-color: #26a69a;
      }
      .spinner-blue,
      .spinner-blue-only {
        border-color: #4285f4;
      }
      .spinner-red,
      .spinner-red-only {
        border-color: #db4437;
      }
      .spinner-yellow,
      .spinner-yellow-only {
        border-color: #f4b400;
      }
      .spinner-green,
      .spinner-green-only {
        border-color: #0f9d58;
      }
      .active .spinner-layer.spinner-blue {
        -webkit-animation: fill-unfill-rotate 5332ms
            cubic-bezier(0.4, 0, 0.2, 1) infinite both,
          blue-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
        animation: fill-unfill-rotate 5332ms cubic-bezier(0.4, 0, 0.2, 1)
            infinite both,
          blue-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
      }
      .active .spinner-layer.spinner-red {
        -webkit-animation: fill-unfill-rotate 5332ms
            cubic-bezier(0.4, 0, 0.2, 1) infinite both,
          red-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
        animation: fill-unfill-rotate 5332ms cubic-bezier(0.4, 0, 0.2, 1)
            infinite both,
          red-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
      }
      .active .spinner-layer.spinner-yellow {
        -webkit-animation: fill-unfill-rotate 5332ms
            cubic-bezier(0.4, 0, 0.2, 1) infinite both,
          yellow-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
        animation: fill-unfill-rotate 5332ms cubic-bezier(0.4, 0, 0.2, 1)
            infinite both,
          yellow-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
      }
      .active .spinner-layer.spinner-green {
        -webkit-animation: fill-unfill-rotate 5332ms
            cubic-bezier(0.4, 0, 0.2, 1) infinite both,
          green-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
        animation: fill-unfill-rotate 5332ms cubic-bezier(0.4, 0, 0.2, 1)
            infinite both,
          green-fade-in-out 5332ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
      }
      .active .spinner-layer,
      .active .spinner-layer.spinner-blue-only,
      .active .spinner-layer.spinner-red-only,
      .active .spinner-layer.spinner-yellow-only,
      .active .spinner-layer.spinner-green-only {
        opacity: 1;
        -webkit-animation: fill-unfill-rotate 5332ms
          cubic-bezier(0.4, 0, 0.2, 1) infinite both;
        animation: fill-unfill-rotate 5332ms cubic-bezier(0.4, 0, 0.2, 1)
          infinite both;
      }
      @-webkit-keyframes fill-unfill-rotate {
        12.5% {
          -webkit-transform: rotate(135deg);
        }
        25% {
          -webkit-transform: rotate(270deg);
        }
        37.5% {
          -webkit-transform: rotate(405deg);
        }
        50% {
          -webkit-transform: rotate(540deg);
        }
        62.5% {
          -webkit-transform: rotate(675deg);
        }
        75% {
          -webkit-transform: rotate(810deg);
        }
        87.5% {
          -webkit-transform: rotate(945deg);
        }
        to {
          -webkit-transform: rotate(1080deg);
        }
      }
      @keyframes fill-unfill-rotate {
        12.5% {
          -webkit-transform: rotate(135deg);
          transform: rotate(135deg);
        }
        25% {
          -webkit-transform: rotate(270deg);
          transform: rotate(270deg);
        }
        37.5% {
          -webkit-transform: rotate(405deg);
          transform: rotate(405deg);
        }
        50% {
          -webkit-transform: rotate(540deg);
          transform: rotate(540deg);
        }
        62.5% {
          -webkit-transform: rotate(675deg);
          transform: rotate(675deg);
        }
        75% {
          -webkit-transform: rotate(810deg);
          transform: rotate(810deg);
        }
        87.5% {
          -webkit-transform: rotate(945deg);
          transform: rotate(945deg);
        }
        to {
          -webkit-transform: rotate(1080deg);
          transform: rotate(1080deg);
        }
      }
      @-webkit-keyframes blue-fade-in-out {
        from {
          opacity: 1;
        }
        25% {
          opacity: 1;
        }
        26% {
          opacity: 0;
        }
        89% {
          opacity: 0;
        }
        90% {
          opacity: 1;
        }
        100% {
          opacity: 1;
        }
      }
      @keyframes blue-fade-in-out {
        from {
          opacity: 1;
        }
        25% {
          opacity: 1;
        }
        26% {
          opacity: 0;
        }
        89% {
          opacity: 0;
        }
        90% {
          opacity: 1;
        }
        100% {
          opacity: 1;
        }
      }
      @-webkit-keyframes red-fade-in-out {
        from {
          opacity: 0;
        }
        15% {
          opacity: 0;
        }
        25% {
          opacity: 1;
        }
        50% {
          opacity: 1;
        }
        51% {
          opacity: 0;
        }
      }
      @keyframes red-fade-in-out {
        from {
          opacity: 0;
        }
        15% {
          opacity: 0;
        }
        25% {
          opacity: 1;
        }
        50% {
          opacity: 1;
        }
        51% {
          opacity: 0;
        }
      }
      @-webkit-keyframes yellow-fade-in-out {
        from {
          opacity: 0;
        }
        40% {
          opacity: 0;
        }
        50% {
          opacity: 1;
        }
        75% {
          opacity: 1;
        }
        76% {
          opacity: 0;
        }
      }
      @keyframes yellow-fade-in-out {
        from {
          opacity: 0;
        }
        40% {
          opacity: 0;
        }
        50% {
          opacity: 1;
        }
        75% {
          opacity: 1;
        }
        76% {
          opacity: 0;
        }
      }
      @-webkit-keyframes green-fade-in-out {
        from {
          opacity: 0;
        }
        65% {
          opacity: 0;
        }
        75% {
          opacity: 1;
        }
        90% {
          opacity: 1;
        }
        100% {
          opacity: 0;
        }
      }
      @keyframes green-fade-in-out {
        from {
          opacity: 0;
        }
        65% {
          opacity: 0;
        }
        75% {
          opacity: 1;
        }
        90% {
          opacity: 1;
        }
        100% {
          opacity: 0;
        }
      }
      .gap-patch {
        position: absolute;
        top: 0;
        left: 45%;
        width: 10%;
        height: 100%;
        overflow: hidden;
        border-color: inherit;
      }
      .gap-patch .circle {
        width: 1000%;
        left: -450%;
      }
      .circle-clipper {
        display: inline-block;
        position: relative;
        width: 50%;
        height: 100%;
        overflow: hidden;
        border-color: inherit;
      }
      .circle-clipper .circle {
        width: 200%;
        height: 100%;
        border-width: 3px;
        border-style: solid;
        border-color: inherit;
        border-bottom-color: transparent !important;
        border-radius: 50%;
        -webkit-animation: none;
        animation: none;
        position: absolute;
        top: 0;
        right: 0;
        bottom: 0;
      }
      .circle-clipper.left .circle {
        left: 0;
        border-right-color: transparent !important;
        -webkit-transform: rotate(129deg);
        transform: rotate(129deg);
      }
      .circle-clipper.right .circle {
        left: -100%;
        border-left-color: transparent !important;
        -webkit-transform: rotate(-129deg);
        transform: rotate(-129deg);
      }
      .active .circle-clipper.left .circle {
        -webkit-animation: left-spin 1333ms cubic-bezier(0.4, 0, 0.2, 1)
          infinite both;
        animation: left-spin 1333ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
      }
      .active .circle-clipper.right .circle {
        -webkit-animation: right-spin 1333ms cubic-bezier(0.4, 0, 0.2, 1)
          infinite both;
        animation: right-spin 1333ms cubic-bezier(0.4, 0, 0.2, 1) infinite both;
      }
      @-webkit-keyframes left-spin {
        from {
          -webkit-transform: rotate(130deg);
        }
        50% {
          -webkit-transform: rotate(-5deg);
        }
        to {
          -webkit-transform: rotate(130deg);
        }
      }
      @keyframes left-spin {
        from {
          -webkit-transform: rotate(130deg);
          transform: rotate(130deg);
        }
        50% {
          -webkit-transform: rotate(-5deg);
          transform: rotate(-5deg);
        }
        to {
          -webkit-transform: rotate(130deg);
          transform: rotate(130deg);
        }
      }
      @-webkit-keyframes right-spin {
        from {
          -webkit-transform: rotate(-130deg);
        }
        50% {
          -webkit-transform: rotate(5deg);
        }
        to {
          -webkit-transform: rotate(-130deg);
        }
      }
      @keyframes right-spin {
        from {
          -webkit-transform: rotate(-130deg);
          transform: rotate(-130deg);
        }
        50% {
          -webkit-transform: rotate(5deg);
          transform: rotate(5deg);
        }
        to {
          -webkit-transform: rotate(-130deg);
          transform: rotate(-130deg);
        }
      }
      #spinnerContainer.cooldown {
        -webkit-animation: container-rotate 1568ms linear infinite,
          fade-out 400ms cubic-bezier(0.4, 0, 0.2, 1);
        animation: container-rotate 1568ms linear infinite,
          fade-out 400ms cubic-bezier(0.4, 0, 0.2, 1);
      }
      @-webkit-keyframes fade-out {
        from {
          opacity: 1;
        }
        to {
          opacity: 0;
        }
      }
      @keyframes fade-out {
        from {
          opacity: 1;
        }
        to {
          opacity: 0;
        }
      }
      .slider {
        position: relative;
        height: 400px;
        width: 100%;
      }
      .slider.fullscreen {
        height: 100%;
        width: 100%;
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
      }
      .slider.fullscreen ul.slides {
        height: 100%;
      }
      .slider.fullscreen ul.indicators {
        z-index: 2;
        bottom: 30px;
      }
      .slider .slides {
        background-color: #9e9e9e;
        margin: 0;
        height: 400px;
      }
      .slider .slides li {
        opacity: 0;
        position: absolute;
        top: 0;
        left: 0;
        z-index: 1;
        width: 100%;
        height: inherit;
        overflow: hidden;
      }
      .slider .slides li img {
        height: 100%;
        width: 100%;
        background-size: cover;
        background-position: center;
      }
      .slider .slides li .caption {
        color: #fff;
        position: absolute;
        top: 15%;
        left: 15%;
        width: 70%;
        opacity: 0;
      }
      .slider .slides li .caption p {
        color: #e0e0e0;
      }
      .slider .slides li.active {
        z-index: 2;
      }
      .slider .indicators {
        position: absolute;
        text-align: center;
        left: 0;
        right: 0;
        bottom: 0;
        margin: 0;
      }
      .slider .indicators .indicator-item {
        display: inline-block;
        position: relative;
        cursor: pointer;
        height: 16px;
        width: 16px;
        margin: 0 12px;
        background-color: #e0e0e0;
        -webkit-transition: background-color 0.3s;
        transition: background-color 0.3s;
        border-radius: 50%;
      }
      .slider .indicators .indicator-item.active {
        background-color: #4caf50;
      }
      .carousel {
        overflow: hidden;
        position: relative;
        width: 100%;
        height: 400px;
        -webkit-perspective: 500px;
        perspective: 500px;
        -webkit-transform-style: preserve-3d;
        transform-style: preserve-3d;
        -webkit-transform-origin: 0% 50%;
        transform-origin: 0% 50%;
      }
      .carousel.carousel-slider {
        top: 0;
        left: 0;
      }
      .carousel.carousel-slider .carousel-fixed-item {
        position: absolute;
        left: 0;
        right: 0;
        bottom: 20px;
        z-index: 1;
      }
      .carousel.carousel-slider .carousel-fixed-item.with-indicators {
        bottom: 68px;
      }
      .carousel.carousel-slider .carousel-item {
        width: 100%;
        height: 100%;
        min-height: 400px;
        position: absolute;
        top: 0;
        left: 0;
      }
      .carousel.carousel-slider .carousel-item h2 {
        font-size: 24px;
        font-weight: 500;
        line-height: 32px;
      }
      .carousel.carousel-slider .carousel-item p {
        font-size: 15px;
      }
      .carousel .carousel-item {
        visibility: hidden;
        width: 200px;
        height: 200px;
        position: absolute;
        top: 0;
        left: 0;
      }
      .carousel .carousel-item > img {
        width: 100%;
      }
      .carousel .indicators {
        position: absolute;
        text-align: center;
        left: 0;
        right: 0;
        bottom: 0;
        margin: 0;
      }
      .carousel .indicators .indicator-item {
        display: inline-block;
        position: relative;
        cursor: pointer;
        height: 8px;
        width: 8px;
        margin: 24px 4px;
        background-color: rgba(255, 255, 255, 0.5);
        -webkit-transition: background-color 0.3s;
        transition: background-color 0.3s;
        border-radius: 50%;
      }
      .carousel .indicators .indicator-item.active {
        background-color: #fff;
      }
      .carousel.scrolling .carousel-item .materialboxed,
      .carousel .carousel-item:not(.active) .materialboxed {
        pointer-events: none;
      }
      .tap-target-wrapper {
        width: 800px;
        height: 800px;
        position: fixed;
        z-index: 1000;
        visibility: hidden;
        -webkit-transition: visibility 0s 0.3s;
        transition: visibility 0s 0.3s;
      }
      .tap-target-wrapper.open {
        visibility: visible;
        -webkit-transition: visibility 0s;
        transition: visibility 0s;
      }
      .tap-target-wrapper.open .tap-target {
        -webkit-transform: scale(1);
        transform: scale(1);
        opacity: 0.95;
        -webkit-transition: opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          -webkit-transform 0.3s cubic-bezier(0.42, 0, 0.58, 1);
        transition: opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          -webkit-transform 0.3s cubic-bezier(0.42, 0, 0.58, 1);
        transition: transform 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1);
        transition: transform 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          -webkit-transform 0.3s cubic-bezier(0.42, 0, 0.58, 1);
      }
      .tap-target-wrapper.open .tap-target-wave::before {
        -webkit-transform: scale(1);
        transform: scale(1);
      }
      .tap-target-wrapper.open .tap-target-wave::after {
        visibility: visible;
        -webkit-animation: pulse-animation 1s cubic-bezier(0.24, 0, 0.38, 1)
          infinite;
        animation: pulse-animation 1s cubic-bezier(0.24, 0, 0.38, 1) infinite;
        -webkit-transition: opacity 0.3s, visibility 0s 1s,
          -webkit-transform 0.3s;
        transition: opacity 0.3s, visibility 0s 1s, -webkit-transform 0.3s;
        transition: opacity 0.3s, transform 0.3s, visibility 0s 1s;
        transition: opacity 0.3s, transform 0.3s, visibility 0s 1s,
          -webkit-transform 0.3s;
      }
      .tap-target {
        position: absolute;
        font-size: 1rem;
        border-radius: 50%;
        background-color: #ee6e73;
        -webkit-box-shadow: 0 20px 20px 0 rgba(0, 0, 0, 0.14),
          0 10px 50px 0 rgba(0, 0, 0, 0.12),
          0 30px 10px -20px rgba(0, 0, 0, 0.2);
        box-shadow: 0 20px 20px 0 rgba(0, 0, 0, 0.14),
          0 10px 50px 0 rgba(0, 0, 0, 0.12),
          0 30px 10px -20px rgba(0, 0, 0, 0.2);
        width: 100%;
        height: 100%;
        opacity: 0;
        -webkit-transform: scale(0);
        transform: scale(0);
        -webkit-transition: opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          -webkit-transform 0.3s cubic-bezier(0.42, 0, 0.58, 1);
        transition: opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          -webkit-transform 0.3s cubic-bezier(0.42, 0, 0.58, 1);
        transition: transform 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1);
        transition: transform 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          opacity 0.3s cubic-bezier(0.42, 0, 0.58, 1),
          -webkit-transform 0.3s cubic-bezier(0.42, 0, 0.58, 1);
      }
      .tap-target-content {
        position: relative;
        display: table-cell;
      }
      .tap-target-wave {
        position: absolute;
        border-radius: 50%;
        z-index: 10001;
      }
      .tap-target-wave::before,
      .tap-target-wave::after {
        content: "";
        display: block;
        position: absolute;
        width: 100%;
        height: 100%;
        border-radius: 50%;
        background-color: #ffffff;
      }
      .tap-target-wave::before {
        -webkit-transform: scale(0);
        transform: scale(0);
        -webkit-transition: -webkit-transform 0.3s;
        transition: -webkit-transform 0.3s;
        transition: transform 0.3s;
        transition: transform 0.3s, -webkit-transform 0.3s;
      }
      .tap-target-wave::after {
        visibility: hidden;
        -webkit-transition: opacity 0.3s, visibility 0s, -webkit-transform 0.3s;
        transition: opacity 0.3s, visibility 0s, -webkit-transform 0.3s;
        transition: opacity 0.3s, transform 0.3s, visibility 0s;
        transition: opacity 0.3s, transform 0.3s, visibility 0s,
          -webkit-transform 0.3s;
        z-index: -1;
      }
      .tap-target-origin {
        top: 50%;
        left: 50%;
        -webkit-transform: translate(-50%, -50%);
        transform: translate(-50%, -50%);
        z-index: 10002;
        position: absolute !important;
      }
      .tap-target-origin:not(.btn):not(.btn-large):not(.btn-small),
      .tap-target-origin:not(.btn):not(.btn-large):not(.btn-small):hover {
        background: none;
      }
      @media only screen and (max-width: 600px) {
        .tap-target,
        .tap-target-wrapper {
          width: 600px;
          height: 600px;
        }
      }
      .pulse {
        overflow: visible;
        position: relative;
      }
      .pulse::before {
        content: "";
        display: block;
        position: absolute;
        width: 100%;
        height: 100%;
        top: 0;
        left: 0;
        background-color: inherit;
        border-radius: inherit;
        -webkit-transition: opacity 0.3s, -webkit-transform 0.3s;
        transition: opacity 0.3s, -webkit-transform 0.3s;
        transition: opacity 0.3s, transform 0.3s;
        transition: opacity 0.3s, transform 0.3s, -webkit-transform 0.3s;
        -webkit-animation: pulse-animation 1s cubic-bezier(0.24, 0, 0.38, 1)
          infinite;
        animation: pulse-animation 1s cubic-bezier(0.24, 0, 0.38, 1) infinite;
        z-index: -1;
      }
      @-webkit-keyframes pulse-animation {
        0% {
          opacity: 1;
          -webkit-transform: scale(1);
          transform: scale(1);
        }
        50% {
          opacity: 0;
          -webkit-transform: scale(1.5);
          transform: scale(1.5);
        }
        100% {
          opacity: 0;
          -webkit-transform: scale(1.5);
          transform: scale(1.5);
        }
      }
      @keyframes pulse-animation {
        0% {
          opacity: 1;
          -webkit-transform: scale(1);
          transform: scale(1);
        }
        50% {
          opacity: 0;
          -webkit-transform: scale(1.5);
          transform: scale(1.5);
        }
        100% {
          opacity: 0;
          -webkit-transform: scale(1.5);
          transform: scale(1.5);
        }
      }
      .datepicker-modal {
        max-width: 325px;
        min-width: 300px;
        max-height: none;
      }
      .datepicker-container.modal-content {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-orient: vertical;
        -webkit-box-direction: normal;
        -webkit-flex-direction: column;
        -ms-flex-direction: column;
        flex-direction: column;
        padding: 0;
      }
      .datepicker-controls {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-pack: justify;
        -webkit-justify-content: space-between;
        -ms-flex-pack: justify;
        justify-content: space-between;
        width: 280px;
        margin: 0 auto;
      }
      .datepicker-controls .selects-container {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
      }
      .datepicker-controls .select-wrapper input {
        border-bottom: none;
        text-align: center;
        margin: 0;
      }
      .datepicker-controls .select-wrapper input:focus {
        border-bottom: none;
      }
      .datepicker-controls .select-wrapper .caret {
        display: none;
      }
      .datepicker-controls .select-year input {
        width: 50px;
      }
      .datepicker-controls .select-month input {
        width: 70px;
      }
      .month-prev,
      .month-next {
        margin-top: 4px;
        cursor: pointer;
        background-color: transparent;
        border: none;
      }
      .datepicker-date-display {
        -webkit-box-flex: 1;
        -webkit-flex: 1 auto;
        -ms-flex: 1 auto;
        flex: 1 auto;
        background-color: #26a69a;
        color: #fff;
        padding: 20px 22px;
        font-weight: 500;
      }
      .datepicker-date-display .year-text {
        display: block;
        font-size: 1.5rem;
        line-height: 25px;
        color: rgba(255, 255, 255, 0.7);
      }
      .datepicker-date-display .date-text {
        display: block;
        font-size: 2.8rem;
        line-height: 47px;
        font-weight: 500;
      }
      .datepicker-calendar-container {
        -webkit-box-flex: 2.5;
        -webkit-flex: 2.5 auto;
        -ms-flex: 2.5 auto;
        flex: 2.5 auto;
      }
      .datepicker-table {
        width: 280px;
        font-size: 1rem;
        margin: 0 auto;
      }
      .datepicker-table thead {
        border-bottom: none;
      }
      .datepicker-table th {
        padding: 10px 5px;
        text-align: center;
      }
      .datepicker-table tr {
        border: none;
      }
      .datepicker-table abbr {
        text-decoration: none;
        color: #999;
      }
      .datepicker-table td {
        border-radius: 50%;
        padding: 0;
      }
      .datepicker-table td.is-today {
        color: #26a69a;
      }
      .datepicker-table td.is-selected {
        background-color: #26a69a;
        color: #fff;
      }
      .datepicker-table td.is-outside-current-month,
      .datepicker-table td.is-disabled {
        color: rgba(0, 0, 0, 0.3);
        pointer-events: none;
      }
      .datepicker-day-button {
        background-color: transparent;
        border: none;
        line-height: 38px;
        display: block;
        width: 100%;
        border-radius: 50%;
        padding: 0 5px;
        cursor: pointer;
        color: inherit;
      }
      .datepicker-day-button:focus {
        background-color: rgba(43, 161, 150, 0.25);
      }
      .datepicker-footer {
        width: 280px;
        margin: 0 auto;
        padding-bottom: 5px;
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-pack: justify;
        -webkit-justify-content: space-between;
        -ms-flex-pack: justify;
        justify-content: space-between;
      }
      .datepicker-cancel,
      .datepicker-clear,
      .datepicker-today,
      .datepicker-done {
        color: #26a69a;
        padding: 0 1rem;
      }
      .datepicker-clear {
        color: #f44336;
      }
      @media only screen and (min-width: 601px) {
        .datepicker-modal {
          max-width: 625px;
        }
        .datepicker-container.modal-content {
          -webkit-box-orient: horizontal;
          -webkit-box-direction: normal;
          -webkit-flex-direction: row;
          -ms-flex-direction: row;
          flex-direction: row;
        }
        .datepicker-date-display {
          -webkit-box-flex: 0;
          -webkit-flex: 0 1 270px;
          -ms-flex: 0 1 270px;
          flex: 0 1 270px;
        }
        .datepicker-controls,
        .datepicker-table,
        .datepicker-footer {
          width: 320px;
        }
        .datepicker-day-button {
          line-height: 44px;
        }
      }
      .timepicker-modal {
        max-width: 325px;
        max-height: none;
      }
      .timepicker-container.modal-content {
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-orient: vertical;
        -webkit-box-direction: normal;
        -webkit-flex-direction: column;
        -ms-flex-direction: column;
        flex-direction: column;
        padding: 0;
      }
      .text-primary {
        color: #fff;
      }
      .timepicker-digital-display {
        -webkit-box-flex: 1;
        -webkit-flex: 1 auto;
        -ms-flex: 1 auto;
        flex: 1 auto;
        background-color: #26a69a;
        padding: 10px;
        font-weight: 300;
      }
      .timepicker-text-container {
        font-size: 4rem;
        font-weight: bold;
        text-align: center;
        color: rgba(255, 255, 255, 0.6);
        font-weight: 400;
        position: relative;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      .timepicker-span-hours,
      .timepicker-span-minutes,
      .timepicker-span-am-pm div {
        cursor: pointer;
      }
      .timepicker-span-hours {
        margin-right: 3px;
      }
      .timepicker-span-minutes {
        margin-left: 3px;
      }
      .timepicker-display-am-pm {
        font-size: 1.3rem;
        position: absolute;
        right: 1rem;
        bottom: 1rem;
        font-weight: 400;
      }
      .timepicker-analog-display {
        -webkit-box-flex: 2.5;
        -webkit-flex: 2.5 auto;
        -ms-flex: 2.5 auto;
        flex: 2.5 auto;
      }
      .timepicker-plate {
        background-color: #eee;
        border-radius: 50%;
        width: 270px;
        height: 270px;
        overflow: visible;
        position: relative;
        margin: auto;
        margin-top: 25px;
        margin-bottom: 5px;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      .timepicker-canvas,
      .timepicker-dial {
        position: absolute;
        left: 0;
        right: 0;
        top: 0;
        bottom: 0;
      }
      .timepicker-minutes {
        visibility: hidden;
      }
      .timepicker-tick {
        border-radius: 50%;
        color: rgba(0, 0, 0, 0.87);
        line-height: 40px;
        text-align: center;
        width: 40px;
        height: 40px;
        position: absolute;
        cursor: pointer;
        font-size: 15px;
      }
      .timepicker-tick.active,
      .timepicker-tick:hover {
        background-color: rgba(38, 166, 154, 0.25);
      }
      .timepicker-dial {
        -webkit-transition: opacity 350ms, -webkit-transform 350ms;
        transition: opacity 350ms, -webkit-transform 350ms;
        transition: transform 350ms, opacity 350ms;
        transition: transform 350ms, opacity 350ms, -webkit-transform 350ms;
      }
      .timepicker-dial-out {
        opacity: 0;
      }
      .timepicker-dial-out.timepicker-hours {
        -webkit-transform: scale(1.1, 1.1);
        transform: scale(1.1, 1.1);
      }
      .timepicker-dial-out.timepicker-minutes {
        -webkit-transform: scale(0.8, 0.8);
        transform: scale(0.8, 0.8);
      }
      .timepicker-canvas {
        -webkit-transition: opacity 175ms;
        transition: opacity 175ms;
      }
      .timepicker-canvas line {
        stroke: #26a69a;
        stroke-width: 4;
        stroke-linecap: round;
      }
      .timepicker-canvas-out {
        opacity: 0.25;
      }
      .timepicker-canvas-bearing {
        stroke: none;
        fill: #26a69a;
      }
      .timepicker-canvas-bg {
        stroke: none;
        fill: #26a69a;
      }
      .timepicker-footer {
        margin: 0 auto;
        padding: 5px 1rem;
        display: -webkit-box;
        display: -webkit-flex;
        display: -ms-flexbox;
        display: flex;
        -webkit-box-pack: justify;
        -webkit-justify-content: space-between;
        -ms-flex-pack: justify;
        justify-content: space-between;
      }
      .timepicker-clear {
        color: #f44336;
      }
      .timepicker-close {
        color: #26a69a;
      }
      .timepicker-clear,
      .timepicker-close {
        padding: 0 20px;
      }
      @media only screen and (min-width: 601px) {
        .timepicker-modal {
          max-width: 600px;
        }
        .timepicker-container.modal-content {
          -webkit-box-orient: horizontal;
          -webkit-box-direction: normal;
          -webkit-flex-direction: row;
          -ms-flex-direction: row;
          flex-direction: row;
        }
        .timepicker-text-container {
          top: 32%;
        }
        .timepicker-display-am-pm {
          position: relative;
          right: auto;
          bottom: auto;
          text-align: center;
          margin-top: 1.2rem;
        }
      }
    </style>
    <!-- MATERIALIZE STYLES -->

    <!-- CUSTOM STYLES -->
    <style>
      .download-btn {
        margin-top: 30px;
      }

      #terminal {
        /* border: 2px solid red; */
        margin-top: 30px;
      }

      .mtop {
        margin-top: 20px;
      }

      .padding {
        padding: 8px;
      }

      .pleft {
        padding-left: 20px;
      }

      table tbody {
        display: block;
        max-height: 40vh;
        overflow: auto;
      }

      thead,
      tbody tr {
        display: table;
        width: 100%;
        table-layout: fixed;
      }

      h4 {
        margin: 0;
      }

      .tabs .indicator {
        background-color: #ff9100;
      }

      .tabs .tab a.active {
        color: #ff6d00;
      }

      textarea {
        cursor: none;
      }

      .collection {
        max-height: 40vh;
        overflow: auto;
      }

      .tabs .tab a {
        font-weight: bold;
        color: #9e9e9e;
      }
      .tabs .tab a.active {
        color: #424242;
      }

      /* label focus color */
      .input-field input[type="text"]:focus + label {
        color: #ff9100 !important;
      }
      /* label underline focus color */
      .input-field input[type="text"]:focus {
        border-bottom: 1px solid #ff9100 !important;
        box-shadow: 0 1px 0 0 #ff9100 !important;
      }

      /* icon prefix focus color */
      .input-field .prefix.active {
        color: #ff9100;
      }

      .modal-trigger {
        position: relative;
        top: 20px;
      }

      .infoCircle {
        position: relative;
        top: 8px;
      }

      .infoCircle:hover path {
        fill: #2bbbad;
        cursor: pointer;
      }
      .infoCircle:active path {
        fill: #8bdad2;
        cursor: pointer;
      }

      .carousel .indicators .indicator-item {
        background-color: rgba(253, 216, 53, 0.4);
      }
      .carousel .indicators .indicator-item.active {
        background-color: rgb(255, 145, 0);
      }

      .card-image {
        display: flex !important;
        justify-content: center !important;
        align-items: center;
        padding: 40px;
        max-width: 100%;
      }

      @media screen and (max-width: 480px) {
        .tabSelectorBottom {
          position: fixed;
          width: 100vw;
          z-index: 1;
          bottom: 0px;
        }
      }
    </style>
    <!-- CUSTOM STYLES -->

    <title>OASYS</title>
  </head>

  <body class="grey lighten-5">
    <!-- HEADER -->
    <nav>
      <div class="nav-wrapper yellow accent-4">
        <div class="container">
          <a href="/" class="center brand-logo grey-text text-darken-3">
            Oasys
          </a>
        </div>
      </div>
    </nav>
    <!-- HEADER -->

    <!--  TAB SELECTOR: [STATS] [FILES] [TERMINAL]  -->
    <div class="tabSelectorBottom yellow accent-4">
      <div class="container">
        <ul class="tabs tabs-fixed-width yellow accent-4">
          <li class="tab"><a class="active" href="#info">Info</a></li>
          <li class="tab"><a href="#terminal">Terminal</a></li>
          <li class="tab"><a href="#files">Files</a></li>
        </ul>
      </div>
    </div>
    <!-- TAB SELECTOR -->

    <!-- INFO TAB -->
    <div class="container mtop" id="info">
      <div class="row">
        <div class="col s12 m6 l4">
          <div class="card yellow darken-2">
            <div class="card-image glider">
              <img
                src="data:image/png;base64,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"
                alt=""
              />
            </div>
            <div class="card-content black-text">
              <span class="card-title black-text">G2</span>
              <p>
                Lorem ipsum dolor sit amet consectetur adipisicing elit. Quaerat
                sapiente quasi iure ea sequi a doloribus ratione accusamus, esse
                necessitatibus.
              </p>
            </div>
          </div>
        </div>

        <div class="col s4 m6 l8">
          <div class="card yellow darken-3">
            <div class="card-content white-text">
              <span class="card-title">Some info</span>
              <p>Lorem ipsum dolor sit amet.</p>
            </div>
          </div>
        </div>
        <div class="col s8 m6 l4">
          <div class="card yellow darken-4">
            <div class="card-content white-text">
              <span class="card-title">Moar info</span>
              <p>
                Lorem ipsum dolor sit amet consectetur adipisicing elit. Nihil
                amet adipisci unde fugit! Deleniti libero numquam eligendi.
                Repellendus officia quas nam vitae incidunt fuga inventore dicta
                placeat blanditiis, veniam labore!
              </p>
            </div>
          </div>
        </div>
        <div class="col s6 m7 l4">
          <div class="card yellow darken-5">
            <div class="card-content black-text">
              <span class="card-title">Some info</span>
              <p>Lorem ipsum dolor sit amet.</p>
            </div>
          </div>
        </div>
        <div class="col s6 m5 l4">
          <div class="card yellow accent-4">
            <div class="card-content black-text">
              <span class="card-title">Moar info</span>
              <p>
                Lorem ipsum dolor, sit amet consectetur adipisicing elit.
                Fugiat, ipsam.
              </p>
            </div>
          </div>
        </div>
      </div>
      <!-- row -->
    </div>
    <!-- INFO TAB -->

    <!-- TERMINAL TAB -->
    <div class="container mtop" id="terminal">
      <div class="row">
        <div class="col s10">
          <form>
            <div class="input-field">
              <!-- <i class="material-icons prefix">mode</i> -->
              <!-- TODO: create validation, add class invalid or valid on input-->
              <input id="commands" type="text" class="" />
              <label for="commands">Insert command</label>
              <span
                class="helper-text"
                data-error="Invalid command"
                data-success="Command sent"
              >
                command format is a key value pair separated by space
              </span>
            </div>
          </form>
        </div>

        <div class="col s2">
          <!-- Info button, opens modal -->
          <a class="modal-trigger" data-target="commandModal">
            <svg
              class="infoCircle"
              width="31"
              height="31"
              viewBox="0 0 31 31"
              fill="none"
            >
              <path
                d="M15.3334 0.333252C7.05337 0.333252 0.333374 7.05325 0.333374 15.3333C0.333374 23.6133 7.05337 30.3333 15.3334 30.3333C23.6134 30.3333 30.3334 23.6133 30.3334 15.3333C30.3334 7.05325 23.6134 0.333252 15.3334 0.333252ZM16.8334 22.8333H13.8334V13.8333H16.8334V22.8333ZM16.8334 10.8333H13.8334V7.83325H16.8334V10.8333Z"
                fill="#26A69A"
              />
            </svg>
          </a>
        </div>
      </div>

      <!-- COMMAND MODAL -->
      <div id="commandModal" class="modal">
        <div class="modal-content">
          <h4>Valid commands</h4>
          <p>Some commands...</p>
        </div>
        <div class="modal-footer">
          <a class="modal-close waves-effect waves-green btn-flat">Close</a>
        </div>
      </div>

      <!-- LIST OF COMMANDS SENT -->
      <table class="striped centered">
        <thead class="table-header yellow darken-2">
          <tr>
            <th style="font-size: 1.2rem">Commands sent</th>
          </tr>
        </thead>

        <tbody class="tbodyCommands">
          <tr>
            <td>T 20</td>
          </tr>
          <tr>
            <td>P 4</td>
          </tr>
        </tbody>
      </table>
    </div>
    <!-- TERMINAL TAB -->

    <!-- FILES TAB -->
    <div class="container mtop" id="files">
      <!-- CARDS: file data -->
      <div class="row">
        <div class="col s6">
          <div class="card yellow darken-1 center">
            <div class="card-content grey-text text-darken-3">
              <span class="card-title">Files</span>
              <h4>20</h4>
            </div>
          </div>
        </div>
        <div class="col s6">
          <div class="card yellow darken-1 center">
            <div class="card-content grey-text text-darken-3">
              <span class="card-title">Size [mB]</span>
              <h4>2349</h4>
            </div>
          </div>
        </div>
      </div>

      <!-- TABLE -->
      <table class="striped centered">
        <thead class="table-header yellow darken-2">
          <tr>
            <th>Filename [Date]</th>
            <th>Size [kB]</th>
          </tr>
        </thead>

        <tbody class="tbodyFiles"></tbody>
      </table>

      <!-- DOWNLOAD BUTTON -->
      <div class="center">
        <a class="download-btn waves-effect waves-light btn">
          generate & download
          <i class="left">
            <svg
              width="14"
              height="17"
              viewBox="0 0 14 17"
              fill="none"
              xmlns="http://www.w3.org/2000/svg"
            >
              <path
                d="M0.583374 16.3333H13.4167V14.5H0.583374V16.3333ZM13.4167 6.25H9.75004V0.75H4.25004V6.25H0.583374L7.00004 12.6667L13.4167 6.25Z"
                fill="white"
              />
            </svg>
          </i>
        </a>
      </div>

      <!-- PROGRESS BAR -->
      <div class="row valign-wrapper hide prog-bar mtop">
        <div class="col s10">
          <div class="progress mtop">
            <div class="determinate" style="width: 20%"></div>
          </div>
        </div>
        <div class="col s2">
          <span class="new badge" data-badge-caption="">20%</span>
        </div>
      </div>
    </div>
    <!-- FILES TAB-->
)====";
