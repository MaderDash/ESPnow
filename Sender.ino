/*
  Here is the working broadcaster, ESP32, that corresponds to the receiver that I already posted, and am having issues with. The motion sensor is a RCWL-0516 radar motion sensor without CDS.
*/

#include <WiFi.h>
#include <esp_now.h>
// ESP8266 Mac address (first peer)
uint8_t mac_peer1[] = {0x3C, 0x71, 0xBF, 0x29, 0x6E, 0x3D};

esp_now_peer_info_t peer1;
typedef struct message {
  int rly;
};
struct message myMessage;

int motsensPin = 26;
int motion;
bool toggle = true;

void setup() {
  Serial.begin(115200);
  pinMode(motsensPin, INPUT);
  WiFi.mode(WIFI_STA);
  // Get Mac Add
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println(" ESP32 ESP-Now Broadcast");
  // Initializing the ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }
  memcpy(peer1.peer_addr, mac_peer1, 6);
  peer1.channel = 1;
  peer1.encrypt = 0;
  // Register the peer
  Serial.println("Registering a peer 1");
  if ( esp_now_add_peer(&peer1) == ESP_OK) {
    Serial.println("Peer 1 added");
  }
  esp_now_register_send_cb;
}
void loop() {
  motion = toggle;
  //motion = digitalRead(motsensPin);
  Serial.print("Motion Sensor: ");
  Serial.print(motion);
  Serial.print(" rly: ");
  Serial.println(myMessage.rly);
  //  if (motion == 1) {
  //    if (myMessage.rly == 1) {
  //      myMessage.rly = 0;
  //
  //    }
  //  }
  //  else {
  //    myMessage.rly = 1;
  //  }
  esp_err_t result = esp_now_send(mac_peer1, (uint8_t *) &myMessage, sizeof(myMessage));

  if (result == 0) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  myMessage.rly = 0;
  toggle = !toggle;
  delay(1000);
  Serial.println(myMessage.rly);
}
