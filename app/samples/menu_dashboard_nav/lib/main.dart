import 'package:bachelor_app/UI/pages/map_screen.dart';
import 'package:bachelor_app/menu_dashboard_layout/menu_dashboard_layout.dart';
import 'package:flutter/material.dart';

void main() {
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
      home: MapScreen(),
      /*MenuDashboardLayout()*/
    );
  }
}
