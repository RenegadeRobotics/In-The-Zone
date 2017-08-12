#pragma config(Sensor, in1,    bottomPOT,      sensorPotentiometer)
#pragma config(Sensor, in2,    topPOT,         sensorPotentiometer)
#pragma config(Sensor, dgtl2,  ,               sensorQuadEncoder)
#pragma config(Motor,  port2,           leftfront,     tmotorServoContinuousRotation, openLoop, driveLeft)
#pragma config(Motor,  port3,           leftback,      tmotorServoContinuousRotation, openLoop, driveLeft, encoderPort, None)
#pragma config(Motor,  port4,            ,             tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           bottomLift,    tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           topLift,       tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port7,           claw,          tmotorNone, openLoop)
#pragma config(Motor,  port8,           rightfront,    tmotorServoContinuousRotation, openLoop, driveRight)
#pragma config(Motor,  port9,           rightback,     tmotorServoContinuousRotation, openLoop, driveRight, encoderPort, dgtl2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................

	// Remove this function call once you have "real" code.
	AutonomousCodePlaceholderForTesting();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	int rightpower = 0;
	int leftpower = 0;
	int topPOTvalue;
	int bottomPOTvalue;
	int topPower = 0;
	int bottomPower = 0;
	int clawOpen = 0;
	int clawClose = 0;
	int maxPOTvalue;
	int maxPOT = 100;

#define MAX_POWER 127
#define DEADBAND 5

	while (true)
	{
		leftpower = (vexRT[Ch3] + vexRT[Ch4]);
		rightpower = (vexRT[Ch3] - vexRT[Ch4]);

		if(abs(rightpower) > MAX_POWER){
			rightpower = sgn(rightpower) * MAX_POWER;
		}

		if(abs(leftpower) > MAX_POWER){
			leftpower = sgn(leftpower) * MAX_POWER;
		}

		if(abs(leftpower) < DEADBAND) leftpower = 0;
		if(abs(rightpower) < DEADBAND) rightpower = 0;


		// set left side motors;
		motor[leftfront] = leftpower;
		motor[leftback] = leftpower;

		// set right side motors;
		motor[rightfront] = rightpower;
		motor[rightback] = rightpower;

		// claw
		clawOpen = vexRT[Btn8U];
		clawClose = vexRT[Btn8D];

		if( clawClose == 1) motor[claw] = -100;
		else if( clawOpen == 1 ) motor[claw] = 100;

		//potentiometer limits
		topPOTvalue = SensorValue[topPOT];
		bottomPOTvalue = SensorValue[bottomPOT];

		// lift  to joystick
		topPower = vexRT[Ch2];
		bottomPower = vexRT[Ch2];

		if(abs(bottomPower) < DEADBAND) bottomPower = 0;
		if(abs(topPower) < DEADBAND) topPower = 0;


		// set bottom motors;
		motor[bottomLift] = bottomPower;

		// set top motors;
		motor[topLift] = topPower;

		// which POT is larger
		maxPOTvalue = topPOTvalue;
		if(bottomPOTvalue > maxPOTvalue) maxPOTvalue = bottomPOTvalue;

		// potentiometer limits part 2
		if(maxPOTvalue < maxPOT || maxPOTvalue > maxPOT && vexRT[Ch2] < -1 * DEADBAND){ motor[topLift] = vexRT[Ch2]; motor[bottomLift] = vexRT[Ch2];}
		else if(maxPOTvalue > maxPOT) topPower = bottomPower = 0;


	}
}
