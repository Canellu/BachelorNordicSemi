// THIS SECTION IS EXPERIMENTAL
Chart.defaults.LineWithLine = Chart.defaults.line;
Chart.controllers.LineWithLine = Chart.controllers.line.extend({
  draw: function (ease) {
    Chart.controllers.line.prototype.draw.call(this, ease);

    if (this.chart.tooltip._active && this.chart.tooltip._active.length) {
      var activePoint = this.chart.tooltip._active[0],
        ctx = this.chart.ctx,
        x = activePoint.tooltipPosition().x,
        topY = this.chart.scales["y-axis-0"].top,
        bottomY = this.chart.scales["y-axis-0"].bottom;

      // draw line
      ctx.save();
      ctx.beginPath();
      ctx.moveTo(x, topY);
      ctx.lineTo(x, bottomY);
      ctx.lineWidth = 2;
      ctx.strokeStyle = "rgba(50, 170, 170, 0.5)";
      ctx.stroke();
      ctx.restore();
    }
  },
});

Chart.defaults.global.defaultFontFamily = "'Montserrat'";

var ctx = document.getElementById("myChart");
var ctx1 = document.getElementById("myChart1");
var ctx2 = document.getElementById("myChart2");

var myChart = new Chart(ctx, {
  type: "doughnut",
  data: {
    labels: ["Red", "Blue", "Yellow", "Green", "Purple", "Orange"],
    datasets: [
      {
        label: "# of Votes",
        data: [12, 19, 3, 5, 2, 3],
        backgroundColor: [
          "rgba(255, 99, 132, 0.2)",
          "rgba(54, 162, 235, 0.2)",
          "rgba(255, 206, 86, 0.2)",
          "rgba(75, 192, 192, 0.2)",
          "rgba(153, 102, 255, 0.2)",
          "rgba(255, 159, 64, 0.2)",
        ],
        borderColor: [
          "rgba(255, 99, 132, 1)",
          "rgba(54, 162, 235, 1)",
          "rgba(255, 206, 86, 1)",
          "rgba(75, 192, 192, 1)",
          "rgba(153, 102, 255, 1)",
          "rgba(255, 159, 64, 1)",
        ],
        borderWidth: 2,
      },
    ],
  },
  options: {
    legend: {
      position: "right",
      labels: {
        padding: 24,
      },
    },
    // scales: {
    //   yAxes: [
    //     {
    //       ticks: {
    //         beginAtZero: true,
    //       },
    //     },
    //   ],
    // },
    maintainAspectRatio: false,
  },
});

var myChart1 = new Chart(ctx1, {
  type: "line",
  data: {
    labels: [],
    datasets: [
      {
        label: "# of Votes",
        data: [12, 19, 3, 5, 2, 3],
        backgroundColor: [
          "rgba(255, 99, 132, 0.2)",
          "rgba(54, 162, 235, 0.2)",
          "rgba(255, 206, 86, 0.2)",
          "rgba(75, 192, 192, 0.2)",
          "rgba(153, 102, 255, 0.2)",
          "rgba(255, 159, 64, 0.2)",
        ],
        borderColor: [
          "rgba(255, 99, 132, 1)",
          "rgba(54, 162, 235, 1)",
          "rgba(255, 206, 86, 1)",
          "rgba(75, 192, 192, 1)",
          "rgba(153, 102, 255, 1)",
          "rgba(255, 159, 64, 1)",
        ],
        borderWidth: 2,
      },
    ],
  },
  options: {
    title: {
      display: true,
      text: "Temperature",
      position: "top", //default, also left, right, bottom
      fontSize: 20, // 12 default
      fontColor: "#34455D", // #666 default
      padding: 14, //default 10
      lineHeight: 1.6, //default 1.2
    },
    plugins: {
      zoom: {
        // Container for pan options
        pan: {
          // Boolean to enable panning
          enabled: true,

          // Panning directions. Remove the appropriate direction to disable
          // Eg. 'y' would only allow panning in the y direction
          mode: "x",
        },

        // Container for zoom options
        zoom: {
          // Boolean to enable zooming
          enabled: true,

          // Zooming directions. Remove the appropriate direction to disable
          // Eg. 'y' would only allow zooming in the y direction
          mode: "x",
        },
      },
    },
    scales: {
      xAxes: [
        {
          ticks: {
            source: "data",
          },
        },
      ],
      yAxes: [
        {
          ticks: {
            beginAtZero: true,
          },
        },
      ],
    },
    maintainAspectRatio: false,
    responsiveAnimationDuration: 500,
    legend: {
      display: false,
    },
  },
});

var myChart2 = new Chart(ctx2, {
  type: "bar",
  data: {
    labels: ["Red", "Blue", "Yellow", "Green", "Purple", "Orange"],
    datasets: [
      {
        label: "# of Votes",
        data: [12, 19, 3, 5, 2, 3],
        backgroundColor: [
          "rgba(255, 99, 132, 0.2)",
          "rgba(54, 162, 235, 0.2)",
          "rgba(255, 206, 86, 0.2)",
          "rgba(75, 192, 192, 0.2)",
          "rgba(153, 102, 255, 0.2)",
          "rgba(255, 159, 64, 0.2)",
        ],
        borderColor: [
          "rgba(255, 99, 132, 1)",
          "rgba(54, 162, 235, 1)",
          "rgba(255, 206, 86, 1)",
          "rgba(75, 192, 192, 1)",
          "rgba(153, 102, 255, 1)",
          "rgba(255, 159, 64, 1)",
        ],
        borderWidth: 2,
      },
    ],
  },
  options: {
    scales: {
      yAxes: [
        {
          ticks: {
            beginAtZero: true,
          },
        },
      ],
    },
    maintainAspectRatio: false,
  },
});

// Needs to be changed?
// Key needs quotation marks and brackets around entire value
// Firestore needs to save as string, so string will be in JSON format
// {"00:00:16":"{"T":13.126, "P":3.927, "C":4.407}"}

const dummyDate = "2021-03-20";

const dummyData1 = [
  { "00:00:16": { T: 13.126, P: 3.927, C: 4.407 } },
  { "00:20:34": { T: 5.207, P: 2.711, C: 5.706 } },
  { "00:24:14": { T: 18.241, P: 3.488, C: 4.089 } },
];

// If time or date is less than 10, add 0.  09, 08 etc...
function pad(n) {
  return n < 10 ? "0" + n : n;
}

// Simple random number generator with min max parameters
function randNum(min, max) {
  return Math.floor(Math.random() * max) + min;
}

// Compares objects, used for sorting from lowest to highest value
function compare(a, b) {
  if (Object.keys(a) < Object.keys(b)) {
    return -1;
  }
  if (Object.keys(a) > Object.keys(b)) {
    return 1;
  }
  return 0;
}

// Create dummy data for graph
function createGraphData() {
  let dataArray = [];
  for (let i = 0; i < 100; i++) {
    let hour = randNum(0, 23);
    let min = randNum(0, 59);
    let second = randNum(0, 59);
    let timestamp = `${pad(hour)}:${pad(min)}:${pad(second)}`;
    let temp = randNum(0, 20000) / 1000;
    let pressure = randNum(0, 4000) / 1000;
    let conductivity = randNum(4000, 6000) / 1000;
    let jsonData = {
      [timestamp]: {
        T: temp,
        P: pressure,
        C: conductivity,
      },
    };

    dataArray.push(jsonData);
  }

  dataArray.sort(compare);

  return dataArray;
}

const dummyData2 = createGraphData();

// Convert "firestore" data into pairs that can be used in line graph,
// Currently only returns time - temperature pair
function parseFirestoreData() {
  let timeArr = [];
  let TemperatureArr = [];
  // let PressureArr = [];
  // let ConductivityArr = [];

  //console.log(dummyData2);

  dummyData2.forEach((timestamp) => {
    let keyVal = Object.keys(timestamp);
    // console.log(timestamp[keyVal].T);
    // console.log(keyVal);
    timeArr.push(`${dummyDate}T${keyVal}`);
    TemperatureArr.push(timestamp[keyVal].T);
    // PressureArr.push(timestamp[keyVal].P);
    // ConductivityArr.push(timestamp[keyVal].C);
  });

  let timeVarPair = [];
  for (let i = 0; i < timeArr.length; i++) {
    timeVarPair.push({ t: timeArr[i], y: TemperatureArr[i] });
  }

  return timeVarPair;
}

const firestoreCtx = document.getElementById("firestoreChart");

const dataT = parseFirestoreData();

const firestoreChart = new Chart(firestoreCtx, {
  type: "LineWithLine",
  data: {
    // Currently controls number of and location of ticks
    // labels: ["2021-03-20T00:00:00", "2021-03-21T00:00:00"],
    datasets: [
      {
        label: "Temperature",
        lineTension: 0, // turns off interpolation (smooth graph)
        data: dataT,
        // backgroundColor: "rgba(0,0,0,0.5)",
        borderColor: "rgba(50, 170, 170, 0.5)",
        fill: false,
      },
    ],
  },
  options: {
    plugins: {
      zoom: {
        // Container for pan options
        pan: {
          // Boolean to enable panning
          enabled: true,

          // Panning directions. Remove the appropriate direction to disable
          // Eg. 'y' would only allow panning in the y direction
          mode: "x",
        },

        // Container for zoom options
        zoom: {
          // Boolean to enable zooming
          enabled: true,

          // Zooming directions. Remove the appropriate direction to disable
          // Eg. 'y' would only allow zooming in the y direction
          mode: "x",
        },
      },
    },
    tooltips: {
      intersect: false,
      axis: "x",
    },
    scales: {
      xAxes: [
        {
          type: "time",
          time: {
            tooltipFormat: "YYYY-MM-DD HH:mm:ss",
          },
          distribution: "linear",
          ticks: {
            source: "data", // labels control ticks
          },
        },
      ],
      yAxes: [
        {
          ticks: {
            beginAtZero: true,
          },
        },
      ],
    },
    maintainAspectRatio: false,
  },
});
