import 'dart:async';
//import 'dart:html';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:bachelor_app/models/device.dart';

class MapPerDevice extends StatefulWidget {
  final Map dataset;
  final List missionList;
  final List<Device> devices;
  final Map locationMap;
  final String gid;

  MapPerDevice(
      {Key key,
      this.dataset,
      this.missionList,
      this.devices,
      this.locationMap,
      this.gid})
      : super(key: key);

  @override
  _MapPerDeviceState createState() => _MapPerDeviceState();
}

class _MapPerDeviceState extends State<MapPerDevice> {
  final Set<Marker> markers = {};
  Completer<GoogleMapController> _controller = Completer();
  List joinedList = [];
  var time = [];
//var pair;
  var dateString;
  var day;
  var dayString;
  var hour;
  double coorLat;
  double coorLng;

  @override
  void initState() {
    super.initState();
  }

  Widget _onMapCreated(GoogleMapController controller) {
    _controller.complete(controller);

    return _buildGoogleMap(context);
  }

  @override
  Widget build(BuildContext context) {
    return _buildGoogleMap(context);
  }

  Widget _buildGoogleMap(BuildContext context) {
//Mission

    //Coordinates get
    for (var i = 0; i < widget.dataset.length; i++) {
      for (var i in widget.dataset["coodinates"]) {
        joinedList.add(i.lat);
        joinedList.add(i.lng);
        time.add(i.date);
      }
      //Coordinates split
      final string = joinedList.toString();
      final regex = new RegExp(r'[a-z : \[\]] ');
      final latlng = string.replaceAll(regex, "");

      var coor = latlng.split(",");

      var coorLat = double.parse(coor[0]);
      for (int i = 0; i < coor.length; i++) {
        coorLat = double.parse(coor[i]);
        coorLng = double.parse(coor[i + 1]);
      }

      //Time data formating
      /*
      dateString = time.toString();
      day = dateString.split("T");
      dayString = day.toString();
      hour = dayString.split(",");
*/

      widget.dataset.forEach((key, value) {});
      markers.add(Marker(
        markerId: MarkerId(widget.gid),
        // position: LatLng(coor[0], coor[1]),
        //infoWindow: InfoWindow(),
        icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
      ));

      setState(() {});

      print("-------------------------------------");
      print(coorLat);

      return Container(
        height: MediaQuery.of(context).size.height,
        width: MediaQuery.of(context).size.width,
        child: GoogleMap(
          onMapCreated: _onMapCreated,
          //markers: markers,
          mapType: MapType.normal,
          initialCameraPosition: CameraPosition(
              target: LatLng(59.9099500758733, 10.726712406866044), zoom: 12),
          //onLongPress: (LatLng latLng) {},
        ),
      );
    }
  }

// (59.9099500758733, 10.726712406866044)

}
