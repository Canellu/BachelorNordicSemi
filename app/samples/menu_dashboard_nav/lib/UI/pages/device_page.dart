//import 'dart:js';

import 'package:bachelor_app/UI/pages/home_page.dart';
import 'package:bachelor_app/models/device.dart';
import 'package:bachelor_app/models/mission.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';
import 'package:provider/provider.dart';

import 'chart_page.dart';
import 'mission_Control.dart';

<<<<<<< HEAD
class DevicePageTab extends StatelessWidget {
=======
class DevicePageTab extends StatefulWidget {
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
  final Device selectedDevice;

  const DevicePageTab({Key key, this.selectedDevice}) : super(key: key);

  @override
<<<<<<< HEAD
  Widget build(BuildContext context) {
    final gid = selectedDevice.deviceId;

    return StreamBuilder(
        stream: DatabaseService(selectedDevice.deviceId, "").mission,
=======
  _DevicePageTabState createState() => _DevicePageTabState();
}

class _DevicePageTabState extends State<DevicePageTab> with TickerProviderStateMixin {
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
    });
  }

  @override
  Widget build(BuildContext context) {
    final gid = widget.selectedDevice.deviceId;

    return StreamBuilder(
        stream: DatabaseService(gid,"").mission,
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
        builder: (context, snapshot) {
          if (!snapshot.hasData) return const Text("Loading....");

          final List<String> missionIdList = [];
          final List<DocumentSnapshot> document = snapshot.data.docs;
          document.forEach((element) {
            missionIdList.add(element.id);
          });

<<<<<<< HEAD
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
                          end: Alignment.topLeft)),
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
=======
          return Scaffold(
            appBar: AppBar(
              title: Text("Alias : ${widget.selectedDevice.alias}"),
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
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
                    )
                ),
              ),
<<<<<<< HEAD
              body: TabBarView(
                physics: NeverScrollableScrollPhysics(),
                children: <Widget>[
                  ChartPage(gid: gid, missionList: missionIdList),
                  MissionTabPage(gid: gid, missionList: missionIdList),
                  /*
                  Center(
                    child: Text('Mission Control Tab'),
                  ),*/
=======
              bottom: TabBar(
                controller: _tabController,
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
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
                ],
              ),
            ),
            body: TabBarView(
              controller: _tabController,
              physics: NeverScrollableScrollPhysics(),
              children: <Widget>[
                ChartPage(gid: gid,missionList: missionIdList),
                MissionTabPage(gid: gid,missionList: missionIdList),
              ],
            ),
          );
        });
  }
}
