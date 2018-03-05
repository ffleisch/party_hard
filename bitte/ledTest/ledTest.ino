int redPin=9;
int greenPin=10;
int bluePin=11;
int count=0;
double s=2;
int col[3];

char msg[100];

void setup(){
  col[0]=0;
  col[1]=0;
  col[2]=0;
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  Serial.begin(38400);
}



void readRGB(int* inp){
   long start=millis();
   int num=0;
   int b;
   do{
     b=Serial.read();
     if(b=='R'){
       inp[0]=Serial.parseInt();
       inp[1]=Serial.parseInt();
       inp[2]=Serial.parseInt();
     }
   }while((millis()-start<30)&&(num<3));
   //sprintf(msg,"%d %d %d\n",inp[0],inp[1],inp[2]);
   //Serial.print(msg);
}

void loop(){
  count++;
  readRGB(col);
  analogWrite(redPin,col[0]);
  analogWrite(greenPin,col[1]);
  analogWrite(bluePin,col[2]);
  //analogWrite(redPin,abs((int)(s*count+1)%511-255));
  //analogWrite(greenPin,abs((int)(s*count+170)%511-255));
  //analogWrite(bluePin,abs((int)(s*count+340)%511-255));
}

