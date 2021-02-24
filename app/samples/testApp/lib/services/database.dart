import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:oasys_app/models/data.dart';

class DatabaseService {
  //collection reference
  final CollectionReference connectionTestCollection = FirebaseFirestore.instance.collection('Sensor');

  Future<void> updateData(String battery, String duration, String health) async {
    return await connectionTestCollection.doc('H4JYgp8VythfG7JX4GZ4').update({
      'Battery': battery,
      'Dive duration': duration,
      'Health': health
    }).then((value) => print("Database Updated"))
        .catchError((error) => print("Failedd to update database : $error"));
  }

//take out data as list from snapshot
  List<Data> dataListFromSanpshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc){
      //print(doc.data)
      return Data(
          health: doc.data()['Health'] ?? "false",
          duration: doc.data()['Dive duration'] ?? "0",
          battery: doc.data()['Battery'] ?? ''
      );
    }).toList();
  }

//get databse stream
  Stream<List<Data>> get datas {
    return connectionTestCollection.snapshots()
        .map(dataListFromSanpshot);
  }
}