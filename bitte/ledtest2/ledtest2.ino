#include <FastLED.h>


#define DATA_PIN 13
#define NUM_LEDS 19

CRGB leds[NUM_LEDS];

CRGB col;
int nums[3];
void readRGB(CRGB* c){
   long start=millis();
   int num=0;
   int b;
   do{
     b=Serial.read();
     if(b=='R'){
        delay((1000/38400)*10);
       int r=(byte)Serial.parseInt();
       int g=(byte)Serial.parseInt();
       int b=(byte)Serial.parseInt();
       col .setRGB(r,g,b);
       
       //break;
     }
   }while((millis()-start<30)&&(num<3));
   //col=CRGB(inp[0],inp[1],inp[2]);
   //sprintf(msg,"%d %d %d\n",inp[0],inp[1],inp[2]);
   //Serial.print(msg);
}

void setup() {
  col=CRGB::Orange;
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds,NUM_LEDS);
  Serial.begin(38400);
}

int count=0;
void loop() 
{
  //count++;
  //leds[count%NUM_LEDS]=col;
  for(int i=0;i<1;i++){
    leds[NUM_LEDS-i-1]=col;
  }
  //for(int i=1;i<NUM_LEDS;i++){
    //leds[i]=leds[i-1];  
  //}
  //leds[0]=CRGB::White;
  FastLED.show();
  readRGB(&col);
  // put your main code here, to run repeatedly:
  
}
