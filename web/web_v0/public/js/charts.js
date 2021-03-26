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
      ctx.strokeStyle = "rgba(245, 92, 56, 0.5)";
      ctx.stroke();
      ctx.restore();
    }
  },
});

Chart.defaults.global.defaultFontFamily = "'Montserrat'";

// DUMMY DATA

const dummyData2 = createGraphData();
const dummyDate = "2021-03-20";

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
const dataT = parseFirestoreData();

// DUMMY DATA END

// Helper functions

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

// button configurators

// configures download of data to xlsx-file
function createDownloadBtns(btn, chart) {
  btn.addEventListener("click", () => {
    if (chart.data.datasets[0].data.length != 0) {
      downloadExcel(
        activeMission,
        chart.data.datasets[0].label,
        chart.data.datasets[0].data
      );
    }
  });
}

// configures zoom reset for chart
function createResetZoomBtns(btn, chart) {
  btn.addEventListener("click", () => {
    chart.resetZoom();
  });
}

// configures date range functionality
function createRangeBtns(btn, content) {
  // Clicking dateranger shows, dropdown content
  btn.addEventListener("click", () => {
    content.classList.toggle("hidden");
    btn.classList.toggle("activeRange");
  });

  // Clicking elsewhere, hides dropdown content
  window.addEventListener("click", (e) => {
    if (e.target != btn && e.target.parentNode != btn) {
      content.classList.add("hidden");
      btn.classList.remove("activeRange");
    }
  });
}

// Fetch data within a specified timespan
function filterData(chart, type, days) {
  let datetimeNow = moment().format("YYYY-MM-DD[T]HH:mm:ss");
  let datetimePast = moment(datetimeNow)
    .subtract(days, "days")
    .format("YYYY-MM-DD[T]HH:mm:ss");
  let dataset = [];
  missionDataset[activeMission][type].forEach((row) => {
    if (datetimePast <= row.t && row.t <= datetimeNow) {
      dataset.push(row);
    }
  });
  chart.data.datasets[0].data = dataset;
  chart.update();
  chart.resetZoom();
}

// Main functions

// Creates HTML for chart.
// **HTML NEEDS TO BE CREATED FOR ALL CHARTS BEFORE ADDING CHART FUNCTIONALITY**
function createChartHTML(type) {
  let chartHTML = `
      <div class="chartCard">
        <div class="flex items-center w-full h-12 mt-2 px-1">
          <!-- Chart header -->
          <div class="flex-grow">
            <h1 class="font-medium text-lg tracking-tight ml-2 opacity-75">
              ${type}
            </h1>
          </div> 

          <!-- Chart buttons -->
          <div class="graphButtons flex space-x-2">
            
            <!-- download btn -->
            <div id="downloadBtn${type}" class="chartBtn">
              <span class="material-icons "> download </span>
            </div>

            <!-- reset zoom -->
            <div id="resetBtn${type}" class="chartBtn">
              <span class="material-icons "> youtube_searched_for </span>
            </div>

            <!-- chart ranger -->
            <div
              id="chartRange"
              class="hidden flex items-center h-full text-gray-800 tracking-wider"
            > 
              <div
                id="rangeBtn${type}"
                class="rangeBtnActive cursor-pointer relative border px-2 rounded-md h-8 text-left text-xs flex items-center justfiy-start shadow-sm hover:border-gray-800 transform hover:scale-110 duration-200"
                style="width: 56px;"
              >
                <span class="material-icons">
                  calendar_today
                </span>
                <i class="ml-1 rangeBtnActive bi bi-chevron-down"></i>
                <div id="rangeContent${type}" class="rangeContent hidden">
                  <div class="chartDropElement border-b">All</div>
                  <div class="chartDropElement border-b">Last 24 hours</div>
                  <div class="chartDropElement border-b">Last 7 days</div>
                  <div class="chartDropElement">Last 30 days</div>
                </div>
              </div>
            </div>

            <!-- chart settings -->
            <div id="settingsBtn${type}" class="chartBtn">
              <span class="material-icons"> settings </span>
            </div>
          </div>
        </div>

        <div class="h-4/5">
          <canvas id="chart${type}" class="tracking-tight"></canvas>
        </div>
      </div>
    `;

  document.querySelector("#chartGrid").innerHTML += chartHTML;
}

// Configures variables for appearance of chart
function createChartContent(type, ylabel, color, stepSize) {
  let chartContent = {
    type: "LineWithLine",
    data: {
      datasets: [
        {
          pointBorderColor: `${color},0.8)`,
          pointRadius: 2,
          label: type,
          lineTension: 0, // turns off interpolation (smooth graph)
          data: [],
          backgroundColor: `${color},0.25)`,
          borderColor: `${color},0.5)`,
          fill: false,
        },
      ],
    },
    options: {
      plugins: {
        zoom: {
          pan: {
            enabled: true,
            mode: "x",
          },
          zoom: {
            enabled: true,
            mode: "x",
          },
        },
      },
      tooltips: {
        intersect: false,
        axis: "x",

        xPadding: 10,
        yPadding: 10,

        backgroundColor: "rgba(249, 250, 251, 1)",
        titleFontColor: "rgba(31, 41, 55, 1)",
        bodyFontColor: "rgba(31, 41, 55, 1)",
        bodyFontStyle: "bold",
        footerFontColor: "rgba(31, 41, 55, 1)",
        borderColor: "rgba(31, 41, 55, 1)",
        borderWidth: 1,
      },
      scales: {
        xAxes: [
          {
            scaleLabel: {
              display: true,
              labelString: "Time",
            },
            bounds: "ticks",
            type: "time",
            time: {
              unitStepSize: 2,
              displayFormats: {
                minute: "HH:mm",
                hour: "HH:mm",
              },
              tooltipFormat: "YYYY-MM-DD   HH:mm:ss",
            },
            distribution: "linear",
            ticks: {
              source: "auto",
            },
          },
        ],
        yAxes: [
          {
            scaleLabel: {
              display: true,
              labelString: ylabel,
            },
            ticks: {
              // beginAtZero: true,
              stepSize: stepSize,
            },
          },
        ],
      },
      maintainAspectRatio: false,
      responsiveAnimationDuration: 1000,
      legend: {
        display: false,
      },
    },
  };

  return chartContent;
}

// Creates and fetches data for chart, adds functionality to buttons
// **ONLY RUN WHEN ALL HTML FILES ARE CREATED**
function createChartAndFunctions(name, dataType, ylabel, color, stepSize) {
  let chartObject = {
    ctx: document.querySelector(`#chart${name}`).getContext("2d"),
    content: createChartContent(name, ylabel, color, stepSize),
    downloadBtn: document.querySelector(`#downloadBtn${name}`),
    resetZoom: document.querySelector(`#resetBtn${name}`),
    rangeBtn: document.querySelector(`#rangeBtn${name}`),
    rangeContent: document.querySelector(`#rangeContent${name}`),
    chartDropElement: document.querySelectorAll(
      `#rangeContent${name} .chartDropElement`
    ),
  };

  let chart = new Chart(chartObject.ctx, chartObject.content);

  createRangeBtns(chartObject.rangeBtn, chartObject.rangeContent);
  createResetZoomBtns(chartObject.resetZoom, chart);
  createDownloadBtns(chartObject.downloadBtn, chart);

  // All
  chartObject.chartDropElement[0].addEventListener("click", () => {
    chart.data.datasets[0].data = missionDataset[activeMission][dataType];
    chart.update();
    chart.resetZoom();
  });

  // 24 Hours
  chartObject.chartDropElement[1].addEventListener("click", () => {
    filterData(chart, dataType, 1);
  });

  // 7 Days
  chartObject.chartDropElement[2].addEventListener("click", () => {
    filterData(chart, dataType, 7);
  });
  // 30 Days
  chartObject.chartDropElement[3].addEventListener("click", () => {
    filterData(chart, dataType, 30);
  });

  return {type: dataType, chart: chart};
}

// Creates all the charts and adds to array of charts (may not be necessary?)
// currently includes: Conductivity, Pressure, Temperature
function createAllCharts() {
  createChartHTML("Conductivity");
  createChartHTML("Pressure");
  createChartHTML("Temperature");

  const chartConductivity = createChartAndFunctions(
    "Conductivity",
    "C",
    "Conductivity  S/m",
    "rgba(255, 108, 58",
    2
  );

  const chartPressure = createChartAndFunctions(
    "Pressure",
    "P",
    "Pressure MPa",
    "rgba(245, 186, 97",
    1
  );

  const chartTemperature = createChartAndFunctions(
    "Temperature",
    "T",
    "Temperature \xB0 C",
    "rgba(136, 174, 204",
    4
  );

  charts.push(chartConductivity);
  charts.push(chartPressure);
  charts.push(chartTemperature);
}
