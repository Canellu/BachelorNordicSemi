import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'dart:async';
import 'package:bachelor_app/models/device.dart';
import 'full_map_page.dart';

class MapScreen extends StatefulWidget {
  MapScreen(this.devices);
  final List<Device> devices;
  @override
  _MapScreenState createState() => _MapScreenState();
}

class _MapScreenState extends State<MapScreen> {
  double lat;
  double lng;
  Map<String, dynamic> locationMap = new Map();
  var latlng = [];
  String devId;
  Map<MarkerId, Marker> markers = {};
  final Set<Marker> _markers = {};
  Completer<GoogleMapController> _googleMapControllerCompleter = Completer();

  @override
  void initState() {
    super.initState();
  }

  void _onMapCreated(GoogleMapController controller) {
    _googleMapControllerCompleter.complete(controller);

    locationMap.forEach((key, value) {
      _markers.add(
        Marker(
          markerId: MarkerId(key[0]),
          position: LatLng(value[0], value[1]),
          infoWindow: InfoWindow(title: '$key'),
          icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
        ),
      );

      _markers.add(
        Marker(
          markerId: MarkerId(key[1]),
          position: LatLng(value[2], value[3]),
          infoWindow: InfoWindow(title: '$key'),
          icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
        ),
      );

      _markers.add(
        Marker(
          markerId: MarkerId(key[2]),
          position: LatLng(value[4], value[5]),
          infoWindow: InfoWindow(title: '$key'),
          icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
        ),
      );

      _markers.add(
        Marker(
          markerId: MarkerId(key[3]),
          position: LatLng(value[6], value[7]),
          infoWindow: InfoWindow(title: '$key'),
          icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
        ),
      );
      setState(() {});
    });
  }


  @override
  Widget build(BuildContext context) {
//Data processing for maps
    widget.devices.forEach((element) {
      //final List<String> dataPart = element.lastSeen.split(",");
      final dataPart = element.lastSeen.split(",");
      final newDataPart = dataPart.toString();
      final regex = new RegExp(r'[a-z : \[\]]');

//removes the "lat:"" and "lng:"" characters
      final String coor = newDataPart.replaceAll(regex, '');
      lat = double.parse(coor.substring(0, 7));
      lng = double.parse(coor.substring(8, 14));
      latlng.add(lat);
      latlng.add(lng);
      /*
      print(locationMap.keys);
      print("-----------------------------------");

      print(lng);
      print("-----------------------------------");
//Problemet ligger i markers, den är tom
      print(_markers);
      print("-----------------------------------");
      */
//DeviceId
      locationMap[element.deviceId] = latlng;
      //print("-----------------------------------");
      // print(locationMap.values);
    });

    _updateUserMarkerAndCamera(double lat, double lng) async {
      final GoogleMapController controller =
      await _googleMapControllerCompleter.future;
      LatLng latLngPosition = LatLng(lat, lng);
      controller.animateCamera(
        CameraUpdate.newCameraPosition(
          CameraPosition(
            target: latLngPosition,
            zoom: 5,
          ),
        ),
      );
      //and some other logics
    }

    return MaterialApp(
      //Remove the debug banner at the right top
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        body: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          children: <Widget>[
            new Container(
              decoration: BoxDecoration(
                boxShadow: [
                  BoxShadow(
                    color: Colors.black38,
                    blurRadius: 10,
                  )
                ],
              ),
              margin: EdgeInsets.all(5),
              //Both height works, but need to see which one work best on real machine
              //height: MediaQuery.of(context).size.height / 3.8,
              //height: 200,
              height: MediaQuery.of(context).size.height / 3.2,
              width: MediaQuery.of(context).size.width,
              child: new Stack(
                children: <Widget>[
                  GoogleMap(
                    gestureRecognizers: < Factory < OneSequenceGestureRecognizer >> [
                      new Factory < OneSequenceGestureRecognizer > (
                            () => new EagerGestureRecognizer(),
                      ),
                    ].toSet(),
                    onMapCreated: _onMapCreated,
                    markers: _markers,
                    //Set<Marker>.of(markers.values),
                    initialCameraPosition: CameraPosition(
                      target: LatLng(lat, lng),
                      //(59.785604271197464, 10.435280266881726),
                      zoom: 5.0,
                    ),
                  ),
                  Padding(
                    padding: EdgeInsets.all(10),
                    child: Align(
                      alignment: Alignment.topRight,
                      child: Column(
                        children: <Widget>[
                          FloatingActionButton(
                            //add a heroTag for buttons, remove the multi hero error
                            heroTag: "btn1",
                            mini: true,
                            materialTapTargetSize: MaterialTapTargetSize.padded,
                            backgroundColor: Colors.white,
                            child: Icon(
                              Icons.zoom_out_map,
                              color: Colors.black,
                              size: 25,
                            ),
                            onPressed: () => Navigator.push(
                              context,
                              MaterialPageRoute(
                                builder: (context) => MapFullPage(lat: lat, lng: lng),
                              ),
                            ),
                          ),
                          SizedBox(height: 5),
                          FloatingActionButton(
                              heroTag: "btn2",
                            mini: true,
                            materialTapTargetSize: MaterialTapTargetSize.padded,
                            backgroundColor: Colors.white,
                            child: Icon(
                              Icons.location_on,
                              color: Colors.black,
                              size: 25,
                            ),
                            onPressed: () {
                              _updateUserMarkerAndCamera(lat,lng);
                            }
                          ),
                        ],
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
