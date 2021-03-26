let dataMap;
let missionMap;
let markers = [];
let gliderPath;
let dataMapDiv = document.getElementById("dataMap");
let missionMapDiv = document.getElementById("missionMap");

var defaultLocation = { lat: 59.921, lng: 10.734, zoom: 6 };
function initMap() {
  dataMap = new google.maps.Map(dataMapDiv, {
    center: { lat: defaultLocation.lat, lng: defaultLocation.lng },
    zoom: defaultLocation.zoom,
  });
}

function initMissionMap() {
  console.log("Loading mission map");
  missionMap = new google.maps.Map(missionMapDiv, {
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
