
#include <ESP8266WiFi.h>
 #include <DHT.h> // DHT11 temperature and humidity sensor Predefined library
 #include <SPI.h>
#include <Wire.h>
#include <SSD1306Wire.h>


SSD1306Wire  display(0x3c, D2, D1);

 int gas_sensor = D4; //Sensor pin 
 
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0    //GPIO-08 D pin of nodemcu

int moisture = D0;
int threshold = 16;      //Nodemcu digital pin water sensor read-GPIO16---D0 of NodeMCU
int Solenoid = 13;       // GPIO13---D7 of NodeMCU--Motor connection

const char* ssid = "Palak";
const char* password = "califormium";


DHT dht(dht_dpin, DHTTYPE); 
WiFiServer server(80);

void setup()

{ 

  Serial.begin(9600); //Baud rate 
 
  pinMode(gas_sensor, INPUT); //Set gas sensor as input 
  dht.begin();
  pinMode(threshold,INPUT_PULLUP); //Pin#13 as output-Activate pullup at pin 13
  pinMode(Solenoid, OUTPUT);       //D7 as output
  digitalWrite(Solenoid, LOW);     //Deactivate Solenoid
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);     //Begin WiFi
  
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
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

 float h =0.0;  //Humidity level
 float t =0.0;  //Temperature in celcius 
 float f =0.0;  //Temperature in fahrenheit
 float reading;
 
  // Match the request
  int value = LOW;
  if (request.indexOf("/Up=ON") != -1)  {
     h = dht.readHumidity();
     h = h/2.0;
     t = dht.readTemperature(); //Read temperature in celcius
     f = (h * 1.8) + 32;        //Temperature converted to Fahrenheit
     reading = analogRead(A0); //Analog pin reading output voltage by water moisture rain sensor
  
    }

  if (request.indexOf("/Solenoid=ON") != -1)  {   //Motor ON
    digitalWrite(Solenoid, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/Solenoid=OFF") != -1)  {  //Motor OFF
    digitalWrite(Solenoid, LOW);
    value = LOW;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center color=blue>Welcome to EcoFiFarms</h1><br><br>");
  client.print("Temperature in Celsius =");
  client.println(t);
  client.println("<br>");
  client.print("Temperature in Fahrenheit =");
  client.println(f);
  client.println("<br>");
  client.print("Humidity =");
  client.println(h);
  client.print(" %");   
  client.println("<br>");  
  client.println();
  client.print("Moisture Level Percentage =");
  client.print(reading/10);
  client.print("%");
  client.print("<br>");
  
  if(digitalRead(threshold)==HIGH){ // Read digital output of soil sensor
  client.println("Threshold Reached = Rain detected / Moisture exceeded / Water detected");
  }
  
  client.println("<br><br>");
  if(value == HIGH) 
    client.println("Motor/Pump Operational");
   else 
    client.print("Motor/Pump at Halt");
  
  client.println("<br><br>");
  client.println("<a href=\"/Up=ON\"\"><button>Update = Temperature  Humidity Moisture Values</button></a><br />"); 
  client.println("<a href=\"/Solenoid=ON\"\"><button>Motor Pump On </button></a>");
  client.println("<a href=\"/Solenoid=OFF\"\"><button>Motor Pump Off </button></a><br />"); 
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  }
