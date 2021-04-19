import 'package:bachelor_app/models/device.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';

class DatabaseService {
  final CollectionReference connectCollection = FirebaseFirestore.instance.collection('Gliders');

  final String gid;
  DatabaseService(this.gid);


  //divice list from snapshot
  List<Device> _deviceListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc){
      return Device(
          added: doc.data()['Added'] ?? 'not found',
          alias: doc.data()['Alias'] ?? 'not found',
          lastSync: doc.data()['Last sync'] ?? 'not found',
          deviceId: doc.id ?? "not found"
      );
    }).toList();
  }

  //get stream
  Stream<List<Device>> get glider {
    return connectCollection.snapshots()
        .map(_deviceListFromSnapshot);
  }

  /*
  Stream<QuerySnapshot> get mission {
    //var oneData = await connectCollection.doc("").get();
    return connectCollection.doc("311910").collection("Missions").snapshots();
  }


  Future<QuerySnapshot> get datas async {
    return await connectCollection.doc(gid).collection("Missions")
        .doc("Mission 1").collection("Data").get();
    //print(oneData.data());
  }*/

  Future<QuerySnapshot> get datas async {
    var missionData = await connectCollection.doc(gid).collection("Missions")
        .doc("Mission 1").collection("Data").get();

    final Map<String,String> dataset = {};

    missionData.docs.forEach((doc) {
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

    return missionData;
  }
}
