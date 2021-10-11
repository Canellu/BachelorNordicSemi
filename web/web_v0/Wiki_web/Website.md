# Website

The website is written with the standard web stack (HTML/CSS/Javascript) but uses Tailwind CSS for styling. Any relevant javascript libraries will be mentioned along the way.

Some of the most used documentations are shown below:

Google Maps:

https://developers.google.com/maps/documentation/javascript/overview

Tailwind CSS:

https://tailwindcss.com/




## Main Page

The main page contains basic information about all the devices that have been added to the project. It makes use of the Google Maps API to display the GPS coordinates of the devices. The devices are displayed as cards with some basic information.

Selecting any of the cards will open up a page containing more details about the device. This will be elaborated upon below.

![](./figures/main_cards.png)

### Fetching data

Whenever the home page loads, an async request is sent to firebase for the "Gliders" collection. Fields from each glider is collected to be used for displaying information for each card. At the same time, the **last seen** field is used for placing the marker on the map.

### Note!

Each read per field counts towards a quota in firebase. Caution is therefore advised when refreshing the page many times.

![](./figures/main_cards_fb.png)


## Device Page

The device page contains more information about the device. This includes the data that it has collected. These are displayed in graphs. The coordinates for where the device has been is also shown in a map as can be seen at the top of the page.

The device is specified using URL parameters, as can be seen below:
```
device.html?gliderUID=039189
```

### Fetching data
Certain measures were taken to try reducing the number of reads from Firebase as much as possible.

A request is sent to firebase much like in the homepage. This request reads the names of all the missions (referred to as **documents** in firebase) present within the glider. This only fetches the names, not the data stored in each document as to reduce reads.

![](./figures/device_list.png)

Data per mission is only fetched when a mission is chosen from the list, which prompts another request from Firebase. Data that has been read will remain saved until the page is refreshed. 

Data is saved as an array of JSON objects. The structure can be seen below:
```*.jsx
dataset = [obj1, obj2, ...];

obj1 structure:
"{date}T{time}": {data}

data structure:
data = {
        "dataType1": val1,
        "dataType2": val2,
        "dataType3": val3
       }
```

Example:
```*.jsx
dataset = 
[
  {
    "2020-03-28T02:24:12":
    {
      "T": 2,
      "P": 1,
      "C": 5,
    }
  },
  {...},
  ...
];
```

This dataset is then sorted chronologically.

It is also possible to view data over a custom date range, instead of a per-mission basis.

![](./figures/device_date.png)

This works by going through the dataset, the ones in the website and the ones in Firebase, and checking whether the starting or ending date is present within the range of the mission. Any missions in-between the mission sets will be included. Once the relevant missions have been chosen, the dataset is sifted through to find the data that is within the actual date range.

![](./figures/device_date_picking.png)

This is essentially a 2-step process. Select all relevant missions, then select the specific values within those missions.

### Preparing the data for the graphs
The dataset collected cannot be used yet for plotting the graphs. Not all datatypes are present within each timestamp. Therefore, individual sets need to be created for each datatype.

A function has been made to find out different data types. In theory it should make it easier to add new types of data to the project. Referring to the example, the program will find the datatypes **T, P, C**. The program will then go through the entire dataset and save any values containing the datatype to the appropriate array.

The new dataset will therefore look like this. Notice the difference in structure for **coordinates**:

```*.jsx
dataObj =
{
  T:
  [
    {
      t:"2020-03-28T02:24:12",
      y: 2
    }, ...
  ],
  P:[...],
  C:[...],
  coordinates:
  [
    {
      t:"2020-03-28T02:24:12",
      lat: 60,
      lng: 11
    }, ...
  ]
}
```

### Google Maps API

The maps need to be initialized before displaying any content. An API key is necessary before using the Google Maps API for javascript.

The waypoints can be displayed directly using the **coordinates** array created in the previous section. Lines are created between points using the [polyline function](https://developers.google.com/maps/documentation/javascript/examples/polyline-simple) within the google maps API. 

Custom icons have also been added for each marker, with the first and last markers having a more distinct appearance.

```
// Add new markers to marker-array
let firstMarker;
coordinates.forEach((loc, index) => {
  let icon = "assets/svg/triangleMarker.svg";

  // If start give icon, if end, give another
  switch (index) {
    case 0:
      firstMarker = { lat: loc.lat, lng: loc.lng };
      icon = "assets/svg/startMarkerWhite.svg";
      break;
    case coordinates.length - 1:
      icon = "assets/svg/endMarkerWhite.svg";
      break;
  }

  addMarker(loc, icon, index, map);
});
```

### Graphs: Chart JS 2.9.4

https://www.chartjs.org/docs/2.9.4/

Chartjs has been used for displaying the graphs, which plot the value with respect to time. A custom function has also been implemented to allow a vertical line to select a specific point on the graph. This is not a built-in function of Chartjs (as per the version used) and is a custom addon written into the code.

```*.jsx
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

      // draw vertical line
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
```

![](./figures/device_graph.png)

Additionally, it is possible to pan around the graph and download the dataset as an .xlsx file. These are also not built-in within Chartjs, and have been added with the [Hammerjs library](https://hammerjs.github.io/).


## MIssion control

Mission control allows for creation of mission parameters to be sent to the glider. The map is interactive and lets the user add and edit waypoints directly. The users can also change it directly through the table.

### Map - extra functions

Interacting and adding waypoints were created using [polylines](https://developers.google.com/maps/documentation/javascript/examples/polyline-simple). It has been modified to allow adding, moving and deleting of waypoints, using [this](https://developers.google.com/maps/documentation/javascript/examples/delete-vertex-menu) as an example. 

![image of map and waypoint table](./figures/device_maps.png)

The new modifications make sure that the accompanying table of coordinates updates whenever changes are done on the map and vice versa. These are added as event listeners within google maps.

This code snippet shows one of the event listeners added to the code whenever a waypoint is dragged:

```*.jsx
google.maps.event.addListener(
  missionWaypoints,
  "dragend",
  createPreviewWaypoints
);

// createPreviewWaypoints is the function that updates the coordinates table.
```


### Sliders and input fields

Most of the sliders and input fields are made using noUISlider. 