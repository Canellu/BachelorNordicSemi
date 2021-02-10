import 'package:flutter/material.dart';
import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:hexcolor/hexcolor.dart';

class Loading extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: BoxDecoration(
        color: HexColor('#0081AA'),
      ),
      child: SpinKitCubeGrid(
        color: Colors.white,
        size: 80,
      ),
    );
  }
}
