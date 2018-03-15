#include <FastLED.h>

int redPin=9;
int greenPin=10;
int bluePin=11;
int count=0;S
double s=2;
int col[3];

char msg[100];

#define DATA_PIN 13
#define NUM_LEDS 19

CRGB leds[NUM_LEDS];

CRGB stripCol;

void setup(){
  col[0]=0;
  col[1]=20;
  col[2]=0;
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  Serial.begin(38400);
  stripCol=CRGB::Green;
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds,NUM_LEDS);
}

void readRGBInt(int* inp){
   long start=millis();
   bool found=false;
   int b;
   do{
     b=Serial.read();
     if(b=='R'){
        delay((1000/38400)*10);
       inp[0]=Serial.parseInt();
       inp[1]=Serial.parseInt();
       inp[2]=Serial.parseInt();
       found=true;
       
       //break;
     }
   }while((millis()-start<30)&&(!found));
   //col=CRGB(inp[0],inp[1],inp[2]);
   //sprintf(msg,"%d %d %d\n",inp[0],inp[1],inp[2]);
   //Serial.print(msg);
}

void loop(){
  count++;
  readRGBInt(col);

  analogWrite(redPin,col[0]);
  analogWrite(greenPin,col[1]);
  analogWrite(bluePin,col[2]);

  stripCol.setRGB(col[0],col[1],col[2]);  
  for(int i=0;i<15;i++){
    leds[NUM_LEDS-i-1]=stripCol;
  }

  FastLED.show();
  

  //analogWrite(redPin,abs((int)(s*count+1)%511-255));
  //analogWrite(greenPin,abs((int)(s*count+170)%511-255));
  //analogWrite(bluePin,abs((int)(s*count+340)%511-255));
}

