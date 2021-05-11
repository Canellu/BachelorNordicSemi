import 'package:bachelor_app/bloc/nav_bloc/navigation_bloc.dart';
import 'package:bachelor_app/models/device.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../deviceTile.dart';
import 'map_screen.dart';

class HomePage extends StatelessWidget with NavigationStates {
  final Function onMenuTap;

  const HomePage({Key key, this.onMenuTap}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final device = Provider.of<List<Device>>(context);
    /*
    device.forEach((device) {
      /*
      print(device.added);
      print(device.alias);
      print(device.lastSync);*/
      print(device.deviceId);
      print(device.lastSeen);
    });*/
    return Container(
      decoration: BoxDecoration(
        borderRadius: BorderRadius.all(Radius.circular(40)),
        color: Colors.grey,
      ),
      child: SingleChildScrollView(
        scrollDirection: Axis.vertical,
        physics: ClampingScrollPhysics(),
        child: Container(
          padding: const EdgeInsets.only(left: 16, right: 16, top: 48),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                mainAxisSize: MainAxisSize.max,
                children: [
                  InkWell(
                    child: Icon(Icons.menu, color: Colors.white),
                    onTap: onMenuTap,
                  ),
                  Text("Home page",
                      style: TextStyle(fontSize: 24, color: Colors.white)),
                  Icon(Icons.settings, color: Colors.white),
                ],
              ),
              SizedBox(height: 20),
              Container(
                height: 200,
                child: PageView(
                  controller: PageController(viewportFraction: 1),
                  scrollDirection: Axis.horizontal,
                  pageSnapping: true,
                  children: <Widget>[
                    Container(child: MapScreen(device)),
                  ],
                ),
              ),
              /* The down-half of the screen)*/
              SizedBox(height: 20),
              Text(
                "Active devices",
                style: TextStyle(color: Colors.white, fontSize: 20),
              ),
              ListView.builder(
                //Needed shrinkWarp and physics for listView can be use in SingleChildScroll
                shrinkWrap: true,
                physics: ClampingScrollPhysics(),
                itemCount: device.length,
                itemBuilder: (context, index) {
                  return DeviceTile(oneDevice: device[index]);
                },
              ),
              /*SizedBox(height: 20), //Place holder for the device widgets*/
            ],
          ),
        ),
      ),
    );
    /*
    return ListView.builder(
      itemCount: device.length,
      itemBuilder: (context,index){
        return DeviceTile(oneDevice: device[index]);
      },
    );*/
  }
}
