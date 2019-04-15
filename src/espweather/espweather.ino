#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* OLED DETAILS */
#define PIN_RST D2
#define PIN_SDA D4
#define PIN_SCL D5

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, PIN_RST, PIN_SCL, PIN_SDA);

/* WIFI DETAILS */
const char* ssid      = "SSID";
const char* password  = "PASSWORD";
const char* host_name = "HOST_NAME";
const char* api_url   = "API_PHP_URL";

/* VARIABLES */
bool booted       = false;
int second_count  = 0;
int page          = 0;

/* WEATHER VARIABLES */
int temp;
int temp_min;
int temp_max;
byte humidity;
int pressure;
String conditions;
String weekday;
byte day;
byte month;
int year;
byte hours;
byte minutes;
byte seconds;
 
void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.hostname(host_name);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.printf("Connecting to %s\n", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Succesfully connected!");
  Serial.print("Host Name: ");
  Serial.println(WiFi.hostname());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  u8g2.begin();
}

void loop() {
  String string;
  
  if (!booted && second_count >= 4) {
    booted = true;
    page = 1;
  }

  u8g2.firstPage();
  do {
    if (page == 0) {

      u8g2.setFont(u8g2_font_open_iconic_embedded_1x_t);
      u8g2.drawGlyph(60, 12, 0x0050);

      u8g2.setFont(u8g2_font_mozart_nbp_tf);
      u8g2.drawStr(26, 30, WiFi.localIP().toString().c_str());

    }
    if (page == 1) {
      if (second_count >= 5) {
        second_count = 0;
        
        if (WiFi.status() == WL_CONNECTED) {
          
          HTTPClient http;
          http.begin(api_url);
          int httpCode = http.GET();

          if (httpCode > 0) {

            const size_t bufferSize = JSON_OBJECT_SIZE(14);
            DynamicJsonBuffer jsonBuffer(bufferSize);
            JsonObject& root = jsonBuffer.parseObject(http.getString());

            temp = root["temp"];
            temp_min = root["temp_min"];
            temp_max = root["temp_max"];
            humidity = root["humidity"];
            pressure = root["pressure"];
            const char *a1 = root["conditions"];
            conditions = String(a1);
            
            const char *a2 = root["weekday"];
            weekday = String(a2);
            day = root["day"];
            month = root["month"];
            year = root["year"];
            hours = root["hours"];
            minutes = root["minutes"];
            seconds = root["seconds"];

          }
          http.end();
        }
      }

      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      if (conditions.equals("Clear"))
        u8g2.drawGlyph(0, 32, 0x0045);
      if (conditions.equals("Clouds"))
        u8g2.drawGlyph(0, 32, 0x0041);
      if (conditions.equals("Drizzle"))
        u8g2.drawGlyph(0, 32, 0x0040);
      if (conditions.equals("Thunderstorm") || conditions.equals("Rain") || conditions.equals("Snow"))
        u8g2.drawGlyph(0, 32, 0x0043);

      u8g2.setFont(u8g2_font_mozart_nbp_tf);
      string = weekday + " " + day + "/" + month + " H:" + humidity + "%";
      u8g2.drawStr(40, 12, string.c_str());

      u8g2.setFont(u8g2_font_crox3h_tf);
      string = String(temp) + "°C";
      u8g2.drawUTF8(40, 32, string.c_str());
      
      u8g2.setFont(u8g2_font_crox1h_tf);
      string = String(temp_min) + "° / " + temp_max + "°";
      u8g2.drawUTF8(84, 32, string.c_str());
    }
  } while (u8g2.nextPage());

  second_count++;
  delay(1000);
}