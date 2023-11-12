const char* ssid = "xxxxxxxx";
const char* wifipassword = "xxxxxxxx";
const char* hostname = "rainmeter";

const char* mqttserver = "xxxxxxxx";
const char* mqtt_username = "xxxxxxx";
const char* mqtt_password = "xxxxxxxx";

const char* ota_path = "/firmware";
const char* ota_username = "xxxxxxx";
const char* ota_password = "xxxxxx";

const char* haName   = "Rainmeter";
const char* haUniqid = "rainmeter01";

#define CAPACITANCE_OFFSET  200     // Set so the reported rainintensity stays at zero when the sensor is dry
                                    // Go to the device's local web page to get help finding the correct value
#define BLUE_LED        10
#define YELLOW_LED      11

#define GPIO_NTC        1
#define GPIO_HEATER     48
#define GPIO_CAPACITOR  2
#define GPIO_1MOHM      41
