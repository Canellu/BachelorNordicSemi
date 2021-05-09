let dataMap;
let missionMap;
let homeMap;
let missionWaypoints;
let markers = [];
let gliderPath;
let dataMapDiv = document.getElementById("dataMap");
let missionMapDiv = document.getElementById("missionMap");
let homeMapDiv = document.getElementById("homeMap");
var flightPlanCoordinates;

var defaultLocation = { lat: 59.769, lng: 10.654, zoom: 10 };
function initHomeMap() {
  homeMap = new google.maps.Map(homeMapDiv, {
    center: { lat: defaultLocation.lat, lng: defaultLocation.lng },
    zoom: defaultLocation.zoom,
    disableDefaultUI: true,
    mapTypeControl: true,
    zoomControl: true,
    fullscreenControl: true,
  });
  console.log("HomeMap Made");

  // const imageBounds = {
  //   north: 59.915502221623,
  //   south: 59.6378021993291,
  //   east: 10.7938958857909,
  //   west: 10.4588770529082,
  // };

  // imageOverlay = new google.maps.GroundOverlay(
  //   "https://firebasestorage.googleapis.com/v0/b/oasys-2d5b2.appspot.com/o/test.png?alt=media&token=3dab5a0d-79bd-4f5a-ae05-b7facf3873f5",
  //   imageBounds,
  //   { opacity: 0.5 }
  // );
  // imageOverlay.setMap(homeMap);
}

function initDataMap() {
  dataMap = new google.maps.Map(dataMapDiv, {
    center: { lat: defaultLocation.lat, lng: defaultLocation.lng },
    zoom: defaultLocation.zoom,
    disableDefaultUI: true,
    mapTypeControl: true,
    zoomControl: true,
    fullscreenControl: true,
  });
}

function addMarker(location, icon, index, map) {
  let marker = new google.maps.Marker({
    position: { lat: location.lat, lng: location.lng },
    map: map,
    icon: icon,
    animation: google.maps.Animation.DROP,
    title: `${index + 1}: ${location.t}`,
  });
  markers.push(marker);
}

function addHomeMarkers(location, alias) {
  var markerIcon = {
    url: "assets/svg/Glider.svg",
    labelOrigin: new google.maps.Point(40, 10),
  };

  new google.maps.Marker({
    position: { lat: location.lat, lng: location.lng },
    map: homeMap,
    animation: google.maps.Animation.DROP,
    label: {
      text: alias,
      color: "#CA4955",
      fontSize: "20px",
      fontWeight: "bold",
    },
    icon: markerIcon,
    title: location.t,
  });
}

function addDataMarkers(coordinates, map) {
  // Loop through markers and remove them from map.
  markers.forEach((marker) => {
    marker.setMap(null);
  });

  if (gliderPath) gliderPath.setMap(null);

  // Reset marker-array
  markers = [];

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

  let latlng = coordinates.map((e) => {
    return { lat: e.lat, lng: e.lng };
  });

  const lineSymbol = {
    path: "M 0,-1 0,1",
    strokeColor: "#FF0000",
    strokeOpacity: 1.0,
    strokeWeight: 2,
    scale: 4,
  };

  gliderPath = new google.maps.Polyline({
    path: latlng,
    geodesic: true,
    icons: [
      {
        icon: lineSymbol,
        offset: "0",
        repeat: "16px",
      },
    ],
    strokeOpacity: 0,
  });
  map.panTo(firstMarker);
  map.setZoom(10);
  gliderPath.setMap(map);
}

function initMissionMap() {
  console.log("Loading mission map");
  missionMap = new google.maps.Map(missionMapDiv, {
    center: { lat: defaultLocation.lat, lng: defaultLocation.lng },
    zoom: defaultLocation.zoom,
    disableDefaultUI: true,
    mapTypeControl: true,
    zoomControl: true,
    fullscreenControl: true,
  });

  missionWaypoints = new google.maps.Polyline({
    strokeColor: "#FF0000",
    editable: true,
    strokeOpacity: 1.0,
    strokeWeight: 2,
    map: missionMap,
  });

  google.maps.event.addListener(
    missionWaypoints,
    "dragend",
    createPreviewWaypoints
  );
  google.maps.event.addListener(
    missionWaypoints.getPath(),
    "insert_at",
    createPreviewWaypoints
  );
  google.maps.event.addListener(
    missionWaypoints.getPath(),
    "remove_at",
    createPreviewWaypoints
  );
  google.maps.event.addListener(
    missionWaypoints.getPath(),
    "set_at",
    createPreviewWaypoints
  );

  var mapListener = missionMap.addListener("click", addLatLng);

  /**
   * A menu that lets a user delete a selected vertex of a path.
   */
  class DeleteMenu extends google.maps.OverlayView {
    constructor() {
      super();
      this.div_ = document.createElement("div");
      this.div_.className = "delete-menu";
      this.div_.innerHTML = "Delete";
      const menu = this;
      google.maps.event.addDomListener(this.div_, "click", () => {
        menu.removeVertex();
        google.maps.event.removeListener(mapListener);
        setTimeout(() => {
          mapListener = missionMap.addListener("click", addLatLng);
        }, 1);
      });
    }
    onAdd() {
      const deleteMenu = this;
      const map = this.getMap();
      this.getPanes().floatPane.appendChild(this.div_);
      // mousedown anywhere on the map except on the menu div will close the
      // menu.
      this.divListener_ = google.maps.event.addDomListener(
        map.getDiv(),
        "mousedown",
        (e) => {
          if (e.target != deleteMenu.div_) {
            deleteMenu.close();
          }
        },
        true
      );
    }
    onRemove() {
      if (this.divListener_) {
        google.maps.event.removeListener(this.divListener_);
      }
      this.div_.parentNode.removeChild(this.div_);
      // clean up
      this.set("position", null);
      this.set("path", null);
      this.set("vertex", null);
    }
    close() {
      this.setMap(null);
    }
    draw() {
      const position = this.get("position");
      const projection = this.getProjection();

      if (!position || !projection) {
        return;
      }
      const point = projection.fromLatLngToDivPixel(position);
      this.div_.style.width = "40px";
      this.div_.style.borderRadius = "6px";
      this.div_.style.border = "1px solid #adadad";
      this.div_.style.padding = "6px 4px 4px 4px";
      this.div_.style.backgroundColor = "#fff";
      this.div_.style.top = point.y - 20 + "px";
      this.div_.style.left = point.x + 10 + "px";
      this.div_.style.position = "relative";
    }
    /**
     * Opens the menu at a vertex of a given path.
     */
    open(map, path, vertex) {
      this.set("position", path.getAt(vertex));
      this.set("path", path);
      this.set("vertex", vertex);
      this.setMap(map);
      this.draw();
    }
    /**
     * Deletes the vertex from the path.
     */
    removeVertex() {
      const path = this.get("path");
      const vertex = this.get("vertex");

      if (!path || vertex == undefined) {
        this.close();
        return;
      }
      path.removeAt(vertex);
      this.close();
    }
  }
  const deleteMenu = new DeleteMenu();
  google.maps.event.addListener(missionWaypoints, "contextmenu", (e) => {
    // Check if click was on a vertex control point
    if (e.vertex == undefined) {
      return;
    }
    deleteMenu.open(missionMap, missionWaypoints.getPath(), e.vertex);
  });
}

// Handles click events on a map, and adds a new point to the Polyline.
function addLatLng(event) {
  let path = missionWaypoints.getPath();
  // Because path is an MVCArray, we can simply append a new coordinate
  // and it will automatically appear.
  path.push(event.latLng);
}

function createPreviewWaypoints() {
  let waypoints = missionWaypoints.getPath().Nb;
  let waypointDiv = document.querySelector("#previewWaypoints");

  waypointDiv.innerHTML = `
  <h1 class="border-b col-span-7">Waypoints</h1>
  <h1 class="text-center">No.</h1>
  <h1 class="col-span-3 text-right">Lat</h1>
  <h1 class="col-span-3 text-right">Lng</h1>`;

  waypoints.forEach((waypoint, index) => {
    let lat = waypoint.lat().toFixed(4);
    let lng = waypoint.lng().toFixed(4);
    let waypointHTML = `
      <p class="col-span-1 text-center">${index + 1}</p>
      <p class="truncate col-span-3 text-right">${lat}</p>
      <p class="truncate col-span-3 text-right">${lng}</p>`;

    waypointDiv.innerHTML += waypointHTML;
  });

  renderWaypointList();
}
