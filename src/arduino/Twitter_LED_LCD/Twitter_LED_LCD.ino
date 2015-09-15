#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(74,125,232,128); // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

boolean startPrinting = false;
int lineCounter = 0;

//Replace the ??? with the Twitter Account you want to use
String name = "@???????";

String rawText ="";
String color;

//Use the onboard GPIO to enable pins A0-A5 for the LCD
LiquidCrystal lcd(14,15, 16, 17, 18, 19);

//Enable LED pins
int redPin = 9;
int greenPin = 3;
int bluePin = 6;

void setup() {
  
  //Setup LEDS
  pinMode(redPin, OUTPUT);   
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
 
  //Setup LCD for 20x4
  lcd.begin(20, 4); 
  lcd.print("Starting up !!");  
  

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    //Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
 
   
  callTwitter();
}

void loop() {
  
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) 
  {
    char c = client.read();
    if (c == '\n')
    {
       lineCounter+=1;
    }
    if(lineCounter>13)
    {   
      startPrinting = true;
    }
    if(startPrinting && lineCounter < 18) 
    {  
      rawText+=c;
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();
    
    
    //Parse and format raw text from Twitter request 
    int locations[2];
    locations[0] = rawText.indexOf('\n',0);
    locations[1] = rawText.indexOf('\n',locations[0]+1);

    String str1 = rawText.substring(1,locations[1]-6);
    str1.setCharAt(str1.length()-2, '!');
 
    if(str1.charAt(0)=='R' && str1.charAt(1)=='T' && str1.charAt(3)=='@') {
      name = str1.substring(3, str1.indexOf(':'));
      color = str1.substring(str1.indexOf(':')+2,str1.indexOf(' ',str1.indexOf(':')+2));
    }
    
    else {
      if(str1.indexOf(' ') == -1)
        color = str1.substring(0, str1.indexOf('<'));
      else
        color = str1.substring(0, str1.indexOf(' '));
      
      String tempColor = color;
      tempColor.toUpperCase();
      
      //call methods to LED and LCD
      displayLCD(name, tempColor);
      changeLED(tempColor);
  } 
    //Twitter limits the number of requests, set delay to 10 seconds 
    delay(10000);
    
    callTwitter();
  }
}
void callTwitter()
{
  
  if (client.connect(server, 80)) 
  {
     rawText="";
     lineCounter = 0;
     startPrinting = false;
     
    // Make a HTTP request: Use your server name/address in the host line
    client.println("GET /getTweets.php HTTP/1.0");
    client.println("Host: yourservername.com");
    client.println("Connection: close");
    client.println();
  } 
  
}

//display name of color to LCD
void displayLCD(String name, String color)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(name.substring(0,20));
  lcd.setCursor(0,2);
  lcd.print("Picked: "+color.substring(0,12));
}

//change pin values to match requested color
void changeLED(String color)
{
  if(color.equals("RED"))
  {
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }

  else if(color.equals("ORANGE"))
  {
    analogWrite(redPin, 248);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
  else if(color.equals("YELLOW"))
  {
    analogWrite(redPin, 222);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
  else if(color.equals("GREEN"))
  {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
  else if(color.equals("BLUE"))
  {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
  }
  else if(color.equals("PURPLE"))
  {
    analogWrite(redPin, 220);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
  }
  else
  {
    //White
    analogWrite(redPin, 190);
    analogWrite(greenPin, 60);
    analogWrite(bluePin, 255);
  }
}
  

