#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(Sensor, in1,    bottomPOT,      sensorNone)
#pragma config(Sensor, in2,    topPOT,         sensorPotentiometer)
#pragma config(Sensor, dgtl1,  SonicSensor,    sensorSONAR_inch)
#pragma config(Sensor, dgtl4,  backLeftENC,    sensorQuadEncoder)
#pragma config(Motor,  port2,           rightBack,     tmotorServoContinuousRotation, openLoop, reversed, encoderPort, dgtl2)
#pragma config(Motor,  port3,           rightFront,    tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port5,           bottomLift,    tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           topLift,       tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port7,           claw,          tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port8,           leftBack,      tmotorServoContinuousRotation, openLoop, encoderPort, dgtl4)
#pragma config(Motor,  port9,           leftFront,     tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///SmartMotorLibrary by JPearman on the VEX forums
#include "Libraries/SmartMotorLib.c"
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



#pragma systemFile

/*---------------------------------------------------------------------------*/
/*                     Renegade Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void DriveForTime (int power, int time) {
	SetMotor(leftFront, power);
	SetMotor(rightFront, power);
	SetMotor(leftBack, power);
	SetMotor(rightBack, power);

	wait1Msec(time);

	SetMotor(leftFront, -10, true);
	SetMotor(rightFront, -10, true);
	SetMotor(leftBack, -10, true);
	SetMotor(rightBack, -10, true);

	wait1Msec (50);

	SetMotor(rightBack,0);
	SetMotor(rightFront, 0);
	SetMotor(leftBack, 0);
	SetMotor(leftFront, 0);
}


void DriveForClicks(int encChosen, int amountToGo, int power){

	// get initial encoder clicks; calculate ending click value
	// function uses relative values for encoder clicks instead of
	// resetting enc to 0
	int currentCount = SensorValue[encChosen];
	int finalAmount = currentCount + amountToGo;

	// turn on motors to desired power
	SetMotor(rightBack, power);
	SetMotor(rightFront, power);
	SetMotor(leftBack, power);
	SetMotor(leftFront, power);

	// keep checking sensor value with tiny wait
	// code will stay in this statement until clicks reached
	while(SensorValue[encChosen] <= finalAmount) {
		wait1Msec(20);
	}

	SetMotor(rightBack, -10, true);
	SetMotor(rightFront, -10, true);
	SetMotor(leftBack, -10, true);
	SetMotor(leftFront, -10, true);

	wait1Msec(50);

	SetMotor(rightBack,0);
	SetMotor(rightFront, 0);
	SetMotor(leftBack, 0);
	SetMotor(leftFront, 0);

}


// use an asterisk on direction variable below; makes it a pointer
// this is the way you have to pass strings/characters to a function in RobotC
void LiftUsingPOT (int power, int topPOTlimit, int bottomPOTlimit, string *direction){

	// get initial sensor values
	int topPOTvalue = SensorValue[topPOT];
	int bottomPOTvalue = SensorValue[bottomPOT];

	// turn on motors to desired power
	SetMotor(bottomLift, power);
	SetMotor(topLift, power);

	if ( *direction == "up" ) {

		// run this loop while both POTs are under our set limits
		while (topPOTvalue <= topPOTlimit && bottomPOTvalue <= bottomPOTlimit){

			//keep checking potentiometer values
			topPOTvalue = SensorValue[topPOT];
			bottomPOTvalue = SensorValue[bottomPOT];

			// add a small wait - you don't have to check the
			// sensor value all the time
			wait1Msec(50);
		}
	}

	// otherwise, the direction must be "down"
	else {

		// run this loop while both POTs are over our set limits
		while (topPOTvalue >= topPOTlimit && bottomPOTvalue >= bottomPOTlimit){

			//keep checking potentiometer values
			topPOTvalue = SensorValue[topPOT];
			bottomPOTvalue = SensorValue[bottomPOT];

			// add a small wait
			wait1Msec(10);
		}
	}

	// after POT limit is hit, turn off motors
	SetMotor(bottomLift, 0);
	SetMotor(topLift, 0);
}


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
	bStopTasksBetweenModes = false;
	SmartMotorsInit();
	SmartMotorRun();

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...

	clearLCDLine(0);
	clearLCDLine(1);

	while(nVexRCReceiveState & vrDisabled) {
		string strSensorValue = SensorValue [SonicSensor];
		displayLCDCenteredString (0,strSensorValue);


	}
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

task autonomous()
{

	// keep this wait statement here;
	// program does not seem to run the first
	// motor command without it
	wait1Msec(100);


	// Drive forward 70 power / 500ms
	// pushes cone forward so claw will be able to reach it
	DriveForTime (70, 500);

	wait1Msec(500);

	// Drive backward -70 power / 427ms
	// moves backward to give claw some grabbing room
	DriveForTime (-70, 427);

	wait1Msec(500);

	// Close Claw to pop it out from folded position
	SetMotor(claw, -80);
	wait1Msec(500);
	SetMotor(claw, 0);

	// small wait to avoid stressing motors, going from
	// close direction to open direction
	wait1Msec(100);

	// Open Claw after it's flipped out
	SetMotor(claw, 127);
	// smallish wait here because when it's all the way open
	// screw heads from the gusset get slightly stuck
	// underneath the c-channel & makes it harder to close
	wait1Msec(200);

	// apply just enough power to hold the claw open
	// but not so much that it stalls the motor
	SetMotor(claw, 20);

	// Drive forward, 70 power / 300ms
	// moves back to where cone is
	DriveForTime (70,300);

	wait1Msec(500);

	// Close Claw to grab cone
	SetMotor(claw, -80);
	wait1Msec(500);
	// apply a small amount of power to make sure claw
	// stays closed, but not so much it will stall.
	// need less power here because rubber bands help
	// keep claw closed
	SetMotor(claw, -25);

	//Raise lift: 127 power / bottomLimit 1500 / topLimit 1400
	LiftUsingPOT (127, 1500, 1400, "up");
	wait1Msec(1000);

	// Drive foward to tower
	// use back left encoder to measure / 115 clicks / 100 power
	DriveForClicks(backLeftENC, 115, 100);
	wait1Msec(500);

	// Lower lift: -40 power, bottomPOT limit 1300 / topPOT limit 1200
	// use really low power & small change in POT levels
	// because otherwise gravity adds too much "down"
	LiftUsingPOT(-40, 1400, 1300, "down");

	// Open claw after lift is down
	// OK to do this after lift is down because claw is hinged
	// and will fold instead of break
	SetMotor(claw, 100);
	wait1Msec(500);
	SetMotor(claw, 0);


	// Drive backward, -127 power / 300ms
	// to get away from tower
	DriveForTime (-127, 300);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/


task usercontrol(){
	displayLCDCenteredString(0, "driver");


	// chassis variables -------
	int rightpower = 0;
	int leftpower = 0;
	int adjLeft = 0;
	int adjRight = 0;
	int inverseBtn = 0;

	// claw variables -------
	int clawPower = 0;

	// lift variables --------
	int topPOTvalue;
	int bottomPOTvalue;

	int maxPOTtop = 2222;
	int maxPOTbottom = 2176;

	int topPower = 0;
	int bottomPower = 0;


	#define MAX_POWER 127
	#define DEADBAND 5

	while (true){
		////////// Chassis //////////
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
		SetMotor(leftFront, leftpower);
		SetMotor(leftBack, leftpower);

		// set right side motors;
		SetMotor(rightFront, rightpower);
		SetMotor(rightBack, rightpower);

		// small adjust
		adjLeft = vexRT[Btn7L];
		adjRight = vexRT[Btn7R];

		if (adjLeft == 1){
			SetMotor(rightFront, 70);
			SetMotor(rightBack, 70);
			SetMotor(leftFront, 0);
			SetMotor(leftBack, 0);
		}
		else if(adjRight == 1) {
			SetMotor(leftFront, 70);
			SetMotor(leftBack, 70);
			SetMotor(rightFront, 0);
			SetMotor(rightBack, 0);
		}


		////////// Claw //////////
	int clawVar;
	if (vexRT[Btn6UXmtr2] == 1) {
		clawVar = 1;
	}
	else if (vexRT[Btn6DXmtr2] == 1) {
		clawVar = 2;
	}

	else if (vexRT[Btn5UXmtr2] == 1) {
		clawVar = 3;

	}

	switch(clawVar){
		case 1:
			clawPower = -100;
			break;
		case 2:
			clawPower = 100;
			break;
		case 3:
			clawPower = -20;
			break;
		default:
			clawPower = 0;
			break;
	}

		SetMotor (claw, clawPower);
		////////// Lift //////////

		//potentiometer values
		topPOTvalue = SensorValue[topPOT];
		bottomPOTvalue = SensorValue[bottomPOT];


		// lift to joystick
		topPower = vexRT[Ch3Xmtr2];
		bottomPower = vexRT[Ch3Xmtr2];

		if (topPOTvalue >= maxPOTtop || bottomPOTvalue >= maxPOTbottom){
			if (vexRT[Ch3Xmtr2] > 0 ){
				topPower = 0;
				bottomPower = 0;
			}
		}
		SetMotor(bottomLift, bottomPower);
		SetMotor(topLift, topPower);

	}
}