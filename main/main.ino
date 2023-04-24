#include "network-config.h"
#include "network-ip-address.h"
#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebSrv.h"
#include "FastLED.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define NUM_LEDS 74
#define led_pin D7
#define LED_TYPE WS2811
CRGBArray<NUM_LEDS> leds;
 
const char *ssid = wifi_ssa;
const char *password = wifi_key;
String theInput = "0";
const char *PARAM_INPUT = "value";

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
  <html>
    <head>
    <meta charset="utf-8"/>
      <style>
        h1  {color: Grey; font-family: tahoma; font-size: 350%; text-align: center; margin-top: 100px;}
        .center{display:grid; justify-content:center;}
        [type="color"] {opacity: 40;width: 300px;height: 200px; border-radius:5px;}
        .hex {font-family: tahoma; font-weight: bold; font-style:italic; text-align:center; justify-content: center; margin:25px;}
        .buttoncontainer{display:grid; justify-content:center; grid-template-columns: 150px 150px; grid-gap:15px;}
        button{outline:none; border:none; font-family: tahoma; font-weight:bold; font-size:20px; opacity:0.8; border-radius:10px; width:auto; padding:20px;}        
        button:active{opacity:1.0;}
        #red{background-color:Salmon;}
        #green{background-color:#2bba18;}
      </style>
      <title>LED Sign Controller</title>
      <meta name="viewport" content="width=device-width", intital-scale=1>
    </head>
    <body>
      <h1>Select Color</h1>
          <div class="center"><input type="color"  id="colorPicker" value="#cc0000" class="picker" >
            <label class="hex" id="hex">Signature Red</label>
          </div>
          <div class="buttoncontainer">
            <button type="submit" id="green">Grün</button>
            <button type="submit" id="red">Lachsrot</button>
          </div>
      
      <script>      
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/color?value=r204g0b0", true);
        xhr.send();

        colorPicker.addEventListener("input", updateHex, false);
        
          function updateHex(event){
          document.getElementById("hex").innerHTML = document.getElementById("colorPicker").value;
          var xhr = new XMLHttpRequest();
          var hexcode = document.getElementById("colorPicker").value;
          var rgbcode = hexToRGB(hexcode.slice(1));
          console.log(rgbcode);
          var url = "/color?value=" + rgbcode;
          xhr.open("GET", url, true);
          xhr.send();
          }
          
          function hexToRGB(hex){
          var aRgbHex = hex.match(/.{1,2}/g);
          var aRgb = [parseInt(aRgbHex[0], 16),
                  parseInt(aRgbHex[1], 16),
                  parseInt(aRgbHex[2], 16)];
          var rgbcode = "r"+aRgb[0]+"g"+aRgb[1]+"b"+aRgb[2];
          return rgbcode;
          }
          
        document.getElementById("red").addEventListener("click", red, false);       
          function red(event){
            console.log("The red button has been clicked!");
            document.getElementById("hex").innerHTML = "Lachsrot Teslas";
            document.getElementById("colorPicker").value = "#fa8072";
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/color?value=r213g109b86", true);
            xhr.send();
          }
          
        document.getElementById("green").addEventListener("click", green, false);
          function green(event){
            console.log("The green button has been clicked!");
            document.getElementById("hex").innerHTML = "Grüne Mitsubishis";
            document.getElementById("colorPicker").value = "#2bba18";
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/color?value=r39g186b39", true);
            xhr.send();
          }
      </script>
    </body>
  </html>
)=====";

IPAddress local_ip(ip_address_1,ip_address_2,ip_address_3,ip_address_led_stripe);
IPAddress gateway(ip_address_1,ip_address_2,ip_address_3,ip_address_gateway);
IPAddress subnet(255,255,255,0);
 
AsyncWebServer server(80);

void setup(){
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  delay(100);
  WiFi.begin(ssid, password);

  if (!WiFi.config(local_ip, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to Wifi..."); 
  }

  //NEOPIXEL
  FastLED.addLeds<LED_TYPE,led_pin>(leds, NUM_LEDS);

  //WiFi.softAP(ssid, password);
  //WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(local_ip, gateway, subnet);

  Serial.print("IP addresse: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  
  server.on("/color", HTTP_GET, [] (AsyncWebServerRequest * request){
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      convertHextoRGB(inputMessage);
      Serial.println(inputMessage);
      delay(5000);
      convertHextoRGB(inputMessage);
    }
    else{
      Serial.println("No new color set");
    }
    request->send(200, "text/plain", "OK");
  });
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });
 
  server.begin();
}
 
void loop(){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = 0xFF44DD;
   }
  FastLED.show();
  digitalWrite(LED_BUILTIN, HIGH);
}

void convertHextoRGB(String rgbcode){
  int pos1 = rgbcode.indexOf('r');
  int pos2 = rgbcode.indexOf('g');
  int pos3 = rgbcode.indexOf('b');

  int red = (rgbcode.substring(pos1+1,pos2)).toInt();
  int green = (rgbcode.substring(pos2+1,pos3)).toInt();
  int blue = (rgbcode.substring(pos3+1)).toInt();

  for(int i=0; i<NUM_LEDS; i++){
    leds[i].r = red;
    leds[i].g = green;
    leds[i].b = blue;
    FastLED.show();
  }
  Serial.print("Red:");
  Serial.println(red);
  Serial.print("Green:");
  Serial.println(green);
  Serial.print("Blue:");
  Serial.println(blue);
  }
