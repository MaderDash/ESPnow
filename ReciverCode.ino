*
  This is still a work in progress, and is not working as intended. If you remove all code related to millis() it works to turn on the LED by tripping a relay when it receives a command from another ESP32 over the peer 2 peer ESP-NOW protocol. I cannot get the millis() command to work to delay the time at which it will turn off. With the millis() command it ignores the first if statement, never turns on. I am trying to have it turn on when the the motion sensor triggers the broadcaster to send a signal to the receiver indicating that there is movement, I need it to stay on and NOT turn off if there is continuous movement, and I need to be able to set a specific length of time for it to remain on if it is triggered by motion, BUT the motion stops (ie someone walks in front of the motion sensor and then stops there).
*/

#include <WiFi.h>
#include <esp_now.h>
//#include <espnow.h>
//#include <ESP8266WiFi.h>
uint8_t rlyPin    = 26;
bool trigger      = false;
long holdOn       = 10000; // this is the delay that the light will stay on for.
long set          = 0;     // This is the millis set for the start of the timer.

typedef struct message {
  int rly;
} message;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) { //Changed the varables because the varables was incorrect.
  message myMessage;
  Serial.println("Message received.");                               //Debugging.
  memcpy(&myMessage, incomingData, sizeof(myMessage));
  Serial.println(myMessage.rly);                                     //Debugging.
  if (myMessage.rly == 1 && trigger == false) {                      // Now works with new varable type.
    Serial.println("the number is one");                             //To show your in the loop.
    trigger = true;                                                  //This turns the light on.
    set = millis();                                                  //Sets millis start point.
  }
  if (trigger == true && millis() - set >= holdOn) {                 //Looks for the millis to run over.
    trigger = false;                                                 //Sets the led off, and readys the next check.
  }
  if (trigger == true) {
    Serial.println("led is on");                                     //Debugging.
    digitalWrite(rlyPin, HIGH);                                      //Pin high.
  }
  if (trigger == false) {
    Serial.println("led is off");                                   //Debugging.
    digitalWrite(rlyPin, LOW);                                      //Pin LOW.
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  pinMode(rlyPin, OUTPUT);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
