import 'dart:io';

import 'package:bachelor_app/models/device.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
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
  final Map<MarkerId, Marker> mapMakers;
  final String navigateFrom;
  final Map<PolylineId, Polyline> mapPolylines;

  const MapFullPage({Key key, this.navigateFrom, this.mapMakers, this.mapPolylines}) : super(key: key);

  @override
  _MapFullPageState createState() => _MapFullPageState();
}

class _MapFullPageState extends State<MapFullPage> {
  Completer<GoogleMapController> _googleMapControllerCompleter = Completer();
  String title;

  @override
  void initState(){
    super.initState();
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
      //DeviceOrientation.portraitUp,
      //DeviceOrientation.portraitDown,
    ]);
  }

  dispose(){
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitUp,
      DeviceOrientation.portraitDown,
    ]);
    super.dispose();
  }

  void _onMapCreated(GoogleMapController controller) {
    _googleMapControllerCompleter.complete(controller);
  }

  _updateUserMarkerAndCamera() async {
    final GoogleMapController controller =
    await _googleMapControllerCompleter.future;
    LatLng latLngPosition = widget.mapMakers.values.first.position;
    controller.animateCamera(
      CameraUpdate.newCameraPosition(
        CameraPosition(
          target: latLngPosition,
          zoom: 9,
        ),
      ),
    );
  }

  String titleForPage() {
    if(widget.navigateFrom == "home"){
      return 'Last Position';
    }else {
      return 'Mission Route';
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      //Remove the debug banner at the right top
      debugShowCheckedModeBanner: false,
      home: Scaffold(/*
        appBar: AppBar(
          elevation: 20,
          title: Text(
            titleForPage(),
            style: TextStyle(
              letterSpacing: 3,
              fontWeight: FontWeight.bold,
            ),
          ),
          centerTitle: true,
          leading: IconButton(
            icon: Icon(Icons.arrow_back_rounded),
            onPressed: () {
              Navigator.of(context).pop();
            },
          ),
          flexibleSpace: Container(
            decoration: BoxDecoration(
              color: const Color.fromRGBO(31, 41, 55, 1),
            ),
          ),
        ),*/
        body: Container(
          height: MediaQuery.of(context).size.height,
          width: MediaQuery.of(context).size.width,
          child: Stack(
            children: <Widget>[
              _showMap(),
              Padding(
                padding: EdgeInsets.all(30),
                child: Align(
                  alignment: Alignment.topRight,
                  child: FloatingActionButton(
                      heroTag: "btn1",
                      materialTapTargetSize: MaterialTapTargetSize.padded,
                      backgroundColor: Colors.white,
                      child: Icon(
                        Icons.location_on,
                        color: Colors.black,
                        size: 25,
                      ),
                      onPressed: () {
                        _updateUserMarkerAndCamera();
                      }
                  ),
                ),
              ),
              Padding(
                padding: EdgeInsets.all(30),
                child: Align(
                  alignment: Alignment.topLeft,
                  child: FloatingActionButton(
                      heroTag: "btn2",
                      materialTapTargetSize: MaterialTapTargetSize.padded,
                      backgroundColor: Colors.white,
                      child: Icon(
                        Icons.arrow_back,
                        color: Colors.black,
                        size: 25,
                      ),
                      onPressed: () {
                        dispose();
                        Navigator.of(context).pop();
                      }
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _showMap(){
    if(widget.navigateFrom == 'home') {
      return GoogleMap(
        onMapCreated: _onMapCreated,
        markers: Set.of(widget.mapMakers.values),
        initialCameraPosition: CameraPosition(
          target: widget.mapMakers.values.first.position,
          zoom: 5,
        ),
      );
    }else {
      return GoogleMap(
        onMapCreated: _onMapCreated,
        markers: Set.of(widget.mapMakers.values),
        polylines: Set.of(widget.mapPolylines.values),
        initialCameraPosition: CameraPosition(
          target: widget.mapMakers.values.first.position,
          zoom: 9,
        ),
      );
    }
  }
}
