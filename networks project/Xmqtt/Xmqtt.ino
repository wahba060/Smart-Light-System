#include <WiFi.h>
#include <PubSubClient.h>

// Define PIR sensor pin
int pirPinA = 12; 
int pirPinB = 27;
int pirPinC = 25;


// Define LED pin
int ledPin1 = 13;  //A asfr
int ledPin2 = 14;  //B a5dar
int ledPin3 = 26;  //C a7mar

const char *ssid = "Swaify";
const char *password = "swaify123";
const char *mqttServer = "4.tcp.eu.ngrok.io"; // Ngrok hostname without "tcp://"
const int mqttPort = 14963;           // Ngrok port
const char *mqttTopic1 = "room1/light";
const char *mqttTopic2 = "room2/light";
const char *mqttTopic3 = "room3/light";

WiFiClient espClient;
PubSubClient client(espClient);



void setup() {

  Serial.begin(9600);
  // Set PIR sensor pin as input
  pinMode(pirPinA, INPUT);
  pinMode(pirPinB, INPUT);
  pinMode(pirPinC, INPUT);

  // Set LED pin as output
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
  digitalWrite(ledPin3,LOW);

  // Initialize serial communication for debugging (optional)
  

  WiFi.begin(ssid, password);

      // connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");


    // Here we're gonna connect to the MQTT server
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT");
      client.subscribe(mqttTopic1);
      client.subscribe(mqttTopic2);
      client.subscribe(mqttTopic3);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message received on topic: " + String(topic));

  // Handle messages based on the topic

  String message1 = "";
  String message2 = "";
  String message3 = "";


  if (strcmp(topic, "room1/light") == 0) {

       
    for (int i = 0; i < length; i++) {
      message1 += (char)payload[i];
    }
    
    TurnLights(ledPin1, message1);

  } else if (strcmp(topic, "room2/light") == 0) {

       
    for (int i = 0; i < length; i++) {
      message2 += (char)payload[i];
    }

    TurnLights(ledPin2, message2);
    
  } else if (strcmp(topic, "room3/light") == 0) {

      
    for (int i = 0; i < length; i++) {
      message3 += (char)payload[i];
    }


    TurnLights(ledPin3, message3);
    
  }
  if (message1.length() > 0) {
    Serial.println(message1);
  } else {
    Serial.println("Packet loss");
  }
  if (message2.length() > 0) {
    Serial.println(message2);
  } else {
    Serial.println("Packet loss");
  }
  if (message3.length() > 0) {
    Serial.println(message3);
  } else {
    Serial.println("Packet loss");
  }


  
  
  
}





void loop() {
  
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

 

  delay(3000);

  // Print the PIR sensor value to the serial monitor (optional)


// Read the PIR sensor value
  int pirValueA = digitalRead(pirPinA);
  int pirValueB = digitalRead(pirPinB);
  int pirValueC = digitalRead(pirPinC);




  Serial.println("A =");
  Serial.println(pirValueA);
  Serial.println("B =");
  Serial.println(pirValueB);
  Serial.println("C =");
  Serial.println(pirValueC);
  
  // Print the PIR sensor value to the serial monitor (optional)
  

  // Check if motion is detected

  LightFunc(pirValueA, ledPin1, mqttTopic1);
  LightFunc(pirValueB, ledPin2, mqttTopic2);
  LightFunc(pirValueC, ledPin3, mqttTopic3);

 
 
}
void LightFunc(int pirValue, int ledPin, const char* mqtt)
{

 

  if (pirValue == 1) {
    // Turn on the LED
    digitalWrite(ledPin, HIGH);
    delay(3000);
   client.publish(mqtt, "ON");
    digitalWrite(ledPin, LOW);
  } 
  else {
    // Turn off the LED
    digitalWrite(ledPin, LOW);
   client.publish(mqtt, "OFF");
  }
}

void TurnLights(int led, String message)
{

 if (message.equals("ON"))
 {
    digitalWrite(led,HIGH);
    delay(500);
 }
  else if (message.equals("OFF")){
    digitalWrite(led,LOW);
  }
}


