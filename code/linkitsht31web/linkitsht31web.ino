#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiServer.h>
#include <LWiFiClient.h>

//change these
#define WIFI_AP "username"
#define WIFI_PASSWORD "password"
#define WIFI_AUTH LWIFI_WPA                 // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your WiFi AP configuration


LWiFiServer server(80);
Adafruit_SHT31 sht31 = Adafruit_SHT31();

float t,h;
int outputpin= A0;


float getTemperature()
{
    t = sht31.readTemperature();
    if (! isnan(t)) 
    {
      Serial.print("Temp *C = "); Serial.println(t);
    } 
    else 
    { 
      Serial.println("Failed to read temperature");
    }
}

void getHumidity()
{
    h = sht31.readHumidity();
    if (! isnan(h)) 
    {
      Serial.print("Hum. % = "); Serial.println(h);
    } 
    else 
    { 
      Serial.println("Failed to read humidity");
    }
    Serial.println();
}

void setup()
{
    LTask.begin();
    LWiFi.begin();
    Serial.begin(9600);
    //while(!Serial.available());     // input anything to start the program
    if (! sht31.begin(0x44)) 
    {
      Serial.println("Couldn't find SHT31");
      while (1) delay(1);
    }

    // keep retrying until connected to AP
    Serial.println("Connecting to AP");
    while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
    {
        delay(1000);
    }

    printWifiStatus();

    Serial.println("Start Server");
    server.begin();
    Serial.println("Server Started");
}

int loopCount = 0;

void loop()
{
    // put your main code here, to run repeatedly:
    delay(500);
    loopCount++;
    LWiFiClient client = server.available();
        
    getTemperature();
    getHumidity();
    if (client)
    {
        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        
        unsigned long timer_out = millis();
        
        while (client.connected())
        {
        
            if(millis()-timer_out > 5000)break;
            
            if (client.available())
            {
                // we basically ignores client request, but wait for HTTP request end
                int c = client.read();
                Serial.print((char)c);

                if (c == '\n' && currentLineIsBlank)
                {
                    Serial.println("send response");
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    client.println("<h2>Temperature and humidity Example</h2>");
                    client.println("<table border=1>");
                    client.println("<tr><td>Temperature</td><td>");
                    client.println(t);
                    client.println("</td></tr>");
                    client.println("<tr><td>Humidity</td><td>");
                    client.println(h);
                    client.println("</td></tr>");
                    client.println("</table>");
                    client.println("</html>");
                    client.println();
                    break;
                }
                if (c == '\n')
                {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }
                else if (c != '\r')
                {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(500);

        // close the connection:
        Serial.println("close connection");
        client.stop();
        Serial.println("client disconnected");
    }
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(LWiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = LWiFi.localIP();
    Serial.println("Please open your browser, and input the following address:");
    Serial.println(ip);

    Serial.print("\r\nsubnet mask: ");
    Serial.println(LWiFi.subnetMask());

    Serial.print("gateway IP: ");
    Serial.println(LWiFi.gatewayIP());

    // print the received signal strength:
    long rssi = LWiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
