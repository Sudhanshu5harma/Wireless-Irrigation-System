#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

String ssid ="NET";
String password="dontconnect";
int old = 1;

const int VAL_PROBE = 8; // Digital pin 8
const int MOISTURE_LEVEL = 250;


String uri = "192.168.43.179:8080/";
String server = "192.168.43.179";

void connectWIFI(){

  String cmd = "AT+CWJAP=\"NET\",\"dontconnect\"";
  Serial.println(cmd);
  mySerial.println(cmd);
  delay(6000);

  if(mySerial.find("OK")){
    Serial.println("Connected!");

  }
  else{
    Serial.println("Not Connected.");
    mySerial.println("AT+CIPSTAMAC?");
    connectWIFI();
    }
}

void getRequest(String wdata="0"){

  String cmd = "AT+CIPSTART=\"TCP\",\""; //make this command: AT+CPISTART="TCP","192.168.88.35",80
  cmd += server;
  cmd += "\",8080";
  mySerial.println(cmd);

  //wait a little while for 'Linked'
  delay(300);

  //This is our HTTP GET Request change to the page and server you want to load.
  if (old==0)
  {cmd = "GET /1 HTTP/1.0\r\n";
  cmd += "Host: "+server+"\r\n\r\n";
  }
  else{cmd = "GET /0 HTTP/1.0\r\n";
  cmd += "Host: "+server+"\r\n\r\n";

    }
  Serial.println(cmd);
  
  //The ESP8266 needs to know the size of the GET request
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  Serial.println("AT+CIPSEND="+cmd.length());

  //Look for the > prompt from the esp8266
  if(mySerial.find(">"))
  {
    //Send our http GET request
    Serial.println("Sending Data Now");
    mySerial.println(cmd);
  }
  else
  {
    //Something didn't work...
    Serial.println("Closed Connection");
    mySerial.println("AT+CIPCLOSE");
  }
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(VAL_PROBE, INPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("START");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

      connectWIFI();

      getRequest("1");

  }

void loop() { // run over and over

  int moisture = digitalRead(VAL_PROBE);
      //Serial.println(moisture);
  if (moisture!=old){
      old = moisture;
      Serial.println(old);
      getRequest(old+"");
    }

  if (mySerial.available()) {
    Serial.write(mySerial.read());

  }
}
