import 'package:flutter/material.dart';
import 'package:hexcolor/hexcolor.dart';

class Home extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return SafeArea(
      child: Scaffold(
        body: Stack(
          alignment: Alignment.center,
          children: [
            Container(
              decoration: BoxDecoration(
                image: DecorationImage(
                  image: AssetImage("assets/jellyfish.jfif"),
                  fit: BoxFit.cover,
                ),
              ),
            ),
            Positioned(
              top: 180,
              child: RichText(
                textAlign: TextAlign.center,
                text: TextSpan(
                    text: "SIMULATION",
                    style: TextStyle(
                      fontSize: 40,
                      fontWeight: FontWeight.bold,
                      letterSpacing: 2.2,
                      color: Colors.white,
                    ),
                    children: [
                      TextSpan(
                        text: '\nA demo of communcation \nbetween MUG & APP'
                            .toUpperCase(),
                        style: TextStyle(
                          fontSize: 14,
                          color: Colors.grey[200],
                        ),
                      )
                    ]),
              ),
            ),
            Positioned(
                bottom: 160,
                child: ElevatedButton.icon(
                  label: Text('Go to demo'),
                  icon: Icon(Icons.arrow_right),
                  style: ElevatedButton.styleFrom(
                    primary: HexColor('#00C2FF'),
                    onPrimary: Colors.grey[200],
                    shadowColor: Colors.white,
                    elevation: 5,
                    textStyle: TextStyle(
                      fontSize: 20,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  onPressed: () {
                    Navigator.of(context).pushNamed('/dashboard');
                  },
                )),
          ],
        ),
      ),
    );
  }
}
