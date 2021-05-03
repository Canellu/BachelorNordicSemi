import 'dart:collection';

import 'package:bachelor_app/models/device.dart';
import 'package:bachelor_app/models/mission.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';

class DatabaseService {
  final CollectionReference connectCollection = FirebaseFirestore.instance.collection('Gliders');

  final String gid;
  final String mid;
  DatabaseService(this.gid, this.mid);


  //divice list from snapshot
  List<Device> _deviceListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc){
      return Device(
          added: doc.data()['Added'] ?? 'not found',
          alias: doc.data()['Alias'] ?? 'not found',
          lastSync: doc.data()['Last sync'] ?? 'not found',
          deviceId: doc.id ?? "not found",
          lastSeen: doc.data()['Last seen'] ?? 'not found'
      );
    }).toList();
  }
/*
  List<Mission> _missionListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc){
      return Mission(
        endTime: doc.data()['end'] ?? 'not end yet',
        freqC: doc.data()['freqC'] ?? 'not found',
        freqP: doc.data()['freqP'] ?? 'not found',
        freqT: doc.data()['freqT'] ?? 'not found',
        minD: doc.data()['minD'] ?? 'not found',
        maxD: doc.data()['maxD'] ?? 'not found',
        startTime: doc.data()['start'] ?? 'not found',
        missionId: doc.id ?? 'not found',
      );
    }).toList();
  }*/

  Future<void> newMission(Mission missionObject) async {
    return await connectCollection.doc(gid).collection("Missions").doc(missionObject.missionId).set({
      "freqC" : missionObject.freqC,
      "freqP" : missionObject.freqP,
      "freqT" : missionObject.freqT,
      "maxD" : missionObject.maxD,
      "minD" : missionObject.minD,
      "start" : missionObject.startTime
    }).then((value) => print("New Mission Added"))
    .catchError((error) => print("Failed to add new mission : $error"));
  }

  //get stream
  Stream<List<Device>> get glider {
    return connectCollection.snapshots()
        .map(_deviceListFromSnapshot);
  }
/*
  //get missions doc from database
  Stream<List<Mission>> get mission {
    return connectCollection.doc("311910").collection("Missions").snapshots()
        .map(_missionListFromSnapshot);
  }*/
  Stream<QuerySnapshot> get mission {
    return connectCollection.doc("311910").collection("Missions").snapshots();
  }

/*
  Map<String,dynamic> preProcessData(QuerySnapshot snapshot) {
    var dataset = testData;
  }

  Future<QuerySnapshot> get testData async {
    return await connectCollection.doc(gid).collection("Missions")
        .doc(mid).collection("Data").get();
  }
*/
  //get data for selected mission, and preprocess it
  Future<Map<String, dynamic>> get datas async {
    var missionData = await connectCollection.doc(gid).collection("Missions")
        .doc(mid).collection("Data").get();

    final SplayTreeMap<String,String> dataset = SplayTreeMap<String,String>();
    var allDate = [];

    missionData.docs.forEach((doc) {
      allDate.add(doc.id);
      doc.data().forEach((k, v) {
        dataset['${doc.id}T$k'] = v;
      });
    });

    //Loop through to find each data type
    var dataTypeFound = [];
    dataset.forEach((key, value) {
      var split = value.split(",");
      for(var i = 0; i < split.length -1; i++) {
        const start = '"';
        const end = '"';

        final startIndex = split[i].indexOf(start);
        final endIndex = split[i].indexOf(end, startIndex + start.length);

        var dataType = split[i].substring(startIndex + start.length, endIndex);

        if(!dataTypeFound.contains(dataType)) {
          dataTypeFound.add(dataType);
        }
      }
    });

    final Map<String, dynamic> dataObj = new Map();
    dataObj["Date"] = allDate;

    final Map<String, Map> dataCoodinatesRaw = new Map();

    dataTypeFound.forEach((type) {

      final Map<String, String> typeDataset = {};

      dataset.forEach((key, value) {
        if(value.contains(type)) {
          var start = '$type":';
          const end = ",";

          final startIndex = value.indexOf(start);
          final endIndex = value.indexOf(end, startIndex + start.length);
          var typeData = value.substring(startIndex + start.length, endIndex);

          typeDataset[key] = typeData;
        }
      });


      if(type == "lng" || type == "lat") {
        dataCoodinatesRaw[type] = typeDataset;
      }else {
        dataObj[type] = typeDataset;
      }
    });

    var data = [];
    //elementAt(0) == lng, elementAt(1) == lat
    var dataCoodinatesValue = dataCoodinatesRaw.values;

    for(var i = 0; i<dataCoodinatesValue.elementAt(0).length; i++){
      //lat and long have the same key(timestamp)
      var t = dataCoodinatesValue.elementAt(0).keys.elementAt(i);
      var lng = double.parse(dataCoodinatesValue.elementAt(0).values.elementAt(i));
      var lat = double.parse(dataCoodinatesValue.elementAt(1).values.elementAt(i));

      data.add({'t':t, 'lng':lng, 'lat':lat});
    }
    dataObj["coodinates"] = data;

    return dataObj;
  }
}
