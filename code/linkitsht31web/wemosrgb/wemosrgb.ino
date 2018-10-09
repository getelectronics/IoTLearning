#include <ESP8266WiFi.h>
 
const char* ssid = "iainhendry";
const char* password = "iain061271";
 
int redPin = D5;
int greenPin = D6;
int bluePin = D7;

WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200);
  delay(10);
 
  //set up LEDs
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, LOW);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, LOW);
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
    
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // Match the request
  int redvalue = LOW;
  if (request.indexOf("/REDLED=ON") != -1) {
    digitalWrite(redPin, HIGH);
    redvalue = HIGH;
  } 
  if (request.indexOf("/REDLED=OFF") != -1){
    digitalWrite(redPin, LOW);
    redvalue = LOW;
  }

  int greenvalue = LOW;
  if (request.indexOf("/GREENLED=ON") != -1) {
    digitalWrite(greenPin, HIGH);
    greenvalue = HIGH;
  } 
  if (request.indexOf("/GREENLED=OFF") != -1){
    digitalWrite(greenPin, LOW);
    greenvalue = LOW;
  }

  int bluevalue = LOW;
  if (request.indexOf("/BLUELED=ON") != -1) {
    digitalWrite(bluePin, HIGH);
    bluevalue = HIGH;
  } 
  if (request.indexOf("/BLUELED=OFF") != -1){
    digitalWrite(bluePin, LOW);
    bluevalue = LOW;
  }
   
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
   
  client.print("Red led pin is now: ");
  if(redvalue == HIGH) {
    client.print(" On");  
  } else {
    client.print(" Off");
  }
  client.println("<br>");
  
  client.print("Green led pin is now: ");   
  if(greenvalue == HIGH) {
    client.print(" On");  
  } else {
    client.print(" Off");
  }
  client.println("<br>");
  
  client.print("Blue led pin is now: ");  
  if(bluevalue == HIGH) {
    client.print(" On");  
  } else {
    client.print(" Off");
  }
  client.println("<br>");
  
  client.println("<table>");
  client.println("<tr><td>");
  client.println("<a href=\"/REDLED=ON\"><input type=\"button\" value=\"RED LED ON\"></a><br>");
  client.println("</td><td>");
  client.println("<a href=\"/REDLED=OFF\"><input type=\"button\" value=\"RED LED OFF\"></a><br>");
  client.println("</td></tr>");
  client.println("<tr><td>");
  client.println("<a href=\"/GREENLED=ON\"><input type=\"button\" value=\"GREEN LED ON\"></a><br>");
  client.println("</td><td>");
  client.println("<a href=\"/GREENLED=OFF\"><input type=\"button\" value=\"GREEN LED OFF\"></a><br>");
  client.println("</td></tr>");
  client.println("<tr><td>");
  client.println("<a href=\"/BLUELED=ON\"><input type=\"button\" value=\"BLUE LED ON\"></a><br>");
  client.println("</td><td>");
  client.println("<a href=\"/BLUELED=OFF\"><input type=\"button\" value=\"BLUE LED OFF\"></a><br>");
  client.println("</td></tr>");
  client.println("</table>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}
