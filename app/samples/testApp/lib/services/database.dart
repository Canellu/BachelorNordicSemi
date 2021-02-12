import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:oasys_app/models/data.dart';

class DatabaseService {
  //give the documentID
  // final String docID;
  // DatabaseService({this.docID});

  //collection reference
  final CollectionReference connectionTestCollection =
      FirebaseFirestore.instance.collection('connectTest');

  Future<void> updateData(String battery, int duration, bool health) async {
    return await connectionTestCollection
        .doc('uhXEigO7WcpwJltYZgPJ')
        .update({'battery': battery, ' duration': duration, 'health': health})
        .then((value) => print("Database Updated"))
        .catchError((error) => print("Failed to update database : $error"));
  }

//take out data as list from snapshot
  List<Data> dataListFromSanpshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc) {
      //print(doc.data)
      return Data(
          health: doc.data()['health'] ?? false,
          duration: doc.data()['duration'] ?? 0,
          battery: doc.data()['battery'] ?? '');
    }).toList();
  }

/*
  DataWithDocInfo dataDocInfoFromSnapshot(QuerySnapshot snapshot){
    return DataWithDocInfo(
        docID: 'uhXEigO7WcpwJItYZgPJ',
        health: snapshot.data()['health'],
        duration: snapshot.data()['duration'],
        battery: snapshot.data()['battery']
    );
  }
*/
//get databse stream
  Stream<List<Data>> get datas {
    return connectionTestCollection.snapshots().map(dataListFromSanpshot);
  }

  // Stream<Data> get docData {
  //   return connectionTestCollection.doc('uhXEigO7WcpwJItYZgPJ').snapshots()
  //       .map(dataListFromSanpshot);
  // }
}
