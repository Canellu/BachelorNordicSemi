import 'package:bachelor_app/menu_dashboard_layout/menu_dashboard_layout.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:firebase_core/firebase_core.dart';
//import 'dart:html';
import 'models/device.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      //Remove the debug banner at the right top
      debugShowCheckedModeBanner: false,
      title: 'Menu and Dashbord',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: HomeRun(),
      /*MenuDashboardLayout()*/
    );
  }
}

class HomeRun extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return StreamProvider<List<Device>>.value(
<<<<<<< HEAD
      value: DatabaseService("", "").glider,
      child: MaterialApp(
        //Remove the debug banner at the right top
        debugShowCheckedModeBanner: false,
        home: MenuDashboardLayout(),
=======
      value: DatabaseService("","").glider,
      child: GestureDetector(
        onTap: (){
          FocusScopeNode currentFocus = FocusScope.of(context);
          if (!currentFocus.hasPrimaryFocus && currentFocus.focusedChild != null) {
            FocusManager.instance.primaryFocus.unfocus();
          }
        },
        child: MaterialApp(
          //Remove the debug banner at the right top
          debugShowCheckedModeBanner: false,
          home: MenuDashboardLayout(),
        ),
>>>>>>> b7a094108ddbfcb4eae47ae512b277f0e7e475ea
      ),
    );
  }
}
