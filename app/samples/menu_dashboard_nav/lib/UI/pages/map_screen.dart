import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'dart:async';
import 'package:bachelor_app/models/device.dart';
import 'full_map_page.dart';
import 'package:flutter_svg/flutter_svg.dart';

class MapScreen extends StatefulWidget {
  MapScreen(this.devices);
  final List<Device> devices;
  @override
  _MapScreenState createState() => _MapScreenState();
}

class _MapScreenState extends State<MapScreen> {
  Completer<GoogleMapController> _googleMapControllerCompleter = Completer();
  Map<MarkerId, Marker> _mapMakers = {};
  BitmapDescriptor customIcon;
  int count = 1;

  var lat ;
  var lng ;


  @override
  void initState() {
    super.initState();
    BitmapDescriptor.fromAssetImage(ImageConfiguration(size: Size(1, 1)),
        'assets/images/Glider.jpg')
        .then((d) {
      customIcon = d;
    });
  }

  void _onMapCreated(GoogleMapController controller) {
    _googleMapControllerCompleter.complete(controller);
  }

  void _createMarkerAndLine() {
    widget.devices.forEach((element) {

      final dataPart = element.lastSeen.split(",");
      final newDataPart = dataPart.toString();
      final regex = new RegExp(r'[a-z : \[\]]');

      //removes the "lat:"" and "lng:"" characters
      final String coor = newDataPart.replaceAll(regex, '');
      lat = double.parse(coor.substring(0, 7));
      lng = double.parse(coor.substring(8, 14));

      MarkerId markerId =MarkerId(element.deviceId.toString());
      Marker marker = Marker(
        markerId: markerId,
        position: LatLng(lat,lng),
        infoWindow: InfoWindow(title: element.alias),
        //icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
        icon: customIcon,
      );

      _mapMakers[markerId] = marker;
    });
  }

  @override
  Widget build(BuildContext context) {
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
    }

    _createMarkerAndLine();

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
                    markers: Set.of(_mapMakers.values),
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
                                builder: (context) =>
                                    MapFullPage(mapMakers: _mapMakers,navigateFrom: "home",),
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
