import 'package:flutter_spinkit/flutter_spinkit.dart';
import 'package:flutter/material.dart';

class Loading extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.grey[800],
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Container(
                child: Text(
                  'LOGO',
                  style: TextStyle(fontSize: 40,fontWeight: FontWeight.bold, color: Colors.white),
                ),
              ),
              SizedBox(height: 40),
              Container(
                child: SpinKitFoldingCube(
                  color: Colors.white,
                ),
              )
            ],
        ),
      ),
    );
  }
}
