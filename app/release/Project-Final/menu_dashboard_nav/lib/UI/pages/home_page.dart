
import 'package:bachelor_app/models/device.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:hexcolor/hexcolor.dart';
import 'package:provider/provider.dart';
import '../compoment/deviceTile.dart';
import '../compoment/map_screen.dart';

class HomePage extends StatelessWidget {
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
            color: const Color.fromRGBO(31, 41, 55, 1),
          ),
        ),
      ),
      //-------Meny bar-----------
      drawer: Drawer(
        child: ListView(
          //important : any padding will make statusBar not transparent
          padding: EdgeInsets.zero,
          children: <Widget>[
            UserAccountsDrawerHeader(
              accountName: Text('UserName'),
              accountEmail: Text('nordicoasys@gmail.com'),
              currentAccountPicture: GestureDetector(
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
                //Navigtion-bar close
                Navigator.of(context).pop();
              },
            ),
            ListTile(
              leading: Icon(Icons.library_add_rounded),
              title: Text('Add Device'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                //Navigtion-bar close
                Navigator.of(context).pop();
              },
            ),
            ListTile(
              leading: Icon(Icons.settings),
              title: Text('Settings'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                //Navigtion-bar close
                Navigator.of(context).pop();
              },
            ),
            SizedBox(height: MediaQuery.of(context).size.height / 3),
            Divider(color: Colors.grey, thickness: 1),
            ListTile(
              title: Text('About OASYS'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                //Navigtion-bar close
                Navigator.of(context).pop();
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
                //Navigtion-bar close
                Navigator.of(context).pop();
              },
            ),
          ],
        ),
      ),
      //--------Meny bar end---------
      body: Container(
        height: MediaQuery.of(context).size.height,
        color: HexColor("F3F4F6"),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisSize: MainAxisSize.min,
          children: <Widget>[
            SizedBox(height: 5),
            //--------Google Map-----------
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
            Padding(
              padding: EdgeInsets.fromLTRB(15, 5, 0, 5),
              child: Text(
                "Active devices",
                style: TextStyle(
                    color: HexColor('#1f2937'),
                    fontSize: 20,
                  letterSpacing: 2,
                  fontWeight: FontWeight.bold,
                ),
              ),
            ),
            //---------List of gliders----------
            Container(
              height: MediaQuery.of(context).size.height/1.8,
              width: MediaQuery.of(context).size.width,
              padding: EdgeInsets.fromLTRB(10, 0, 10, 0),
              child: ListView.builder(
                //Needed shrinkWarp and physics for listView can be use in SingleChildScroll
                shrinkWrap: true,
                physics: AlwaysScrollableScrollPhysics(),
                itemCount: device.length,
                itemBuilder: (context, index) {
                  return DeviceTile(oneDevice: device[index]);
                },
              ),
            ),
          ],
        ),
      ),
    );
  }

  //--------Only allow vertical version orientation--------
  void setOrientation() {
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitUp,
      DeviceOrientation.portraitDown,
    ]);
  }
}
