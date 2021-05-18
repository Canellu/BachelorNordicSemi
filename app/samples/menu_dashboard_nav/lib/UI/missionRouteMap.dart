import 'dart:async';

import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';


class ShowRouteMap extends StatefulWidget {
  final Map dataset;

  const ShowRouteMap({Key key, this.dataset}) : super(key: key);

  @override
  _ShowRouteMapState createState() => _ShowRouteMapState();
}

class _ShowRouteMapState extends State<ShowRouteMap> {
  Completer<GoogleMapController> _controller = Completer();
  Map<MarkerId, Marker> _mapMakers = {};
  Map<PolylineId, Polyline> _mapPolylines = {};

  var lat;
  var lng;
  int count = 1;

  void _onMapCreated(GoogleMapController controller) {
    _controller.complete(controller);
  }

  void _createMarkerAndLine() {
    //_controller.complete(controller);
    _mapMakers.clear();
    _mapPolylines.clear();

      final List<LatLng> points = [];

      for(var i in widget.dataset["coodinates"]) {
        if(count == 1) {
          lat = i.lat;
          lng = i.lng;
        }
        points.add(LatLng(i.lat, i.lng));

        MarkerId markerId =MarkerId(count.toString());
        Marker marker = Marker(
          markerId: markerId,
          position: LatLng(i.lat,i.lng),
          infoWindow: InfoWindow(title: i.date),
          icon: BitmapDescriptor.defaultMarkerWithHue(BitmapDescriptor.hueRed),
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
      child: GoogleMap(
        onMapCreated: _onMapCreated,
        markers: Set.of(_mapMakers.values),
        polylines: Set.of(_mapPolylines.values),
        initialCameraPosition: CameraPosition(
          target: LatLng(lat, lng),
          zoom: 9,
        ),
      ),
    );
  }
}
