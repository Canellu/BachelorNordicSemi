import 'package:bachelor_app/models/device.dart';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'dart:async';
import 'package:bachelor_app/UI/deviceList.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'dart:async';
import 'map_screen.dart';

//import 'database.dart';

class MapFullPage extends StatefulWidget {
  double lat;
  double lng;

  MapFullPage({Key key, this.lat, this.lng}) : super(key: key);
  @override
  _MapFullPageState createState() => _MapFullPageState();
}

class _MapFullPageState extends State<MapFullPage> {
  Completer<GoogleMapController> _controller = Completer();

  void _onMapCreated(GoogleMapController controller) {
    _controller.complete(controller);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      //Remove the debug banner at the right top
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        body: SingleChildScrollView(
          child: Container(
            height: MediaQuery.of(context).size.height,
            width: MediaQuery.of(context).size.width,
            child: GoogleMap(
              onMapCreated: _onMapCreated,
              initialCameraPosition: CameraPosition(
                target: LatLng(widget.lat, widget.lng),
                zoom: 11.0,
              ),
            ),
          ),
        ),
      ),
    );
  }
}
