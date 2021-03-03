import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';

class GoogleMapScreen extends StatefulWidget {
  @override
  _GoogleMapScreenState createState() => _GoogleMapScreenState();
}

class _GoogleMapScreenState extends State<GoogleMapScreen> {
  Set<Marker> _marker = {};

  void _onMapCreated(GoogleMapController controller) {
    setState(() {
      _marker.add(
        Marker(
          markerId: MarkerId('id1'),
          position: LatLng(59.921195475762616, 10.733630008176334),
        ),
      );
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(
          'Google Map Screen',
          style: TextStyle(
            fontSize: 25,
            fontWeight: FontWeight.bold,
          ),
        ),
      ),
      body: GoogleMap(
        onMapCreated: _onMapCreated,
        markers: _marker,
        initialCameraPosition: CameraPosition(
          target: LatLng(59.921195475762616, 10.733630008176334),
          zoom: 15,
        ),
      ),
    );
  }
}
