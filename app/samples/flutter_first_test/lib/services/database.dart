import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter_first_test/models/sensor.dart';

class DatabaseService {
  // reference to the collection 'Sensor' using Firestore instance
  final CollectionReference sensorCollection =
      FirebaseFirestore.instance.collection('Sensor');

  // sensor list from snapshot
  List<Sensor> _sensorListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc) {
      return Sensor(
        battery: doc.data()['Battery'] ?? '',
        duration: doc.data()['Dive duration'] ?? '',
        status: doc.data()['Dive status'] ?? '',
        health: doc.data()['Health'] ?? '',
        temp: doc.data()['Temperature'] ?? '',
      );
    }).toList();
  }

  // get sensor stream
  Stream<List<Sensor>> get sensor {
    return sensorCollection.snapshots().map(_sensorListFromSnapshot);
  }
}
