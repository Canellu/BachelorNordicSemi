import 'package:bachelor_app/models/device.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'deviceTile.dart';

class DeviceList extends StatefulWidget {
  @override
  _DeviceListState createState() => _DeviceListState();
}

class _DeviceListState extends State<DeviceList> {
  @override
  Widget build(BuildContext context) {

    final device = Provider.of<List<Device>>(context);
    device.forEach((device) {
      print(device.added);
      print(device.alias);
      print(device.lastSync);
    });

    return ListView.builder(
      itemCount: device.length,
      itemBuilder: (context,index){
        return DeviceTile(oneDevice: device[index]);
      },
    );
  }
}