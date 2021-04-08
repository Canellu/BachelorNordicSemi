import 'package:bachelor_app/bloc/nav_bloc/navigation_bloc.dart';
import 'package:flutter/material.dart';

class HomePage extends StatelessWidget with NavigationStates {
  final Function onMenuTap;

  const HomePage({Key key, this.onMenuTap}) : super(key: key);

  @override
  Widget build(BuildContext context) {
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
              SizedBox(height: 50),
              Container(
                height: 200,
                child: PageView(
                  controller: PageController(viewportFraction: 0.8),
                  scrollDirection: Axis.horizontal,
                  pageSnapping: true,
                  children: <Widget>[
                    Container(
                      margin: const EdgeInsets.symmetric(horizontal: 8),
                      color: Colors.white,
                      width: 100,
                    ),
                  ],
                ),
              ),
              /* The down-half of the screen)*/
              SizedBox(height: 20),
              Text(
                "Active devices",
                style: TextStyle(color: Colors.white, fontSize: 20),
              ),
              /*SizedBox(height: 20), //Place holder for the device widgets*/
            ],
          ),
        ),
      ),
    );
  }
}
