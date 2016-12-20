/*
 * Arduino 1.6.6
 * Sketch uses 237,464 bytes (45%) of program storage space. Maximum is 524,288 bytes.
 */


#include <ESP8266WiFi.h>
#include <Wire.h>


const char* ssid     = "KryptiskEgyptisk";
const char* password = "CDF9FF1567";
// const char* host = "utcnist2.colorado.edu";
const char* host = "129.6.15.30";
//http://tf.nist.gov/tf-cgi/servers.cgi

int ln = 0;
String TimeDate = "";


void setup() {
  Serial.begin(115200);                   // diagnostic channel
  //Serial.begin(74880);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  delay(100);

  //Wire.pins(int sda, int scl), etc
  Wire.pins(0, 2);                        //on ESP-01.
  Wire.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 13;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 NodeMcu Lua;)\r\n\r\n");

  delay(1000);

  // Read all the lines of the reply from server and print them to Serial
  // expected line is like : Date: Thu, 01 Jan 2015 22:00:14 GMT
  char buffer[12];
  String dateTime = "";
  //sendStrXY(">  Listening...<", 2, 0);

  while(client.available())
  {
    String line = client.readStringUntil('\r');

    if (line.indexOf("Date") != -1)
    {
      Serial.print("=====>");
    } else
    {
      Serial.print(line);
      // date starts at pos 7
      TimeDate = line.substring(7);
      //Serial.println(TimeDate);

      // time starts at pos 14
      //TimeDate = line.substring(7, 15);
      String time = line.substring(16, 21);
      Serial.println(time);
      //TimeDate.toCharArray(buffer, 10);
      Serial.print(TimeDate);
      // TimeDate = line.substring(16, 24);
      // TimeDate.toCharArray(buffer, 10);
      // Serial.println(TimeDate);
      //sendStrXY(buffer, 6, 0);
      int clock0 = line.substring(16,17).toInt();
      Serial.print("first: ");
      Serial.println(clock0);
      int clock1 = 1+line.substring(17,18).toInt();
      Serial.print("second: ");
      Serial.println(clock1);
      int clock2 = line.substring(19,20).toInt();
      Serial.print("third: ");
      Serial.println(clock2);
      int clock3 = line.substring(20,21).toInt();
      Serial.print("fourth: ");
      Serial.println(clock3);
    }
  }

  Serial.println();
  Serial.println("closing connection");
}
