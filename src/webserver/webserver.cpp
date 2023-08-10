#include "A_Config.h"
#include "WebServer.h"

//下面是引用网页文件
#include "index.h"
#include "csss.h"
#include "jss.h"
#include "favicon.h"
WebServer server(80);
void addFileSystemHandler();
void beginWebServer()
{
    addFileSystemHandler();
    server.on("/", HTTP_GET, []() {
        server.sendHeader("Content-Encoding", "gzip");
        server.send_P(200, "text/html", __index_html_gz, __index_html_gz_len);
    });
    server.on("/favicon.ico", HTTP_GET, []() {
        server.sendHeader("Content-Encoding", "gzip");
        server.send_P(200, "image/x-icon", __favicon_ico_gz, __favicon_ico_gz_len);
    });
    server.on("/css/csss.css", HTTP_GET, []() {
        server.sendHeader("Content-Encoding", "gzip");
        server.send_P(200, "text/css", __csss_css_gz, __csss_css_gz_len);
    });
    server.on("/js/jss.js", HTTP_GET, []() {
        server.sendHeader("Content-Encoding", "gzip");
        server.send_P(200, "application/javascript", __jss_js_gz, __jss_js_gz_len);
    });
    server.on("/info", HTTP_GET, []() {
        String message;
        message += "IP地址: ";
        message += WiFi.localIP().toString();
        refreshIPV6Addr();
        if(strcmp(ipv6_to_str(&ipv6global) ,"::") != 0)
        {
            message += "<br/>IPv6地址: ";
            message += ipv6_to_str(&ipv6global);
        }
        message += "<br/>MAC地址: ";
        message += WiFi.macAddress();
        message += "<br/>系统时间: ";
        char strftime_buf[64];
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &hal.timeinfo);
        message += strftime_buf;
        message += "<br/>ADC电压：";
        message += hal.VCC;
        message += "<br/>USB";
        message += hal.USBPluggedIn?"已插入":"未插入";
        message += hal.isCharging?"<br/>正在充电":"<br/>未充电";
        server.send(200, "text/plain", message);
    });
    server.on("/conf", HTTP_POST, []() {
        deserializeJson(config, server.arg(0));
        server.send(200, "text/plain", "OK");
        hal.saveConfig();
    });
    server.on("/poweroff", HTTP_POST, []() {
        server.send(200, "text/plain", "OK");
        delay(50);
        hal.powerOff();
    });
    server.on("/config.json", HTTP_GET, []() {
        server.send(200, "application/json", config.as<String>());
    });
    server.on("/reset", HTTP_POST, []() {
        server.send(200, "text/plain", "OK");
        if(server.args() == 1)
        {
            delay(100);
            ESP.restart();
        }
    });
    
    server.begin();
    Serial.println("HTTP server started");
}
void updateWebServer()
{
    server.handleClient();
}

///////////////////////////////////////////////
///////下面是文件管理
/*版权信息：
  FSWebServer - Example WebServer with FS backend for esp8266/esp32
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the WebServer library for Arduino environment.
  开源协议：LGPL 2.1
*/
///////////////////////////////////////////////


//holds the current upload
#define FILESYSTEM SPIFFS
#define DBG_OUTPUT_PORT Serial
File fsUploadFile;

//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool exists(String path){
  bool yes = false;
  File file = FILESYSTEM.open(path, "r");
  if(!file.isDirectory()){
    yes = true;
  }
  file.close();
  return yes;
}

bool handleFileRead(String path) {
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (exists(pathWithGz) || exists(path)) {
    if (exists(pathWithGz)) {
      path += ".gz";
    }
    File file = FILESYSTEM.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Name: "); DBG_OUTPUT_PORT.println(filename);
    fsUploadFile = FILESYSTEM.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}

void handleFileDelete() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (!exists(path)) {
    return server.send(404, "text/plain", "FileNotFound");
  }
  FILESYSTEM.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (exists(path)) {
    return server.send(500, "text/plain", "FILE EXISTS");
  }
  File file = FILESYSTEM.open(path, "w");
  if (file) {
    file.close();
  } else {
    return server.send(500, "text/plain", "CREATE FAILED");
  }
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  DBG_OUTPUT_PORT.println("handleFileList: " + path);


  File root = FILESYSTEM.open(path);
  path = String();

  String output = "[";
  if(root.isDirectory()){
      File file = root.openNextFile();
      while(file){
          if (output != "[") {
            output += ',';
          }
          output += "{\"type\":\"";
          output += (file.isDirectory()) ? "dir" : "file";
          output += "\",\"name\":\"";
          output += String(file.path()).substring(1);
          output += "\"}";
          file = root.openNextFile();
      }
  }
  output += "]";
  server.send(200, "text/json", output);
}
#include "edit.h"
#include "ace.h"
void addFileSystemHandler()
{
  //SERVER INIT
  server.on("/js/ace.js", HTTP_GET, []() {
      server.sendHeader("Content-Encoding", "gzip");
      server.send_P(200, "application/javascript", __ace_js_gz, __ace_js_gz_len);
  });
  //list directory
  server.on("/list", HTTP_GET, handleFileList);
  //load editor
  server.on("/edit", HTTP_GET, []() {
    server.sendHeader("Content-Encoding", "gzip");
    server.send_P(200, "text/html", __edit_htm_gz, __edit_htm_gz_len);
  });
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from FILESYSTEM
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(0));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
}