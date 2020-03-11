#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <SoftwareSerial.h>
#include <avr/wdt.h>

/**
 * Define the constants for the wifi adapter
 */
SoftwareSerial wifiSerial(10,11);      // RX, TX for ESP8266
int responseTime = 20; //communication timeout
bool DEBUG = false;   //show more logs

/**
 * Define the constants for the RGB board
 */
// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
//double buffer mode turned off because of memory consumption
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
int16_t    textX         = matrix.width(),
           textMin       = 100;
           
bool shouldScroll = false;
bool hasWritten = false;
String text = "";
String cycle = "0";
    int red = 0;
    int green = 0;
    int blue = 0;
    int hue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("start\n");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Port connect\n");
  wifiSerial.begin(9600);
  while (!wifiSerial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("wifiSerial connect\n");
  sendToWifi("AT+CIFSR",responseTime,DEBUG); // get ip address
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); // turn on server on port 80
  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); // configure as access point
  sendToWifi("AT+CWSAP?",responseTime,DEBUG); // get credentials
  delay(100);
  sendToUno("Wifi connection is running!",responseTime,DEBUG);
  Serial.print("setup done");
}

void loop() {

  if(wifiSerial.available()>0){
    String message = readWifiSerialMessage();
   
    int startPos = findIndex(message,"~");
    if (startPos != -1) {
      int endPos = findIndex(message,"#");
      
      String parms = message.substring(startPos+1,endPos);
      char buf[100];
      parms.toCharArray(buf, sizeof(buf));
      char *ptr = NULL;
      int count = 0;
      ptr = strtok(buf, ",");  // takes a list of delimiters

      while(ptr != NULL) {     //runs a string tokenizer to get all parameters
                               //currently the message, should it color shift, red, green and blue
      
        if (count == 0) {
            text = ptr;
        } else if (count == 1) {
            cycle = ptr;
        } else if (count == 2) {
            red = atol(ptr);
        } else if (count == 3) {
            green = atol(ptr);
        } else if (count == 4) {
            blue = atol(ptr);
        }
        count++;
        ptr = strtok(NULL, ",");  // takes a list of delimiters
      }
      
      hasWritten = false;
      setMatrixParams();
      matrix.setTextColor(matrix.Color333(red, green, blue));
      textX = matrix.width();
      shouldScroll = true;
      Serial.println(text);
      char str[text.length()];
      //char str[] PROGMEM = parms.toCharArray(str,parms.length());
      text.toCharArray(str,text.length());
      textMin = sizeof(str) * -12;
    }
  }
   
    if (shouldScroll) {
      matrix.fillScreen(0); //blanks the screen
      matrix.setCursor(textX, 1);
      matrix.print(text);
      if((--textX) <= textMin) {
        hasWritten = true;
        shouldScroll = false;
      } 
      matrix.swapBuffers(false);
    } else if (hasWritten == true) {
      delay(25);
      hasWritten = false;
      wdt_enable(WDTO_15MS);
    }
    
  //}
  delay(20);
}

void setMatrixParams() {
   matrix.begin();
   matrix.setTextWrap(false); // Allow text to run off right edge
   matrix.setTextSize(1);
   matrix.fillScreen(0); //blanks the screen
}

// this function will return the number of bytes currently free in RAM     
extern int  __bss_end;
extern int  *__brkval;
int freemem()
{
int free_memory;
if((int)__brkval == 0)
   free_memory = ((int)&free_memory) - ((int)&__bss_end);
else
   free_memory = ((int)&free_memory) - ((int)__brkval);
return free_memory;
} 

/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: find
* Description: Function used to match two string
* Params: 
* Returns: true if match else false
*/
boolean find(String string, String value){
  if(string.indexOf(value)>=0)
    return true;
  return false;
}

/*
* Name: find
* Description: Function used to match two string
* Params: 
* Returns: true if match else false
*/
int findIndex(String string, String value){
  if(string.indexOf(value)>=0)
    return string.indexOf(value);
  return -1;
}

/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    Serial.print(value[index_count]);
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
    delay(5);
  }
  String str(value);
  str.trim();
  return str;
}

String sendToUno(String command, const int timeout, boolean debug){
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}
