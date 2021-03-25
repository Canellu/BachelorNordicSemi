let map;
let markers = [];
let gliderPath;
let mapDiv = document.getElementById("dataMap");
var defaultLocation = { lat: 59.921, lng: 10.734, zoom: 6 };
function initMap() {
  map = new google.maps.Map(mapDiv, {
    center: { lat: defaultLocation.lat, lng: defaultLocation.lng },
    zoom: defaultLocation.zoom,
  });
}

mapDiv.addEventListener("click", () => {});

function addMarker(location, icon, index) {
  let marker = new google.maps.Marker({
    position: { lat: location.lat, lng: location.lng },
    map: map,
    icon: icon,
    title: `${index + 1}: ${location.t}`,
  });
  markers.push(marker);
}

function addMissionMarkers(coordinates) {
  // Loop through markers and remove them from map.
  markers.forEach((marker) => {
    marker.setMap(null);
  });

  if (gliderPath) gliderPath.setMap(null);

  // Reset marker-array
  markers = [];

  // Add new markers to marker-array

  coordinates.forEach((loc, index) => {
    let icon = "../assets/svg/triangleMarker.svg";

    // If start give icon, if end, give another
    switch (index) {
      case 0:
        icon = "../assets/svg/startMarkerWhite.svg";
        break;
      case coordinates.length - 1:
        icon = "../assets/svg/endMarkerWhite.svg";
        break;
    }

    addMarker(loc, icon, index);
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

  gliderPath.setMap(map);
}
