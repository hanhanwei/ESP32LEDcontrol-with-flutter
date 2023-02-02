import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_sliders/sliders.dart';
import 'package:flutter_blue/flutter_blue.dart';
import 'package:intl/intl.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  double height = 70;
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("LED control"),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text("Brightness: ${height}", style: TextStyle(fontSize: 30),),
            ElevatedButton(
              onPressed: (){
                showModalBottomSheet(context: context,
                    builder: (BuildContext context){
                      return Container(
                        height: 200,
                        child: Center(child:
                        StatefulBuilder(
                          builder: (context,state){
                            return Center(
                              child: SfSlider(
                                value: height.toDouble(),
                                max: 100,
                                min: 50,
                                showTicks: true,
                                showLabels: true,
                                enableTooltip: true,
                                onChanged: (value) {
                                  state(() {
                                    height = value.round() as double;
                                  });
                                  setState(() {
                                  });
                                },
                              ),
                            );
                          },
                        ),
                        ),
                      );
                    }
                );
              },
              child: Text(" Select Value "),
            ),
          ],
        ),
      ),
    );
  }
}