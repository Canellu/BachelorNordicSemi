import 'package:bachelor_app/models/device.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import 'chart_page.dart';
import 'loadingPage.dart';
import 'mission_Control.dart';

class DevicePage extends StatefulWidget {
  final Device selectedDevice;

  const DevicePage({Key key, this.selectedDevice}) : super(key: key);

  @override
  _DevicePageState createState() => _DevicePageState();
}

class _DevicePageState extends State<DevicePage> with TickerProviderStateMixin {
  TabController _tabController;

  @override
  void initState() {
    super.initState();
    _tabController = TabController(vsync: this, length: 2);
    _tabController.addListener(() {
      //keyboard on 'Create Mission' page will disappear when changing to 'Chart' page
      if (_tabController.indexIsChanging) {
        FocusScope.of(context).requestFocus(new FocusNode());
      }

      //Only allow vertical version orientation
      SystemChrome.setPreferredOrientations([
        DeviceOrientation.portraitUp,
        DeviceOrientation.portraitDown,
      ]);
    });
  }

  @override
  Widget build(BuildContext context) {
    final gid = widget.selectedDevice.deviceId;

    //Using real time streamBuilder to get Mission list from firestore
    return StreamBuilder(
        stream: DatabaseService(gid,"").mission,
        builder: (context, snapshot) {
          if (!snapshot.hasData) return Loading();

          final List<String> missionIdList = [];
          final List<DocumentSnapshot> document = snapshot.data.docs;
          document.forEach((element) {missionIdList.add(element.id);});

          return Scaffold(
            appBar: AppBar(
              title: Text("ID - ${widget.selectedDevice.deviceId}"),
              centerTitle: true,
              actions: [
                IconButton(
                    onPressed: () {}, icon: Icon(Icons.battery_alert_outlined)),
                IconButton(
                    onPressed: () {}, icon: Icon(Icons.notifications_none))
              ],
              elevation: 10,
              flexibleSpace: Container(
                decoration: BoxDecoration(
                  color: const Color.fromRGBO(31, 41, 55, 1),
                ),
              ),
              bottom: TabBar(
                controller: _tabController,
                indicatorColor: Colors.white,
                indicatorWeight: 5,
                tabs: <Widget>[
                  Tab(
                    icon: Icon(Icons.analytics_outlined),
                    text: 'DATA',
                  ),
                  Tab(
                    icon: Icon(Icons.note_add_outlined),
                    text: 'MISSION CONTROL',
                  )
                ],
              ),
            ),
            body: TabBarView(
              controller: _tabController,
              physics: NeverScrollableScrollPhysics(),
              //Navigtors to Data-Tab and Mission control-tab
              //parse current selected glider id and mission to new page/tab
              children: <Widget>[
                ChartPage(gid: gid,missionList: missionIdList),
                MissionTabPage(gid: gid,missionList: missionIdList),
              ],
            ),
          );
        }
    );
  }
}
