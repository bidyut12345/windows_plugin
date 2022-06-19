import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class WindowsPlugin {
  static const MethodChannel _channel = MethodChannel('windows_plugin');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<String?> get getPrinterList async {
    final String? version = await _channel.invokeMethod('getPrinterList');
    return version;
  }

  static Future<Color> get chooseColor async {
    final String? color = await _channel.invokeMethod('chooseColor');
    return integerToColor(int.parse(color!));
  }

  static Future<void> setTitleBarColor(int color) async {
    final Map<String, dynamic> arguments = {
      'color': color,
    };
    await _channel.invokeMethod('setTitleBarColor', arguments);
  }

  static Future<void> setBlur() async {
    await _channel.invokeMethod('setBlur');
  }

  static Future<void> setUnBlur() async {
    await _channel.invokeMethod('setUnBlur');
  }

  static Future<void> createNewWindow() async {
    await _channel.invokeMethod('createNewWindow');
  }

  static Future<String> printPdf(String filePath, String printerName,
      {int? pageNumber, bool landscape = false}) async {
    final String res =
        (await _channel.invokeMethod('printPdf', <String, dynamic>{
      'path': filePath,
      'printer': printerName,
      'number': pageNumber != null ? (pageNumber - 1).toString() : "ALL_PAGES",
      'orientation': landscape ? "LANDSCAPE_PAGE" : "PORTRAIT",
    }))
            .toString();
    return res;
  }

  static Future<String> printText(
      {required String texttoprint,
      String printerName = "",
      int fontSize = 10,
      Color color = Colors.black,
      String fontFace = "Lucida Console"}) async {
    final String res =
        (await _channel.invokeMethod('printText', <String, dynamic>{
      'texttoprint': texttoprint,
      'printer': printerName,
      'fontSize': fontSize,
      'color': colorToInteger(color),
      'fontFace': fontFace,
    }))
            .toString();
    return res;
  }

  static Color integerToColor(int dec) {
    // dec += 1;16776960
    int bb = (dec / 65536).floor();
    dec -= bb * 65536;
    int gg = (dec / 256).floor();
    dec -= gg * 256;
    int rr = dec;
    // rr = Math.min(rr, 255);
    return Color.fromARGB(255, rr, gg, bb);
  }

  static int colorToInteger(Color col) {
    int dec =
        (col.red + 0) + ((col.green + 0) * 256) + ((col.blue + 0) * 65536) - 0;
    return dec;
  }

  static int hexToInt(String hex) {
    int val = 0;
    int len = hex.length;
    for (int i = 0; i < len; i++) {
      int hexDigit = hex.codeUnitAt(i);
      if (hexDigit >= 48 && hexDigit <= 57) {
        val += (hexDigit - 48) * (1 << (4 * (len - 1 - i)));
      } else if (hexDigit >= 65 && hexDigit <= 70) {
        // A..F
        val += (hexDigit - 55) * (1 << (4 * (len - 1 - i)));
      } else if (hexDigit >= 97 && hexDigit <= 102) {
        // a..f
        val += (hexDigit - 87) * (1 << (4 * (len - 1 - i)));
      } else {
        throw const FormatException("Invalid hexadecimal value");
      }
    }
    return val;
  }

  static colorToHexString(Color color) {
    return '#FF${color.value.toRadixString(16).substring(2, 8)}';
  }
}
