#pragma config(Sensor, in1,    bottomPOT,      sensorPotentiometer)
#pragma config(Sensor, in2,    topPOT,         sensorPotentiometer)
#pragma config(Sensor, dgtl2,  backRightENC,   sensorQuadEncoder)
#pragma config(Sensor, dgtl4,  backLeftENC,    sensorQuadEncoder)
#pragma config(Motor,  port2,           rightBack,     tmotorServoContinuousRotation, openLoop, reversed, encoderPort, dgtl2)
#pragma config(Motor,  port3,           rightFront,    tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port5,           bottomLift,    tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           topLift,       tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port7,           claw,          tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port8,           leftBack,      tmotorServoContinuousRotation, openLoop, encoderPort, dgtl4)
#pragma config(Motor,  port9,           leftFront,     tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download mthod as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

///SmartMotorLibrary by JPearman on the VEX forums
#include "Libraries/SmartMotorLib.c"
#pragma systemFile

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
void waitUntilQuadrature(int sensorChosen, int amountToGo){
	int currentCount = SensorValue[sensorChosen];
	int finalAmount = currentCount + amountToGo;
	while(SensorValue[sensorChosen] <= finalAmount) {wait1Msec(10);}
}
void DriveStraight(int clicks, int power){
	SetMotor(rightBack, power, false);
	SetMotor (rightFront, power, false);
	SetMotor (leftBack, power, false);
	SetMotor (leftFront, power, false);
	//untilEnconderCounts(clicks,dgtl3);
	waitUntilQuadrature(backLeftENC, clicks);
	SetMotor(rightBack, 0, false);
	SetMotor (rightFront, 0, false);
	SetMotor (leftBack, 0, false);
	SetMotor (leftFront, 0, false);


}

void changeClaw (int direction){
	if (direction == 1) { // 1 = open
		SetMotor (claw,50,false);
	}

	else {
		SetMotor (claw,-50,false);
	}
}




void pointTurn (int clicks, int power) {
	SetMotor(rightBack, -1*power, false);
	SetMotor (rightFront, -1*power, false);
	SetMotor (leftBack, power, false);
	SetMotor (leftFront, power, false);
	waitUntilQuadrature(backLeftENC, clicks);
	SetMotor(rightBack, 0, false);
	SetMotor (rightFront, 0, false);
	SetMotor (leftBack, 0, false);
	SetMotor (leftFront, 0, false);

}

void liftHeight (int time, int power){
	SetMotor (bottomLift, power, false);
	SetMotor (topLift, power, false);
	wait1Msec(time);
	SetMotor (topLift, 0, false);
	SetMotor (bottomLift, 0, false);

}

void liftArm (int time, int power) {
}
void pre_auton()
{

	SmartMotorsInit();
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
	SmartMotorRun();
	SmartMotorPtcMonitorEnable ();


	// ..........................................................................
	// Insert user code here.
	// ..........................................................................

	// Remove this function call once you have "real" code.


	////Close Claw

	changeClaw (0);


	///Raise lift

	liftHeight (1000,127);


	///Drive foward

	DriveStraight (553,100);

	///Open claw

	changeClaw (1);

	///Go backwards

	DriveStraight (303,-100);



	///Turn towards mobile goal

	pointTurn (-310, 110);
	///Lower lift

	liftHeight(1000,-127);

	/// Drive foward
	DriveStraight(932,127);

	//close claw
	changeClaw(0);

	//lift lift

	liftHeight(700,127);

	//Turn

	pointTurn(-18,127);

	///Drive foward

	DriveStraight(300,127);

	////lower lift

	liftHeight(500,-127)

	//open claw
	changeClaw(1);





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


task usercontrol(){

	SmartMotorRun();
	SmartMotorPtcMonitorEnable();
	// chassis variables -------
	int rightpower = 0;
	int leftpower = 0;
	int adjLeft = 0;
	int adjRight = 0;
	int inverseBtn = 0;

	// claw variables -------
	int clawOpen = 0;
	int clawClose = 0;

	// lift variables --------
	int topPOTvalue;
	int bottomPOTvalue;
	int maxPOTvalue;
	int maxPOTtop = 2222;
	// int minPOTtop = 533;
	// int minPOTbottom = 640;
	int maxPOTbottom = 2176;

	int topPower = 0;
	int bottomPower = 0;


#define MAX_POWER 127
#define DEADBAND 5

	while (true){
		/////////////////////////////////// CHASSIS
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

		// make the back of the robot the front
		// by pressing Button 6U
		inverseBtn = vexRT(Btn6U);
		if (inverseBtn == 1){
			leftpower = leftpower*-1;
			rightpower = rightpower*-1;
		}

		// set left side motors;
		SetMotor(leftFront, leftpower, false);
		SetMotor(leftBack, leftpower, false);

		// set right side motors;
		SetMotor(rightFront, rightpower, false);
		SetMotor(rightBack, rightpower, false);

		// small adjust
		adjLeft = vexRT[Btn7L];
		adjRight = vexRT[Btn7R];

		if (adjLeft == 1){
			SetMotor(rightFront, 70, false);
			SetMotor(rightBack, 70, false);
			SetMotor(leftFront, 0, false);
			SetMotor(leftBack, 0, false);
		}
		else if(adjRight == 1) {
			SetMotor(leftFront, 70, false);
			SetMotor(leftBack, 70, false);
			SetMotor(rightFront, 0, false);
			SetMotor(rightBack, 0, false);
		}

		/////////////////////////////// CLAW
		//clawOpen = 0;
		//clawClose = 0;
		clawOpen = vexRT[Btn8UXmtr2];
		clawClose = vexRT[Btn8DXmtr2];

		if(clawClose == 1) SetMotor(claw, -100, false);
		else if( clawOpen == 1 ) SetMotor(claw, 100, false);
		else SetMotor(claw, 0, false);



		//////////////////////////////// LIFT

		//potentiometer values
		topPOTvalue = SensorValue[topPOT];
		bottomPOTvalue = SensorValue[bottomPOT];


		//LIFT
		topPower = vexRT[Ch3Xmtr2];
		bottomPower = vexRT[Ch3Xmtr2];

		if (topPOTvalue >= maxPOTtop || bottomPOTvalue >= maxPOTbottom){
			if (vexRT[Ch3Xmtr2] > 0 ){
				topPower = 0;
				bottomPower = 0;
			}
		}
		SetMotor(bottomLift, bottomPower, false);
			SetMotor(topLift, topPower, false);

		}
	}


