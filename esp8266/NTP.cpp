#include "NTP.h"
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>

const unsigned int NTP_RETRIES = 10;
const unsigned int localPort = 2390;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;

void sendNTPpacket(WiFiUDP& udp, IPAddress& address, byte* packetBuffer) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void NTPTime::init()
{
  IPAddress timeServerIP;
  byte packetBuffer[ NTP_PACKET_SIZE];

  WiFiUDP udp;
  udp.begin(localPort);

  for (unsigned int i = 0; i < NTP_RETRIES; ++i)
  {
    //get a random server from the pool
    WiFi.hostByName(ntpServerName, timeServerIP);

    sendNTPpacket(udp, timeServerIP, packetBuffer); // send an NTP packet to a time server
    // wait to see if a reply is available
    Serial.println("syncing time...");
    delay(1000);

    int cb = udp.parsePacket();
    if (cb) 
    {
      udp.read(packetBuffer, NTP_PACKET_SIZE);

      //the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, esxtract the two words:
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;

      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
      const unsigned long seventyYears = 2208988800UL;
      // subtract seventy years:
      unsigned long epoch = secsSince1900 - seventyYears;
      m_time = epoch;
      m_timeRecievedAt = millis();
      return; 
    }
  }
}

unsigned long NTPTime::getTimeStamp() const
{
  return m_time + (millis() - m_timeRecievedAt) / 1000;
}
