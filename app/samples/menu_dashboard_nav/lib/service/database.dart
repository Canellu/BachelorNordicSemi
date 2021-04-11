import 'package:bachelor_app/models/device.dart';
import 'package:cloud_firestore/cloud_firestore.dart';

class DatabaseService {
  final CollectionReference connectCollection = FirebaseFirestore.instance.collection('Gliders');

  //divice list from snapshot
  List<Device> _deviceListFromSnapshot(QuerySnapshot snapshot) {
    return snapshot.docs.map((doc){
      return Device(
          added: doc.data()['Added'] ?? 'not found',
          alias: doc.data()['Alias'] ?? 'not found',
          lastSync: doc.data()['Last sync'] ?? 'not found'
      );
    }).toList();
  }

  //get stream
  Stream<List<Device>> get glider {
    return connectCollection.snapshots()
        .map(_deviceListFromSnapshot);
  }
}