#include <Event.h>
#include <Timer.h>

#include <DHT.h>

#include <ESP8266WiFi.h>
#define G_LED 2
#define Y_LED 16  
#define R_LED 5
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); // dht설정 

Timer t;
Timer led;

String apiKey = "WCP1KYKV4LMW4TB7";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "AndroidHotspot7506";     // replace with your wifi ssid and wpa2 key
const char *pass =  "0104879s";
const char* server = "api.thingspeak.com";
int inputVal = 0;

WiFiClient client;

void setup() {
  pinMode(G_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  Serial.begin(115200);
  delay(20);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  dht.begin();
  WiFi.begin(ssid, pass);

  Serial.print("test");
  delay(100);
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");  
    
}

void loop() {
    int h = dht.readHumidity();
    int t = dht.readTemperature(); 

    if(h < 20 || t <10 || t>30 || h>70)
    {
      digitalWrite(G_LED, LOW);  
      digitalWrite(Y_LED, LOW);
      digitalWrite(R_LED, HIGH);
    }
    else if(h >= 20 && h < 40 || t>25)
    {
      digitalWrite(G_LED, LOW);  
      digitalWrite(Y_LED, HIGH);
      digitalWrite(R_LED, LOW);
    } 
    else
    {
      digitalWrite(G_LED, HIGH);  
      digitalWrite(Y_LED, LOW);
      digitalWrite(R_LED, LOW);
    }
    
    if(h < 100 && t < 100)
    {
      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
     {  
        String postStr = apiKey;
        postStr +="&field1=";
        postStr += String(t);
        postStr +="&field2=";
        postStr += String(h);
        postStr += "\r\n\r\n";
 
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);

       Serial.print("습도 : ");
       Serial.println(h);
       Serial.print("온도 : ");
       Serial.print(t);
       Serial.println(" C ");
     }
  client.stop();
 
  Serial.println("Waiting...");

  delay(15000);    
    }
}
