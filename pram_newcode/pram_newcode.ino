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


/*
Tuning: The process of determining appropriate values for the gain
coefficients kP, kI, and kD refers to “tuning” the system. A simple empirical approach is to start
by zeroing the integral and derivative gains, and just use the proportional term. Setting the
proportional gain increasingly higher will finally cause the system to oscillate. This is bad
behavior; don’t allow it to continue. Reduce the proportional gain until you are just below the
point of incipient oscillation. You can then try bringing up the derivative gain, which should act
to forestall the start of oscillatory behavior. And finally adding a small amount of integral gain
may help bring the system to the final set point.
The best coefficients for a given control system depend on the goals of the system. Bringing a
subway train smoothly up to speed with no oscillations or overshoot is one goal; rapidly
achieving a set point where some overshoot and oscillations are an acceptable tradeoff for fast
response is a different goal. Different control goals require different tunings. 
*/

const float pGain=1;
const float dGain=0;
const float iGain=0;

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
    return distance;
     
    
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
  float ier;
  float IntThresh ;             //set it according to the observed values
  

   if( (signed long)( millis() - millisCounter ) >= 0) //implement oscillator with pid                    //millis wala nahi chamka
   {
      millisCounter = millis() + 1;
    
      prev_sensorRead = sensorRead;
      sensorRead =ultrasonic();
      if(sensorRead>22.0){
        power=0;
      }
      else{
      err=Setpoint-sensorRead;
       if (abs(Error) < IntThresh){              // prevent integral 'windup'
            ier = ier + err;                     // accumulate the error integral
        }
        else {
              ier=0;                             // zero it if out of bounds
              }
      der=prev_sensorRead-sensorRead;

      power=controlBias+ int(pGain*err) + int(dGain*der)+int(iGain*ier);
      }
      Serial.print(ultrasonic());
      Serial.print("      ");
      Serial.println(power);
      int mapped_power = map(power,0,1024,0,255);  
      analogWrite(coilPin,mapped_power);
      checkSerial();
      delay(50);
   }
  
  
}
