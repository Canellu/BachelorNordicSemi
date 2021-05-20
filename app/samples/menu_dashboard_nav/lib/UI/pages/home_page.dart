import 'package:bachelor_app/bloc/nav_bloc/navigation_bloc.dart';
import 'package:bachelor_app/models/device.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:hexcolor/hexcolor.dart';
import 'package:provider/provider.dart';
import '../deviceTile.dart';
import 'map_screen.dart';
import 'package:flutter_svg/flutter_svg.dart';

class HomePage extends StatelessWidget with NavigationStates {
  final Function onMenuTap;

  const HomePage({Key key, this.onMenuTap}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    setOrientation();
    final device = Provider.of<List<Device>>(context) ?? [];
    return Scaffold(
      appBar: AppBar(
        title: Text(
          'OASYS',
          style: TextStyle(
            letterSpacing: 3,
            fontWeight: FontWeight.bold,
          ),
        ),
        centerTitle: true,
        elevation: 20,
        flexibleSpace: Container(
          decoration: BoxDecoration(
            /*
                    gradient: LinearGradient(
                        colors: [Colors.greenAccent, Colors.blue],
                        begin: Alignment.bottomRight,
                        end: Alignment.topLeft
                    )*/
            color: const Color.fromRGBO(31, 41, 55, 1),
          ),
        ),
      ),
      drawer: Drawer(
        child: ListView(
          //important : any padding will make statusBar not transparent
          padding: EdgeInsets.zero,
          children: <Widget>[
            UserAccountsDrawerHeader(
              accountName: Text('UserName'),
              accountEmail: Text('nordicoasys@gmail.com'),
              currentAccountPicture: GestureDetector(
                //onTap: () => print('current user'),
                child: CircleAvatar(
                  backgroundImage: AssetImage('assets/images/OASYS_dark.jpg'),
                ),
              ),
              decoration: BoxDecoration(
                image: DecorationImage(
                  fit: BoxFit.fill,
                  image: AssetImage('assets/images/underWater.jpg'),
                ),
              ),
            ),
            SizedBox(height: 10),
            ListTile(
              leading: Icon(Icons.account_circle),
              title: Text('My Profile'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                Navigator.of(context).pop(); //Close drawer first befor navigate
                //Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('First Page')));
              },
            ),
            ListTile(
              leading: Icon(Icons.library_add_rounded),
              title: Text('Add Device'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                Navigator.of(context).pop(); //Close drawer first befor navigate
                //Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('First Page')));
              },
            ),
            ListTile(
              leading: Icon(Icons.settings),
              title: Text('Settings'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                Navigator.of(context).pop();
                //Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('Second Page')));
              },
            ),
            SizedBox(height: MediaQuery.of(context).size.height / 3),
            Divider(color: Colors.grey, thickness: 1),
            ListTile(
              title: Text('About OASYS'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                Navigator.of(context).pop();
                //Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('Second Page')));
              },
            ),
            ListTile(
              title: Text(
                  'Logout',
                style: TextStyle(
                  color: Colors.red,
                ),
              ),
              onTap: (){
                Navigator.of(context).pop();
                //Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('Second Page')));
              },
            ),
          ],
        ),
      ),
      body: Container(
        color: HexColor("F3F4F6"),
        //padding: const EdgeInsets.only(left: 16, right: 16, top: 48),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          mainAxisSize: MainAxisSize.min,
          children: <Widget>[
            SizedBox(height: 5),
            Container(
              height: 250,
              child: PageView(
                controller: PageController(viewportFraction: 1),
                scrollDirection: Axis.horizontal,
                pageSnapping: true,
                children: <Widget>[
                  Container(
                      child: MapScreen(device),
                  ),
                ],
              ),
            ),
            /* The down-half of the screen)*/
            //SizedBox(height: 10),
            Padding(
              padding: EdgeInsets.only(left: 15),
              child: Text(
                "Active devices",
                style: TextStyle(color: HexColor('#1f2937'), fontSize: 20),
              ),
            ),
            Container(
              height: MediaQuery.of(context).size.height/2,
              width: MediaQuery.of(context).size.width,
              padding: EdgeInsets.fromLTRB(10, 10, 10, 0),
              child: ListView.builder(
                //Needed shrinkWarp and physics for listView can be use in SingleChildScroll
                shrinkWrap: true,
                //physics: ClampingScrollPhysics(),
                physics: AlwaysScrollableScrollPhysics(),
                itemCount: device.length,
                itemBuilder: (context, index) {
                  return DeviceTile(oneDevice: device[index]);
                },
              ),
            ),
            /*SizedBox(height: 20), //Place holder for the device widgets*/
          ],
        ),
      ),
    );
/*

    return Container(
      decoration: BoxDecoration(
        //borderRadius: BorderRadius.all(Radius.circular(40)),
        color: Colors.grey,
        //color: HexColor("F3F4F6")
      ),
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
            Container(
              height: MediaQuery.of(context).size.height/2,
              child: ListView.builder(
                //Needed shrinkWarp and physics for listView can be use in SingleChildScroll
                shrinkWrap: true,
                //physics: ClampingScrollPhysics(),
                physics: AlwaysScrollableScrollPhysics(),
                itemCount: device.length,
                itemBuilder: (context, index) {
                  return DeviceTile(oneDevice: device[index]);
                },
              ),
            ),
            /*SizedBox(height: 20), //Place holder for the device widgets*/
          ],
        ),
      ),
    );*/
  }

  void setOrientation() {
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitUp,
      DeviceOrientation.portraitDown,
    ]);
  }
}
