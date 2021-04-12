import 'package:bachelor_app/models/device.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'dart:convert';

class DatabaseService {
  final CollectionReference connectCollection =
      FirebaseFirestore.instance.collection('Gliders');

  //divice list from snapshot
  List<Device> _deviceListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc) {
      return Device(
          added: doc.data()['Added'] ?? 'not found',
          alias: doc.data()['Alias'] ?? 'not found',
          lastSync: doc.data()['Last sync'] ?? 'not found');
    }).toList();
  }

  //get stream
  Stream<List<Device>> get glider {
    return connectCollection.snapshots().map(_deviceListFromSnapshot);
  }

  somesht() async {
    var noe =
        await connectCollection.doc("311910").collection("Missions").get();
    print("-------------------------------------------------- ETTER AWAIT");

    // print(noe);
    // print(noe.docs);
    // noe.docs.forEach((doc) async {
    //   var stash = await connectCollection
    //       .doc("311910")
    //       .collection("Missions")
    //       .doc(doc.id)
    //       .collection("Data")
    //       .get();
    //   stash.docs.forEach((doc) {
    //     print("*****************" + doc.id);
    //     doc.data().forEach((k, v) => print('$k'));
    //   });
    // });

    // Get single doc test
    // var enDoc = await connectCollection
    //     .doc("311910")
    //     .collection("Missions")
    //     .doc("Mission 1")
    //     .collection("Data")
    //     .doc("2021-03-02")
    //     .get();

    // print(enDoc.data());

    // enDoc.data().forEach((key, value) => {print(key)});
  }
}
