

/*
*contibutors : pranav sankhe                                                 //add your name if you are contribting.
*started on : 2nd june 2016

*/

#include <Stepper.h>
#include <Wire.h>                                     //I2C      

#include <MPU6050_6Axis_MotionApps20.h>				  
#include <MPU6050.h>                                 //MPU 6050
#include <MPU6050_9Axis_MotionApps41.h>
//#include <helper_3dmath.h>

#include <PID_v1.h>                                  //PID 

int trig = 2;
int echo = 3;
int electromagnet = 5;           // th data cable which controls the electromagnet current. 

/*
Stepper stepper_+9*+```x(stepsPerRevolution, 8,9,10,11);
Stepper stepper_y(stepsPerRevolution, 8,9,10,11);                //use a multiplexer circuit to reduce the number of pins.
Stepper stepper_z(stepsPerRevolution, 8,9,10,11);
*/

int ultrasonic_read = 0;
double Setpoint, Input, Output;                                //the input, output and setpoint of the PID controller
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);           // kp = 2; ki = 5; kd=1;
Setpoint = 100;                                                // change this after calibration of the electromagnet



void setup()
{
	Serial.begin(9600);
	myPID.SetMode(AUTOMATIC);
	pinMode(trig,OUTPUT);
	pinMode(echo,INPUT);
	pinMode(electromagnet,OUTPUT);
}


void loop()
{
	ultrasonic_read = ultrasonic();								    //we have to implement PID algo. set a setup point. get the error through 
	
	input = ultrasonic_read;
  	myPID.Compute();
  	analogWrite(5,output);				             //output that will controll the current .volltage controlled current sources . (mosfets)

							                                   //the ultrasonic sensor reading and then reduce the error.    


}

int ultrasonic()
{   digitalWrite(trig,LOW);
	digitalWrite(echo,LOW);
	delayMicroseconds(2);
	digitalWrite(trig,HIGH);
	delayMicroseconds(10);
	digitalWrite(trig,LOW);

	long int duration = pulseIn(echo,HIGH);
	int distance = duration/2/29.1; 
	return distance ; 
	//delay(20);
}



/*

//
**PID()
Description

Creates a PID controller linked to the specified Input, Output, and Setpoint. The PID algorithm is in parallel form.
Syntax

PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction)
Parameters

Input: The variable we're trying to control (double)
Output: The variable that will be adjusted by the pid (double)
Setpoint: The value we want to Input to maintain (double)
Kp, Ki, Kd: Tuning Parameters. these affect how the pid will chage the output. (double>=0)
Direction: Either DIRECT or REVERSE. determines which direction the output will move when faced with a given error. DIRECT is most common.

//
**Compute()
Description
Contains the pid algorithm. it should be called once every loop(). Most of the time it will just return without doing anything. At a frequency specified by SetSampleTime it will calculate a new Output.
Compute()  //no arguments

//
**SetMode()
Description
Specifies whether the PID should be on (Automatic) or off (Manual.) The PID defaults to the off position when created.
SetMode(mode)       mode: AUTOMATIC or MANUAL

//
**SetOutputLimits()
The PID controller is designed to vary its output within a given range. By default this range is 0-255: the arduino PWM range. There's no use sending 300, 400, or 500 to the PWM. Depending on the application though, a different range may be desired.
SetOutputLimits(min, max)
min: Low end of the range. must be < max (double)
max: High end of the range. must be > min (double)

//
**SetTunings()
Tuning parameters (or "Tunings") dictate the dynamic behavior of the PID. Will it oscillate or not? Will it be fast or slow? An initial set of Tunings is specified when the PID is created. For most users this will be enough. There are times however, tunings need to be changed during run-time. At those times this function can be called.
SetTunings(Kp, Ki, Kd)
Kp: Determines how aggressively the PID reacts to the current amount of error (Proportional) (double >=0)
Ki: Determines how aggressively the PID reacts to error over time (Integral) (double>=0)
Kd: Determines how aggressively the PID reacts to the change in error (Derivative) (double>=0)

//
**SetSampleTime()
Determines how often the PID algorithm evaluates. The default is 200mS. For robotics applications this may need to be faster, but for the most part 200mS is plenty fast.
SetSampleTime(SampleTime)
SampleTime: How often, in milliseconds, the PID will be evaluated. (int>0)

//
**SetControllerDirection()
If my Input is above Setpoint, should the output be increased or decreased? Depending on what the PID is connected to, either could be true. With a car, the output should be decreased to bring the speed down. For a refrigerator, the opposite is true. The output (cooling) needs to be increased to bring my temperature down.
This function specifies which type of process the PID is connected to. This information is also specified when the PID constructed. Since it's unlikely that the process will switch from direct to reverse, it's equally unlikely that anyone will actually use this function.
SetControllerDirection(Direction);
Direction: DIRECT (like a car) or REVERSE (like a refrigerator)


//
**Display Functions
GetKp()
GetKi()
GetKd()
GetMode()
GetDirection()
Decription

These functions query the PID internals to get current values. These are useful for display purposes.
Syntax

GetKp()
GetKi()
GetKd()
GetMode()
GetDirection()


/*
Stepper library info : 
Stepper myStepper(stepsPerRevolution, 8,9,10,11);
myStepper.setSpeed()
myStepper.step()
*/