#include <FastLED.h>

//pins für große LED
#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11

//pins für knöpfe
#define B1PIN 2
#define B2PIN 3

CRGB col;

//char msg[100];

//#define DATA_PIN 13
//#define NUM_LEDS 19


volatile byte mode=1;//aktueller modus
const byte modeMax=5;//anzahl verschiedener modi


void setup(){

  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);

  pinMode(B1PIN,INPUT);
  
  Serial.begin(38400);
  col=CRGB::Gold;

  attachInterrupt(digitalPinToInterrupt(B1PIN),b1Press,RISING);

  // put your setup code here, to run once:
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds,NUM_LEDS);
}

void b1Press(){
  mode=(mode+1)%modeMax;
}


void readRGBInt(CRGB* inp){
   long start=millis();
   bool found=false;
   int b;
   do{
     b=Serial.read();
     if(b=='R'){
        delay((1000/38400)*10);
       inp->r=Serial.parseInt();
       inp->g=Serial.parseInt();
       inp->b=Serial.parseInt();
       found=true;
       
       //break;
     }
   }while((millis()-start<30)&&(!found));
   //col=CRGB(inp[0],inp[1],inp[2]);
   //sprintf(msg,"%d %d %d\n",inp[0],inp[1],inp[2]);
   //Serial.print(msg);
}

void hsvRotate(CRGB* inp){
  inp->setHue((millis()/500)%255);
}

void hsvPerlin(CRGB* inp){
    const double tc=10000;
    int data=floor((millis()/tc)*255);
    int per=inoise8(data);
    //int per=0;
    per+=(int)((millis()/tc)*255*0.4)%255;
    while(per>255)per-=255;
    while(per<0)per+=255;
    
    inp->setHue(per);  
}

void setLight(CRGB* inp){
  analogWrite(REDPIN,inp->r);
  analogWrite(GREENPIN,inp->g);
  analogWrite(BLUEPIN,inp->b);
}

void loop(){
  switch(mode){
    case 0:
      readRGBInt(&col);
      break;
    case 1:
      hsvRotate(&col);
      break;
    case 2:
      hsvPerlin(&col);
      break;
    case 3:
      col.setRGB(0,255,0);
      break;
    case 4:
      col.setRGB(0,0,255);
      break;
  }
  setLight(&col);
}

