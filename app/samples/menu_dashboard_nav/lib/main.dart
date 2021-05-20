import 'package:bachelor_app/menu_dashboard_layout/menu_dashboard_layout.dart';
import 'package:bachelor_app/service/database.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:provider/provider.dart';
import 'package:firebase_core/firebase_core.dart';

import 'UI/pages/loadingPage.dart';
import 'models/device.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
  //Set status bar transparent
  SystemUiOverlayStyle systemUiOverlayStyle = SystemUiOverlayStyle(
    //systemNavigationBarColor: Color(0xFF000000),
    //systemNavigationBarDividerColor: null,
    statusBarColor: Colors.transparent,
    //systemNavigationBarIconBrightness: Brightness.light,
    statusBarBrightness: Brightness.dark,
    // To make Status bar icons color white in Android devices.
    statusBarIconBrightness: Brightness.dark,
    /*
    statusBarColor: Colors.transparent,
    statusBarBrightness: Brightness.light,
    statusBarIconBrightness: Brightness.light,
    //statusBarIconBrightness: Brightness.dark,*/
  );
  SystemChrome.setSystemUIOverlayStyle(systemUiOverlayStyle);
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
      builder: (BuildContext context, Widget widget) {
        /*
        Widget error = Text('...rendering error...');
        if(widget is Scaffold || widget is Navigator)
          error = Scaffold(body: Center(child: error,));
          ErrorWidget.builder =
              (FlutterErrorDetails errorDetails) => error;
          return widget;*/
        ErrorWidget.builder = (FlutterErrorDetails errorDetails) {
          return ErrorLoading();
        };
        return widget;
      },
      home: HomeRun(),
      /*MenuDashboardLayout()*/
    );
  }
}

class HomeRun extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return StreamProvider<List<Device>>.value(
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
      ),
    );
  }
}

