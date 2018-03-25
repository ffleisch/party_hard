
int redPin=9;
int greenPin=10;
int bluePin=11;

int bttn1Pin=2;
int bttn2Pin=3;

int enc1P1=


void setup() {
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);

  pinMode(bttn1Pin,INPUT);
  Serial.begin(38400);
}

void loop() {
  digitalWrite(redPin,digitalRead(bttn1Pin)?HIGH:LOW);
  // put your main code here, to run repeatedly:
}
