import 'package:flutter/material.dart';
import 'package:some_sample/loading.dart';

import 'chart/barChart.dart';
import 'chart/lineChart.dart';
import 'chart/multiLineChart.dart';
import 'dialog/simpleDialog.dart';
import 'sidebar/drawer.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Samples',
      theme: ThemeData(

        primarySwatch: Colors.orange,
      ),
      home: Dialog(),
    );
  }
}

class Dialog extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Dialog Samples'),
      ),
      body: Center(
        child: Column(
          children: <Widget>[
            Padding(padding: EdgeInsets.only(top:250)),
            RaisedButton(
              color: Colors.blue[300],
              child: Text('Click Me'),
              onPressed: (){
                showDialog<Null>(
                    context: context,
                    barrierDismissible: true,
                    builder: (BuildContext context){
                      return ShowSimpleDialog(
                        text: 'show text',
                        backMiss: false,
                      );
                    }
                );
              },
            ),
          ],
        ),
      ),
    );
  }
}


class MyHomePage extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Flutter Samples'),
      ),
      body: Center(
        child: ListView(
          children: <Widget>[
            Padding(
              padding: EdgeInsets.fromLTRB(140, 20, 0, 0),
              child: Text(
                  'Temperature :',
                style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
              ),
            ),
            Container(
              child: LineChartSample(),
            ),
            Padding(
              padding: EdgeInsets.fromLTRB(150, 20, 0, 0),
              child: Text(
                'Battery :',
                style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
              ),
            ),
            Container(
              child: BarChartSample(),
            ),
            Padding(
              padding: EdgeInsets.fromLTRB(140, 20, 0, 0),
              child: Text(
                'Something :',
                style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
              ),
            ),
            Container(
              child: MultiLineChartSample(),
            ),
          ],
        ),
      )
    );
  }
}
