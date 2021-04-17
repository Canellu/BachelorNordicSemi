import 'package:bachelor_app/models/device.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

class DevicePage extends StatelessWidget {
  final String gliderId;

  const DevicePage({Key key, this.gliderId}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    /*
    return StreamBuilder<Future<Stream<QuerySnapshot>>>(
        stream: DatabaseService("").datas,
        builder: (context,snapshot) {
          if(!snapshot.hasData) return const Text("Loading....");

          return ListView.builder(
            itemCount: snapshot.data.docs.length,
            itemBuilder: (context, index) =>
            FloatingActionButton(
              //child: Text(index.),
              onPressed: (){
                print(snapshot.data.docs);
              },
            )
          );
        }
    );*/
    return FutureBuilder<DocumentSnapshot>(
      future: DatabaseService(gliderId).datas,
        builder: (context,snapshot) {

          if(!snapshot.hasData) return const Text("Loading....");
          //print(data.data());
          print("---------------------- datas ");
          //data.data().forEach((key, value) => {print(key)});
          return Container(
            child: FloatingActionButton(
              onPressed: (){
                print(gliderId);
                print(snapshot.data.data());
                //snapshot.data.data().forEach((key, value) => {print(key)});
              },
            ),
          );
        }
    );
  }
}


