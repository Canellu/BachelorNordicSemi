let dataMap;
let missionMap;
let missionWaypoints;
let markers = [];
let gliderPath;
let dataMapDiv = document.getElementById("dataMap");
let missionMapDiv = document.getElementById("missionMap");
var flightPlanCoordinates;

var defaultLocation = { lat: 59.921, lng: 10.734, zoom: 6 };
function initMap() {
  dataMap = new google.maps.Map(dataMapDiv, {
    center: { lat: defaultLocation.lat, lng: defaultLocation.lng },
    zoom: defaultLocation.zoom,
  });
}

function addMarker(location, icon, index, map) {
  let marker = new google.maps.Marker({
    position: { lat: location.lat, lng: location.lng },
    map: map,
    icon: icon,
    title: `${index + 1}: ${location.t}`,
  });
  markers.push(marker);
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
    let icon = "../assets/svg/triangleMarker.svg";

    // If start give icon, if end, give another
    switch (index) {
      case 0:
        firstMarker = { lat: loc.lat, lng: loc.lng };
        icon = "../assets/svg/startMarkerWhite.svg";
        break;
      case coordinates.length - 1:
        icon = "../assets/svg/endMarkerWhite.svg";
        break;
    }

    addMarker(loc, icon, index, map);
  });

  let latlng = coordinates.map((e) => {
    return { lat: e.lat, lng: e.lng };
  });

  const lineSymbol = {
    path: "M 0,-1 0,1",
    strokeOpacity: 1,
    strokeColor: "#1F2937",
    strokeWeight: 1.5,
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
  });

  missionWaypoints = new google.maps.Polyline({
    strokeColor: "#FF0000",
    editable: true,
    strokeOpacity: 1.0,
    strokeWeight: 3,
    map: missionMap,
  });

  missionMap.addListener("click", addLatLng);

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
      this.div_.style.width = "30px";
      this.div_.style.height = "30px";
      this.div_.style.top = point.y + "px";
      this.div_.style.left = point.x + "px";
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

      console.log(vertex);
      console.log(path);
      console.log(path.getAt(vertex));
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

  console.log(path.Nb);
}
