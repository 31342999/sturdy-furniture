#include <WiFi.h>
uint64_t chipid;
const char* ssid = "SmartNavigator";
const char* password = "87654321";
void setup()
{
  Serial.begin(115200);
 Serial.setDebugOutput(true);
  Serial.println();
}

void loop()
{
  chipid=ESP.getEfuseMac();

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP address is 'http://");
  Serial.print(WiFi.localIP());

  Serial.printf("and ESP32 Chip ID is = %04X", (uint16_t)(chipid>>32));
  Serial.printf("%08X\n", (uint32_t)chipid);

  delay(3000);
}