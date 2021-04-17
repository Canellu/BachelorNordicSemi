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

  Stream<QuerySnapshot> get mission {
    //var oneData = await connectCollection.doc("").get();
    return connectCollection.doc("311910").collection("Missions").snapshots();
  }

  /*
  Future<Stream<QuerySnapshot>> get datas async {
    var oneData = await connectCollection.doc("311910").collection("Missions")
        .doc("Mission 1").collection("Data").doc("2021-03-02").get();
    //print(oneData.data());
  }*/

  Future<DocumentSnapshot> get datas async {
    return await connectCollection.doc(gid).collection("Missions")
        .doc("Mission 1").collection("Data").doc("2021-03-02").get();
    //print(oneData.data());
  }
}
