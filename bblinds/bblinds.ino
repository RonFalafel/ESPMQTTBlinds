#include <Stepper.h>
#include <EspMQTTClient.h>

EspMQTTClient client(
  "Froot",
  "66666666",
  "192.168.1.150",                                                     // MQTT Broker server ip
  "homeassistant",                                                     // Can be omitted if not needed
  "chiquiingoF9ohsoefoh8ohXoola2quuw2ichahdeiwae7theise7oosheesh7ah",  // Can be omitted if not needed
  "BlindsControl",                                                         // Client name that uniquely identify your device
  1883                                                                 // The MQTT port, default to 1883. this line can be omitted
);

const int stepsPerRevolution = 2048;  // The number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 4
#define IN2 16
#define IN3 17
#define IN4 5

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  // set the speed at 5 rpm
  myStepper.setSpeed(5);
  // initialize the serial port
  Serial.begin(115200);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages();                                           // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater();                                              // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA();                                                         // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true

}

void loop() {
  client.loop();

  
}

// This function is called once everything is connected (Wifi and MQTT)
void onConnectionEstablished() {
  client.subscribe("blinds/open", openBlinds);
  client.subscribe("blinds/close", closeBlinds);
}

void openBlinds(const String& topic, const String& message) {
  // step two revolutions in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution*2);
}

void closeBlinds(const String& topic, const String& message) {
  // step two revolutions in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution*2);
}