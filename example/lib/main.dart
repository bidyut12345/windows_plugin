import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:windows_plugin/windows_plugin.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  String _printerList = 'Unknown';
  int _color = 0;
  Color? _color2 = Colors.white;

  String? _ddlValue = null;

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  Future<void> initPlatformState() async {
    String platformVersion;
    try {
      platformVersion =
          await WindowsPlugin.platformVersion ?? 'Unknown platform version';
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    try {
      _printerList =
          await WindowsPlugin.getPrinterList ?? 'Unknown printer list.';
    } on PlatformException {
      _printerList = 'Failed to get printer.';
    }

    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        // backgroundColor: Colors.transparent,
        backgroundColor: _color2,
        appBar: AppBar(
          backgroundColor: _color2!.withOpacity(0.8),
          title: const Text('Plugin example app'),
          // flexibleSpace: Container(
          //   height: 250,
          //   color: Colors.red.withOpacity(0.2),
          // ),
        ),
        body: Column(
          children: [
            Center(
              child: Text('Running on: $_platformVersion\n'),
            ),
            Center(
              child: Text('Printer list: $_printerList\n'),
            ),
            Center(
              child: Text('Selected Color: $_color\n'),
            ),
            DropdownButton(
                isExpanded: true,
                value: _ddlValue,
                items: _printerList
                    .split(";")
                    .map((e) => DropdownMenuItem(
                          child: Text(e),
                          value: e.toString(),
                        ))
                    .toList(),
                onChanged: (String? val) {
                  _ddlValue = val;
                  setState(() {});
                }),
            ElevatedButton(
              onPressed: () async {
                // _color = await WindowsPlugin.chooseColor;
                _color2 = await WindowsPlugin.chooseColor;
                _color = WindowsPlugin.colorToInteger(_color2!);
                await WindowsPlugin.setTitleBarColor(_color);
                setState(() {});
              },
              child: const Text("Choose color"),
            ),
            ElevatedButton(
              onPressed: () async {
                _color = WindowsPlugin.colorToInteger(_color2!);
                await WindowsPlugin.setTitleBarColor(_color);
                setState(() {});
              },
              child: const Text("Set Title Bar color"),
            ),
            ElevatedButton(
              onPressed: () async {
                // _color = await WindowsPlugin.setTitleBarColor(14919390);
                _color2 = Colors.transparent;
                await WindowsPlugin.setBlur();
                setState(() {});
              },
              child: const Text("setBlur"),
            ),
            ElevatedButton(
              onPressed: () async {
                // _color = await WindowsPlugin.setTitleBarColor(14919390);
                _color = WindowsPlugin.colorToInteger(_color2!);
                await WindowsPlugin.setUnBlur();
                setState(() {});
              },
              child: const Text("setUnBlur"),
            ),
            ElevatedButton(
              onPressed: () async {
                await WindowsPlugin.printText(
                  texttoprint: "This is bidyut das",
                );
              },
              child: const Text("printText"),
            ),
            ElevatedButton(
              onPressed: () async {
                await WindowsPlugin.createNewWindow();
              },
              child: const Text("createNewWindow"),
            ),
            ElevatedButton(
              onPressed: () async {
                await WindowsPlugin.printPdf("", "");
                debugPrint("okay");
              },
              child: const Text("Print PDF"),
            ),
          ],
        ),
      ),
    );
  }
}
