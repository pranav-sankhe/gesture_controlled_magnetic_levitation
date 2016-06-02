#include "WProgram.h"
double readHeight(int speedToSample);
void balance(int direction, int power);
/* Floating Magnets
 * ---------------- 
 * 
 * Created 10 May 2007
 * copyleft 2007 Jonathan Hodgins
 */

const int off = 2, up=1, down=0, none=2, high=1, low=0;

double holdHeight=190.0, smallHeightChange=0.3,largeHeightChange=1.0;//190.0 in dark room
double lowTolerence=0.5,highTolerence=5.0,triggerBalance=4.0;
int count=0,lowCountTolerence=90,highCountTolerence=160,countOut=580;

int startButtonPin=6;
int startButton=LOW;
int setButtonPin=7;
int setButton=LOW;

long startTime=millis();
long currentTime=millis();

const int quick=4, detailed=80;
double sensorReading=0.0;
int sensorPin = 0;
int primaryCoilPins[2]={
  2,3};
int secondaryCoilPins[2]={
  4,5};

int ledPin[3]={
  9,10,11};//ledPin[1] is the bicolour LED cathode

void setup() 
{
  for(int i=0;i<2;i++)
  {
    pinMode(primaryCoilPins[i],OUTPUT);
    digitalWrite(primaryCoilPins[i],LOW);
    pinMode(secondaryCoilPins[i],OUTPUT);
    digitalWrite(secondaryCoilPins[i],LOW);
  }
  
  pinMode(ledPin[0],OUTPUT);
  pinMode(ledPin[1],OUTPUT);
  digitalWrite(ledPin[1],LOW);
  pinMode(ledPin[2],OUTPUT);
  
  pinMode(startButtonPin,INPUT);
}

void loop()
{
  sensorReading = analogRead(sensorPin)+0.0;

  currentTime=millis();
  if(currentTime >= startTime + 200)
  {
    startButton = digitalRead(startButtonPin);
    if(startButton==HIGH)
      holdHeight=readHeight(detailed);
    digitalWrite(ledPin[0],LOW);
    digitalWrite(ledPin[2],LOW);

    if (abs(count) < countOut)
    {
      holdHeight-=smallHeightChange/2;
      if(count < -lowCountTolerence)
      {
        holdHeight+=smallHeightChange;
        digitalWrite(ledPin[2],HIGH);
      }
      else if(count < -highCountTolerence)
      {
        holdHeight+=largeHeightChange;
        digitalWrite(ledPin[2],HIGH);
      }
      else if(count > lowCountTolerence)
      {
        holdHeight-=smallHeightChange;
        digitalWrite(ledPin[0],HIGH);
      }
      else if(count > highCountTolerence)
      {
        holdHeight-=largeHeightChange;
        digitalWrite(ledPin[0],HIGH);
      }
    }
    count=0;
    startTime=millis();
  }

  if(sensorReading < holdHeight-highTolerence+triggerBalance)
  {
    balance(up,high);
  }
  else if (sensorReading < holdHeight-lowTolerence)
  {
    balance(up,low);
  }
  else if(sensorReading > holdHeight+highTolerence)
  {
    balance(down,high);
  }
  else if (sensorReading > holdHeight+lowTolerence+triggerBalance)
  {
    balance(down,low);
  }
  else
  {
    balance(off,low);
  }

  /*balance(off,low);
   digitalWrite(ledPin[2],LOW);
   delay(200);
   balance(up,high);
   digitalWrite(ledPin[0],HIGH);
   delay(400);
   balance(down,high);
   digitalWrite(ledPin[0],LOW);
   digitalWrite(ledPin[2],HIGH);
   delay(400);*/

}

double readHeight(int speedToSample)
{
  int numSamples=speedToSample;
  double avg=0.0;
  for(int i=0;i<numSamples;i++)
    avg += (analogRead(sensorPin)+0.0);
  avg = avg/(numSamples+0.0);

  return avg;
}

void balance(int direction, int power)
{
  if(direction==up)
  {
    count-=1;
    digitalWrite(primaryCoilPins[0],LOW);
    digitalWrite(primaryCoilPins[1],HIGH);
    if(power==high)
    {
      count-=2;
      digitalWrite(secondaryCoilPins[0],LOW);
      digitalWrite(secondaryCoilPins[1],HIGH);
    }
    else
    {
      digitalWrite(secondaryCoilPins[1],LOW);
      digitalWrite(secondaryCoilPins[0],LOW);
    }
  }
  else if(direction==down)
  {
    count+=1;
    digitalWrite(primaryCoilPins[0],HIGH);
    digitalWrite(primaryCoilPins[1],LOW);
    if(power==high)
    {
      count+=2;
      digitalWrite(secondaryCoilPins[0],HIGH);
      digitalWrite(secondaryCoilPins[1],LOW);
    }
    else
    {
      digitalWrite(secondaryCoilPins[1],LOW);
      digitalWrite(secondaryCoilPins[0],LOW);
    }
  }
  else
  {
    digitalWrite(primaryCoilPins[0],LOW);
    digitalWrite(primaryCoilPins[1],LOW);
    digitalWrite(secondaryCoilPins[0],LOW);
    digitalWrite(secondaryCoilPins[1],LOW);
  }
}
