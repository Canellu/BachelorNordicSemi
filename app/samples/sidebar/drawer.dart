import 'package:flutter/material.dart';

import 'nagigatorPage.dart';

class Sidebar extends StatefulWidget {
  @override
  _SidebarState createState() => _SidebarState();
}

class _SidebarState extends State<Sidebar> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Sidebar Sample"),
      ),
      drawer: Drawer(
        child: ListView(
          children: <Widget>[
            UserAccountsDrawerHeader(
                accountName: Text('UserName'),
                accountEmail: Text('UserEmail'),
              currentAccountPicture: GestureDetector(
                //onTap: () => print('current user'),
                child: CircleAvatar(
                  backgroundImage: AssetImage('assets/cat.jpg'),
                ),
              ),
              decoration: BoxDecoration(
                image: DecorationImage(
                  fit: BoxFit.fill,
                  image: AssetImage('assets/cat.jpg'),
                ),
              ),
            ),
            ListTile(
              title: Text('First page'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                Navigator.of(context).pop(); //Close drawer first befor navigate
                Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('First Page')));
              },
            ),
            ListTile(
              title: Text('Second page'),
              trailing: Icon(Icons.arrow_forward_ios),
              onTap: (){
                Navigator.of(context).pop();
                Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) => NavgatorPage('Second Page')));
              },
            )
          ],
        ),
      ),
      body: Center(
        child: Text(
          'HomePage',
          style: TextStyle(fontSize: 30,fontWeight: FontWeight.bold),
        ),
      ),
    );
  }
}
