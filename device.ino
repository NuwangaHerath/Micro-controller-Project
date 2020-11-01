#include <AdafruitIO_WiFi.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <dht.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <SFE_BMP180.h>
#include <Average.h>

//Initialize_DHT------------------------------------------------------------
dht DHT;
#define DHT11_PIN 16

//Initialize_BMP------------------------------------------------------------
SFE_BMP180 bmp;

//Initialize Wifi connection--------------------------------------------------
#define ssid "nuwanga"
#define password "nuwa#12"

//Initialize total and mean temperature, humidity, intensity, presure and count---------

double tot_temp=0.0;
double tot_hum=0.0;
double tot_intensity;
double tot_pres=0.0;
double mean_temp=0.0;
double mean_hum=0.0;
double mean_intensity=0.0;
double mean_pres=0.0;
double sd_temp=0.0;
double sd_hum=0.0;
double sd_intensity=0.0;
double sd_pres=0.0;
double mean_data[4];
double SD_data[4];
Average<double> temp_array(15);
Average<double> hum_array(15);
Average<double> intensity_array(15);
Average<double> pres_array(15);
int count=0;

bool isSend = true;




void setup(){
  Serial.begin(9600);
  //pin_config_of_LDR--------------------------------------------------------
  pinMode(A0,INPUT);
  //Initialize the BMP sensor------------------------------------------------
  if (bmp.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  //Initialize wifi connectivity----------------------------------------------
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP
}

void loop(){

  //DHT11_Start-------------------------------------------------------------------
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  tot_temp=tot_temp+DHT.temperature;
  temp_array.push();
  


  
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  tot_hum=tot_hum+DHT.humidity;
  hum_array[count]=DHT.humidity;

  
  //LDR_Starts------------------------------------------------------------------
  int intensity=analogRead(A0);      
  Serial.println("Intensity="); 
  Serial.println(intensity);
  tot_intensity=tot_intensity+intensity;
  intensity_array[count]=intensity;
  
  //BMP_starts------------------------------------------------------------------
  char status;
  double T,P,p0,a;
 
  status = bmp.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
 
    // Retrieve the completed temperature measurement:
    status = bmp.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      status = bmp.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);
        
        // Retrieve the completed pressure measurement:
        // Function returns 1 if successful, 0 if failure.
        status = bmp.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          tot_pres=tot_pres+P*100;
          pres_array[count]=P*100;

        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  count=count+1;
 //As measurements takes in every minute, 15 measurements have to take in 15 min time.
  if (count=15){

  //calculation of mean values of the measurements
    String postData,s_temp,s_hum,s_intensity,s_pres;
    count=0;
    mean_temp=tot_temp/15;
    s_temp=String(mean_temp);
    mean_data[0]=mean_temp;
    mean_hum=tot_hum/15;
    s_hum=String(mean_hum);
    mean_data[1]=mean_hum;
    mean_intensity=tot_intensity/15;
    s_intensity=String(mean_intensity);
    mean_data[2]=mean_intensity;
    mean_pres=tot_pres/15;
    s_pres=String(mean_pres);
    mean_data[3]=mean_pres;
    sd_temp=String(temp_array.stddev());
    sd_hum=String(hum_array.stddev());
    sd_pres=String(pres_array.stddev());
    sd_intensity=String(intensity_array.stddev());

    postData = "mtemp=" + s_temp + "&mhum=" + s_hum + "&mintensity=" + s_intensity + "&mpres=" + s_pres + "sdtemp=" + sd_temp + "&sdhum=" + sd_hum + "&sdintensity=" + sd_intensity + "&sdpres=" + sd_pres ;
  
    http.begin("http://192.168.43.128/microweb/server.php");              //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
    int httpCode = http.POST(postData);   //Send the request
    String payload = http.getString();    //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
  
    if(httpCode!=200){
      isSend=false;
      }
    else if()httpCode==200{
      isSend=true;
      }
 
    http.end();  

    mean_temp=0.0;
    mean_hum=0.0;
    mean_intensity=0.0;
    mean_pres=0.0;
  }
//if some connection error occures stored data are re-try to send to the server in every minute
  if(!isSend){
  
  http.begin("http://192.168.43.128/microweb/server.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  if(httpCode!=200){
      isSend=false;
      }
  else if()httpCode==200{
      isSend=true;
      }
 
    http.end();
  }  
 delay(1000);
  
}
