import 'dart:async';

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
                duration: const Duration(milliseconds: 1200),
              ),
            )
          ],
        ),
      ),
    );
  }
}

class ErrorLoading extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Container(
      height: 300,
      width: 300,
      child: Card(
        child: Padding(
          padding: EdgeInsets.all(10),
          child: Text(
            'Something is not right here ... \n'
                'Can not load data ',
            style: TextStyle(
              color: Colors.white,
              fontWeight: FontWeight.bold
            ),
          ),
        ),
        color: Colors.red,
        margin: EdgeInsets.zero,
      ),
    );
  }
}
