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
      //print(doc.id);
      //print(doc.data());
      doc.data().forEach((k, v) {
        //var row = {'${doc.id}T$k':v};
        dataset['${doc.id}T$k'] = v;
      });
    });

    var dataTypeFound = [];
    dataset.forEach((key, value) {
      /*
      if(value.contains("C")) {
        const start = 'C":';
        const end = ",";

        final startIndex = value.indexOf(start);
        final endIndex = value.indexOf(end, startIndex + start.length);
        print(value.substring(startIndex + start.length, endIndex));
      }else{
        print("no C");
      }*/

      var split = value.split(",");
      for(var i = 0; i < split.length -1; i++) {
        //print(split[i]);
        const start = '"';
        const end = '"';

        final startIndex = split[i].indexOf(start);
        final endIndex = split[i].indexOf(end, startIndex + start.length);

        var dataType = split[i].substring(startIndex + start.length, endIndex);

        if(!dataTypeFound.contains(dataType)) {
          dataTypeFound.add(dataType);
        }
      }
      //print(key + "  "+ value);

    });

    //print("FIRST PRINT-------------------------------");
    //print(dataset);
    return missionData;
  }
}
