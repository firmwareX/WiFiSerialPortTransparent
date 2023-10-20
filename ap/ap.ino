#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef APSSID
#define APSSID "B612"
#define APPSK "f6d78527d0"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

unsigned int localPort = 8888;  // local port to listen on
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

WiFiUDP udp;
IPAddress host;

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  udp.begin(localPort);
}

void sand(char inChar){
  if(udp.beginPacket(host,8888)){
    udp.write(inChar);
    udp.endPacket();
  }
}

void loop() {
  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    //Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());

    // read the packet into packetBufffer
    int len = udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[len] = 0; //'\0'

    Serial.write(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    host = udp.remoteIP();
    //udp.beginPacket(udp.remoteIP(), udp.remotePort());
    //udp.write(ReplyBuffer);
    //udp.endPacket();
  }
  if (Serial.available() > 0) {
    // read incoming serial data:
    char inChar = Serial.read();
    sand(inChar);
  }
}
