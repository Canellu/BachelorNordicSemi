import 'package:bachelor_app/models/device.dart';
import 'package:bachelor_app/models/mission.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:provider/provider.dart';

class DevicePage extends StatelessWidget {
  final String gliderId;

  const DevicePage({Key key, this.gliderId}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return StreamBuilder(
      stream: DatabaseService(gliderId,"").mission,
      builder: (context,snapshot) {
        if(!snapshot.hasData) return const Text("Loading....");
        return Container(
          child: FloatingActionButton(
            onPressed: (){
              //snapshot.data.docs.forEach((element) => {print(element)});
              snapshot.data.docs.forEach((element) {print(element.id);});
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
        );
      },
    );
  }
}


