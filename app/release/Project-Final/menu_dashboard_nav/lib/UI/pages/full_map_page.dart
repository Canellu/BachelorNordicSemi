import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'dart:async';


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

  void _onMapCreated(GoogleMapController controller) {
    _googleMapControllerCompleter.complete(controller);
  }

  //Make map back to initial position
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
      home: Scaffold(
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

  //Display different markers according to the pre-page
  Widget _showMap(){
    //If it is home page map want to display in full map
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
      //If it is mission route want to display in full map
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
