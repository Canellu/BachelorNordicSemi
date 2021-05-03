import 'package:bachelor_app/UI/pages/home_page.dart';
import 'package:bachelor_app/models/device.dart';
import 'package:bachelor_app/models/mission.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:provider/provider.dart';

import 'chart_page.dart';

class DevicePageTab extends StatelessWidget {
  //final String gliderId;
  final Device selectedDevice;

  //const DevicePage({Key key, this.gliderId, this.selectedDevice}) : super(key: key);

  const DevicePageTab({Key key, this.selectedDevice}) : super(key: key);

  @override
  Widget build(BuildContext context) {

    //final mission = Provider.of<List<Mission>>(context);


    return StreamBuilder(
        stream: DatabaseService(selectedDevice.deviceId,"").mission,
        builder: (context, snapshot) {
          if (!snapshot.hasData) return const Text("Loading....");

          final List<String> missionIdList = [];
          final List<DocumentSnapshot> document = snapshot.data.docs;
          document.forEach((element) {missionIdList.add(element.id);});

          return DefaultTabController(
            length: 2,
            child: Scaffold(
              appBar: AppBar(
                title: Text("Alias : ${selectedDevice.alias}"),
                centerTitle: true,
                actions: [
                  IconButton(
                      onPressed: () {}, icon: Icon(Icons.notifications_none))
                ],
                elevation: 10,
                flexibleSpace: Container(
                  decoration: BoxDecoration(
                      gradient: LinearGradient(
                          colors: [Colors.greenAccent, Colors.blue],
                          begin: Alignment.bottomRight,
                          end: Alignment.topLeft
                      )
                  ),
                ),
                bottom: TabBar(
                  //indicatorPadding: EdgeInsets.all(10),
                  indicatorColor: Colors.white,
                  indicatorWeight: 5,
                  tabs: <Widget>[
                    Tab(
                      icon: Icon(Icons.analytics_outlined),
                      text: 'Chart',
                    ),
                    Tab(
                      icon: Icon(Icons.note_add_outlined),
                      text: 'Create Mission',
                    )
                  ],
                ),
              ),
              body: TabBarView(
                physics: NeverScrollableScrollPhysics(),
                children: <Widget>[
                  ChartPage(),
                  Center(
                    child: Text('Mission Control Tab'),
                  ),
                ],
              ),
            ),
          );
        }
    );
  }
}
/*
class DevicePageTab extends StatelessWidget {
  final deviceID;
  final deviceName;
  final missionSnapshot;

  const DevicePageTab({Key key, this.deviceID, this.deviceName, this.missionSnapshot}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    print(missionSnapshot);

    return DefaultTabController(

    );
  }
}*/


/*
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
        );*/

