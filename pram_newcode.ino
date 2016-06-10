//need to calibrate Setpoint, pGain , dGain
//almost achieved levitation, object starts sliding when kept on a piece
//of cardboard but falls down as soon as it is removed

int coilPin=5;
int trig=2;
int echo=3;
int power;
float currentPos;

const int controlBias=220;
float Setpoint=19.5;

const float pGain=20;
const float dGain=50;

float sensorRead;
float prev_sensorRead;

unsigned long millisCounter;

void setup() {
  pinMode(coilPin, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  digitalWrite(echo,LOW);
  
}

float ultrasonic()
{
    digitalWrite(trig,LOW);
    digitalWrite(echo,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10); 
    digitalWrite(trig,LOW);

    long double duration = pulseIn(echo,HIGH);
    float distance = duration/2/29.1; 
    return distance ;
     
    
}

void checkSerial()
{
  if (Serial.available()){
    char ch=Serial.read();
    if(ch=='s'){
      Setpoint=Setpoint-0.10;
    }
    if(ch='S'){
      Setpoint=Setpoint+0.10;
    }
  }
}
void loop() {
  //this is control mode
  float err;
  float der;
  

   if( (signed long)( millis() - millisCounter ) >= 0)
   {
      millisCounter = millis() + 1;
    
      prev_sensorRead = sensorRead;
      sensorRead =ultrasonic();
      if(sensorRead>22.0){
        power=0;
      }
      else{
      err=Setpoint-sensorRead;
      der=prev_sensorRead-sensorRead;

      power=controlBias+ int(pGain*err) + int(dGain*der);
      }
      Serial.print(ultrasonic());
      Serial.print("      ");
      Serial.println(power);
  
      analogWrite(coilPin,power);
      checkSerial();
      delay(100);
   }
  
  
}
