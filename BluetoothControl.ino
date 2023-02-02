// Library for Bluetooth Related Functionalities
#include "BluetoothSerial.h"

//Intialise the Bluetooth
BluetoothSerial ESP_BT;

// Stored Variable
String incoming_value;
int onoff=0; 
float brightness=0.5;
int colour=127;
int autowc=1;

float g=2.0; //gamma correction

// GPIO for LED (Warm- RX2(PIN 16) , Cool-TX2 (Pin17))
int LED1=16;
int LED2=17;

// Setting PWM Properties
const int freq = 100000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;

// Duty Cycle
const int resolution = 9;

// Gamma Correction 
const uint8_t PROGMEM gamma8[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
  2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
  5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14,
  15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20,
  21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27,
  28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
  49, 50, 50, 51, 52, 54, 55, 56, 57, 58, 59, 60, 
  61, 62, 63, 64, 66, 67, 68, 69, 70, 72, 73, 74,
  75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89, 90,
  92, 93, 95, 96, 98, 99, 101, 102, 104, 107, 109,
  110, 112, 114, 115, 117, 119, 120, 122, 124, 126,
  127, 129, 131, 133, 135, 137, 138, 140, 142, 144,
  146, 148, 150, 152, 154, 156, 158, 160, 162, 164,
  167, 169, 171, 173, 175, 177, 180, 182, 184, 186, 
  189, 191, 193, 196, 198, 200, 203, 205, 208, 210,
  213, 215, 218, 220, 223, 225, 228, 231, 233, 236,
  2239, 241, 244, 247, 249, 252, 255
};

void setup() {
  Serial.begin(9600);                       // Initialise Bluetooth Interface
  ESP_BT.begin("ESP32_LED_Control");        // name of bluetooth 
  Serial.println("Pair your Device");        // Serial Monitor prints this once upload
  ledcSetup(ledChannel1, freq, resolution);  // Configure and set up LED PWM functions for channel 1
  ledcSetup(ledChannel2, freq, resolution);  // Configure and set up LED PWM functions for channel 2
  ledcAttachPin(LED1, ledChannel1);         // LED1 for singal output, Channel 1 will genertate the signal
  ledcAttachPin(LED2, ledChannel2);         // LED2 for singal output, Channel 2 will genertate the signal

}

void loop(){
  if (Serial.available()) {                   // For testing purposes
    String input= Serial.readString();        // Key in and read (0-100)
    colour= input.toFloat() / 100.0 * 255.0;  // Convert to 0-255
    Serial.print("Colour inout:");            // print the conversion
    Serial.println(input);
  }

  if (ESP_BT.available())                     // Check if we receive anything from Bluetooth, adn proceed to next line.
  {
    incoming_value = ESP_BT.readString();     // Read what we received from the phone application
    Serial.println(incoming_value);           // print what we received from phone application

    // ON (LED: 1) & OFF (LED:0)
    if (incoming_value.indexOf("LED:") >=0)
    {
      incoming_value = incoming_value.substring(incoming_value.lastIndexOf(':') + 1);   // Take the string after semicolon (LED)
      onoff= incoming_value.toInt();                                                    // Convert the string into Integer                         
      Serial.print("Received:");                                                        // Print the LED value
      Serial.println(onoff);
    }

    //Brightness (Brightness: x)
    else if (incoming_value.indexOf("Brightness:") >=0)
    {
      incoming_value = incoming_value.substring(incoming_value.indexOf(':') + 1);    // Take the string after semicolon(Brightness)
      brightness = incoming_value.toFloat() / 100.0;                                // Convert the string into float and divide by 100
      brightness= pow(brightness, g);
      Serial.print("Received:");
      Serial.println(brightness);
    }
    
    // Colour Temperature (colour:x) 0-255
    else if (incoming_value.indexOf("Colour Temperature:") >=0)
    {
      incoming_value= incoming_value.substring(incoming_value.indexOf(':') + 1);     // Take the string after the semicolon (Colour temperature)
      colour= incoming_value.toInt();
      Serial.print("Received");
      Serial.println(colour);
    }

    //Auto-warm 3000K / Auto-Cool 5700k
    else if (incoming_value.indexOf("Auto:") >=0 )
    {
      incoming_value = incoming_value.substring(incoming_value.indexOf(':') + 1);     // Take the string after semicolon (Auto-Warm / Auto-Cool)
      autowc = incoming_value.toInt();
      Serial.print("Received:");
      Serial.println(autowc);
    }
  }

// ON
if (onoff ==1 ){
  float cool_val= 0;
  float warm_val= 0;
  // ON > Colour temperature 3000K > Get user brightness > Warm Light On > Cool Light Off
  if (autowc == 3000){
    warm_val = brightness * 255.0;
    cool_val = 0;
  }
  // ON > Colour Temperature 5700K> Get user brightness > Cool Light On > Warm Light Off
  else if (autowc == 5700) {
    warm_val = 0;
    cool_val = brightness *255.0;
  }
  // ON > Customisation > User Input
  else if (autowc ==1) {
    warm_val = byte(brightness * (255.0 - float(colour)));
    cool_val = byte(brightness * float(colour));
  }
  warm_val *= 2.0;
  cool_val *= 2.0;
  Serial.printf("warm val:%f cool val: %f\n", warm_val, cool_val);
  ledcWrite(ledChannel1, int(warm_val));
  ledcWrite(ledChannel2, int(cool_val));
}

//OFF 
if (onoff == 0){
  ledcWrite(ledChannel1, 0);      // Warm Light Off
  ledcWrite(ledChannel2, 0);      // Cool Light Off
}

delay(1);                         // Short Delay
}
