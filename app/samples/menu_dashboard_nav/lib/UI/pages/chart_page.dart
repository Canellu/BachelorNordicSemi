/*
import 'package:bachelor_app/service/database.dart';
import 'package:charts_flutter/flutter.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';

import '../chart.dart';

class Todo {
  final String missionID;
  final String dataType;

  Todo(this.missionID, this.dataType);

}

final knowDataType = ["C","P","T"];

class ChartPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return FutureBuilder(
        future: DatabaseService("311910", "Mission 1").datas,
        builder: (context, snapshot) {
          if (!snapshot.hasData) return const Text("Loading....");

          Map dataset = new Map<String,dynamic>.from(snapshot.data);
          print(dataset);
          //List<Series<dynamic,DateTime>> testDataset = [];
          return SingleChildScrollView(
            child: Column(
              children: <Widget>[
Container(
             child: Chart(missionID: '', dataType: "C",),
),
//Map screen here 
              ],
            ),

          );
        }
    );
  }
}

*/





//Prior in commments
/*
          return Container(
            child: FloatingActionButton(
              onPressed: (){
                //print(dataMap.data);
                /*
                dataset.forEach((key, value) {
                  print("$key $value");
                });*/
                /*
                dataset.forEach((key, value) {
                  print("$key $value");
                });*/
                //snapshot.data.docs.forEach((element) => {print(element)});
                //dataMap.data.forEach((element) {print(element);});
                /*
              var createMissionID = snapshot.data.docs.length + 1;
              DatabaseService(gliderId,"").newMission(
                Mission(
                  missionId: "Mission $createMissionID",
                  startTime: DateFormat('yyyy-MM-dd kk:mm:ss').format(DateTime.now()),
                  freqC: 5,
                  freqP: 5,
                  freqT: 50,
                  maxD: 200,
                  minD: 10
              )
              );*/
              },
            ),
          );*/