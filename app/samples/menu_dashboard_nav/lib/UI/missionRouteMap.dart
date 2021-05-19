import 'dart:async';

import 'package:bachelor_app/UI/pages/full_map_page.dart';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';


class ShowRouteMap extends StatefulWidget {
  final Map dataset;

  const ShowRouteMap({Key key, this.dataset}) : super(key: key);

  @override
  _ShowRouteMapState createState() => _ShowRouteMapState();
}

class _ShowRouteMapState extends State<ShowRouteMap> {
  Completer<GoogleMapController> _googleMapControllerCompleter = Completer();
  Map<MarkerId, Marker> _mapMakers = {};
  Map<PolylineId, Polyline> _mapPolylines = {};
  List coordinateMap;
  BitmapDescriptor customIcon;

  var lat;
  var lng;
  int count = 1;

  @override
  void initState() {
    super.initState();
    coordinateMap = widget.dataset["coodinates"];/*
    BitmapDescriptor.fromAssetImage(ImageConfiguration(size: Size(2, 2)),
        'assets/images/triangleMarker.jpg')
    //'assets/images/Glider.jpg')
        .then((d) {
      customIcon = d;
    });*/
  }

  void _onMapCreated(GoogleMapController controller) {
    _googleMapControllerCompleter.complete(controller);
  }

  _updateUserMarkerAndCamera(double lat, double lng) async {
    final GoogleMapController controller =
    await _googleMapControllerCompleter.future;
    LatLng latLngPosition = LatLng(lat, lng);
    controller.animateCamera(
      CameraUpdate.newCameraPosition(
        CameraPosition(
          target: latLngPosition,
          zoom: 9,
        ),
      ),
    );
  }



  void _createMarkerAndLine() {
    //_controller.complete(controller);
    _mapMakers.clear();
    _mapPolylines.clear();
    count = 1;

      final List<LatLng> points = [];
      BitmapDescriptor markerIcon;

    BitmapDescriptor.fromAssetImage(ImageConfiguration(size: Size(2, 2)),
        'assets/images/triangleMarker.jpg')
    //'assets/images/Glider.jpg')
        .then((d) {
      customIcon = d;
    });

      //for(var i in widget.dataset["coodinates"])
        for(var i in coordinateMap){
        if(count == 1) {
          lat = i.lat;
          lng = i.lng;
          markerIcon = BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed);
        }else if(count == coordinateMap.length){
          markerIcon = BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueGreen);
        } else {
          if(customIcon == null) {
            markerIcon = BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueBlue);
          }else {
            markerIcon = customIcon;
          }
        }

        points.add(LatLng(i.lat, i.lng));


        MarkerId markerId =MarkerId(count.toString());
        Marker marker = Marker(
          markerId: markerId,
          position: LatLng(i.lat,i.lng),
          infoWindow: InfoWindow(title: i.date),
          icon: markerIcon,
        );

        _mapMakers[markerId] = marker;
        count ++;
      }

      final PolylineId polylineId = PolylineId('polyline_id_1');
      final Polyline polylines = Polyline(
        polylineId: polylineId,
        consumeTapEvents: true,
        color: Colors.red,
        width: 5,
        points: points,
      );

      _mapPolylines[polylineId] = polylines;
  }


  @override
  Widget build(BuildContext context) {
    _createMarkerAndLine();
    return Container(
      decoration: BoxDecoration(
        boxShadow: [
          BoxShadow(
            color: Colors.black38,
            blurRadius: 5,
          )
        ],
      ),
      margin: EdgeInsets.fromLTRB(0, 0, 0, 10),
      height: MediaQuery.of(context).size.height /2 + 70,
      width: MediaQuery.of(context).size.width -20,
      child: Stack(
        children: <Widget>[
          GoogleMap(
            onMapCreated: _onMapCreated,
            markers: Set.of(_mapMakers.values),
            polylines: Set.of(_mapPolylines.values),
            initialCameraPosition: CameraPosition(
              target: LatLng(lat, lng),
              zoom: 9,
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
                            MapFullPage(mapMakers: _mapMakers, mapPolylines: _mapPolylines,navigateFrom: "mission",),
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
    );
  }
}
