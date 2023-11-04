#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include <PubSubClient.h>
#include "config.h"


#define HEAT_LEVEL  2500
#define HEAT_HYSTERESIS 25

#define ANALOG_FULL_RANGE   4096
#define ROLLING_AVG 15

#define WEBPAGESIZE 512
char webpage[WEBPAGESIZE];

#define MQTTBUFFSIZE  256
char mqttbuff[MQTTBUFFSIZE];

int rainintensity;
int rawresult;
int result = 0;


WiFiClient client;
WebServer server(80);
HTTPUpdateServer httpUpdater;
PubSubClient mqttclient(client);



// -------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0); // prevent slow serial prints if no usb
  
  pinMode(GPIO_CAPACITOR, INPUT);
  pinMode(GPIO_1MOHM, INPUT);
  pinMode(GPIO_HEATER, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  Serial.print("Connecting to wifi ");
  Serial.print(String(ssid));
  WiFi.setHostname(hostname);
  WiFi.begin(ssid, wifipassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis()>60000) { // NO CONNECTION AFTER 60 SECONDS, REBOOT AND RETRY
      ESP.restart();
    }
  }
  Serial.print(" done!\nSignal strength: ");
  Serial.println(String(WiFi.RSSI()));
  Serial.print("Webserver started at http://");
  Serial.println(WiFi.localIP());

  server.on("/", handlewebpage);
  server.begin();
  httpUpdater.setup(&server, ota_path, ota_username, ota_password);
  mqttclient.setServer(mqttserver, 1883);
  mqttConnect(); 
}



// -------------------------------------------------------------------
void loop() {

  unsigned long now = millis();

  server.handleClient();
  mqttclient.loop();
  
  static unsigned long timerone;
  if (now - timerone >= 50) {       // every 50ms
    timerone = now;
    heatercontrol();
  }

  static unsigned long timertwo;
  if (now - timertwo >= 1000) {     // every 1s
    timertwo = now;
    measurerain();
  }
  
  static unsigned long timerthree;
  if (now - timerthree >= 60000) {  // every 60s
    timerthree = now;
    mqttsend();
  }
  
}



// -------------------------------------------------------------------
void measurerain (void) {

  unsigned long timerstart;
  static int averaged = 0;
  
  heater_off(); // prevent overheating if stuck in while loop below

  // -- stop discharging the now discharged capacitor by making gpio high impedance --
  pinMode(GPIO_CAPACITOR, INPUT); 

  // -- start charging capacitor and measure time to charge it to 63% --  
  pinMode(GPIO_1MOHM, OUTPUT);
  digitalWrite(GPIO_1MOHM, HIGH);
  timerstart = micros();
  while (analogRead(GPIO_CAPACITOR) < 0.63*ANALOG_FULL_RANGE);
  rawresult = (int) (micros()-timerstart);
  if (rawresult>=0 || rawresult<500000) {                  // sanitize from odd readings
    result = (int)sqrt(rawresult - CAPACITANCE_OFFSET);    // make the data less unlinear and remove null offset
  }
  rainintensity = (((ROLLING_AVG-1)*averaged) + result );  // rolling average

  // -- start discharging capacitor --  
  pinMode(GPIO_CAPACITOR, OUTPUT);
  digitalWrite(GPIO_CAPACITOR, LOW);

  Serial.print(rawresult);
  Serial.print("\t");
  Serial.print(result);
  Serial.print("\t");
  Serial.println(rainintensity);
}



// -------------------------------------------------------------------
void mqttsend (void) {

  if (!mqttclient.connected()) {
    mqttConnect();
  }
  snprintf( mqttbuff, MQTTBUFFSIZE, "%d", rainintensity );
  mqttclient.publish("homeassistant/sensor/rainmeter/state", mqttbuff);
}



// -------------------------------------------------------------------
void mqttConnect(void) {
  
  heater_off(); // prevent overheating if stuck in while loop below

  while (!mqttclient.connected()) {
    Serial.print("Connecting to mqtt server ");
    Serial.print(String(mqttserver));
    if (mqttclient.connect(hostname, mqtt_username, mqtt_password)) {
      digitalWrite(BLUE_LED, HIGH);
      Serial.println(" done!");
      snprintf( mqttbuff, MQTTBUFFSIZE, "{\"~\":\"homeassistant/sensor/%s\",\"name\":\"%s\",\"unique_id\":\"%s\",\"device_class\":\"precipitation_intensity\",\"stat_t\":\"~/state\"}", haTopic, haName, haUniqid); 
      mqttclient.publish("homeassistant/sensor/rainmeter/config", mqttbuff);
      Serial.println(mqttbuff);
    } 
    else {
      digitalWrite(BLUE_LED, LOW);
      Serial.print(" failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(".");
      delay(5000);
    }
  }
}



// -------------------------------------------------------------------
void heatercontrol(void) {    
  int temperature = ANALOG_FULL_RANGE-analogRead(GPIO_NTC);
  if (temperature < HEAT_LEVEL) {
      heater_on();
  }
  if (temperature > HEAT_LEVEL+HEAT_HYSTERESIS) {
      heater_off();
  }
}

void heater_on(void) {    
  digitalWrite(GPIO_HEATER, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
}

void heater_off(void) {    
  digitalWrite(GPIO_HEATER, LOW);
  digitalWrite(YELLOW_LED, LOW);
}




// -------------------------------------------------------------------
void handlewebpage(void){

  snprintf( webpage, WEBPAGESIZE, " \
  <html><head><meta http-equiv=refresh content=1></head><body><pre>\n \
  Wifi signal:    %d dB\n \
  Hostname:       \"%s\" \n \
                         \n \
  Mqtt server:    \"%s\" connected: %d \n \
  HA name:        \"%s\" \n \
  HA uniq_id:     \"%s\" \n \
                         \n \
  Raw result:      %d    \n \
  Result:          %d    \n \
  Averaged:        %d    \n \
  </pre></body></html> \
  ", WiFi.RSSI(), hostname, mqttserver, mqttclient.connected(), haName, haUniqid, rawresult, result, rainintensity ); 
  server.send(200, "text/html", webpage );
}






// --------------------------
