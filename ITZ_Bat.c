#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(Sensor, in1,    bottomPOT,      sensorPotentiometer)
#pragma config(Sensor, in2,    topPOT,         sensorPotentiometer)
#pragma config(Sensor, dgtl2,  backLeftENC,    sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  backRightENC,   sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  SonicSensor,    sensorSONAR_cm)
#pragma config(Sensor, dgtl10, Align,          sensorSONAR_cm)
#pragma config(Motor,  port2,           rightBack,     tmotorVex393_MC29, openLoop, reversed, encoderPort, dgtl5)
#pragma config(Motor,  port3,           rightFront,    tmotorVex393_MC29, openLoop, reversed, encoderPort, dgtl5)
#pragma config(Motor,  port4,           MGflipper,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           bottomLift,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           topLift,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           leftBack,      tmotorVex393_MC29, openLoop, encoderPort, dgtl2)
#pragma config(Motor,  port9,           leftFront,     tmotorVex393_MC29, openLoop, encoderPort, dgtl2)
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

// Auton robot brakes
void stopDriving(string *direction){
	if (*direction == "forward"){
		//"true" makes it not slew
		SetMotor(leftFront, -30, true);
		SetMotor(rightFront, -30, true);
		SetMotor(leftBack, -30, true);
		SetMotor(rightBack, -30, true);

		wait1Msec(25);
	}
	else {
		SetMotor(leftFront, 30, true);
		SetMotor(rightFront, 30, true);
		SetMotor(leftBack, 30, true);
		SetMotor(rightBack, 30, true);

		wait1Msec(50);
	}

	SetMotor(rightBack, 0);
	SetMotor(leftBack, 0);
	SetMotor(leftFront, 0);
	SetMotor(rightFront, 0);

}


// use an asterisk on direction variable below; makes it a pointer
// this is the way you have to pass strings/characters to a function in RobotC
void LiftUsingPOT (int power, int topPOTlimit, string *direction, int killSwitch){

	clearTimer(T1);
	// get initial sensor values
	int topPOTvalue = SensorValue[topPOT];
	int bottomPOTvalue = SensorValue[bottomPOT];

	// turn on motors to desired power
	SetMotor(bottomLift, power);
	SetMotor(topLift, power);

	if (*direction == "up") {

		// run this loop while both POTs are under our set limits
		while ((topPOTvalue <= topPOTlimit) && (time1[T1] < killSwitch)){

			// add a small wait - you don't have to check the
			// sensor value all the time
			wait1Msec(20);

			//keep checking potentiometer values
			topPOTvalue = SensorValue[topPOT];
			bottomPOTvalue = SensorValue[bottomPOT];
		}
	}

	// otherwise, the direction must be "down"
	else {

		// run this loop while both POTs are over our set limits
		while ((topPOTvalue >= topPOTlimit) && (time1[T1] < killSwitch)){

			// add a small wait
			wait1Msec(20);

			//keep checking potentiometer values
			topPOTvalue = SensorValue[topPOT];
			bottomPOTvalue = SensorValue[bottomPOT];
		}
	}

	// after POT limit is hit, turn off motors
	SetMotor(bottomLift, 0);
	SetMotor(topLift, 0);
}






//PRE-AUTON FUNCTIONS/////////////////////
//LCD selector

//THIS IS THE VARIABLE THAT RUNS AUTON!
int auton = 0;


//ADD all autonomous choices
void displayAuton(){
	clearLCDLine(0);
	clearLCDLine(1);
	switch(auton){

	case 1://no auton
		displayLCDCenteredString(0, "no auton");
		break;

	case -1://sensor values
		break;

	default://auton
		displayLCDCenteredString(0, "autonomous");
		break;
	}
}



task LCDControl() {

	bLCDBacklight = 1;
	clearLCDLine(0);
	clearLCDLine(1);
	bool noButtonsPressed = true;
	displayAuton();

	while (true){
		if (auton == -1){
			clearLCDLine(0);
			clearLCDLine(1);
			displayLCDNumber(0, 0,SensorValue[Align], -1);
			displayLCDNumber(1, 0,SensorValue[SonicSensor], -1);
		}

		if (noButtonsPressed){ // only update auton if a button is pressed AND wasn't pressed previously
			switch(nLCDButtons){

			case kButtonLeft:
				auton--;
				if (auton < -1){
					auton = 1;
				}
				displayAuton();
				break;

			case kButtonCenter:
				displayLCDCenteredString(0, "Chosen");
				break;

			case kButtonRight:
				auton++;
				if (auton>1){
					auton=-1;
				}
				displayAuton();
				break;
			}
		}
		noButtonsPressed = !nLCDButtons; //update if there is a button currently pressed
		wait1Msec(20);
	}

}




///////////////////////////THIS IS OUR DEFAULT AUTON//////////
void defaultAuton (){
	bLCDBacklight = 0;

	int distance;
	distance = SensorValue[SonicSensor];

	//push cone forward//
	while (distance < 32.5){
		SetMotor(leftBack, 127);
		SetMotor(rightBack, 127);
		SetMotor(leftFront, 127);
		SetMotor(rightFront, 127);

		wait1Msec(20);
		distance = SensorValue[SonicSensor];
	}

	//"true" makes it not slew
	SetMotor(leftBack, 0, true);
	SetMotor(rightBack, 0, true);
	SetMotor(leftFront, 0, true);
	SetMotor(rightFront, 0, true);

	wait1Msec(500);

	//drive backwards to distance from the cone so claw can flip out
	//claw flips out bc of inertia


	SetMotor(leftBack, -80, true);
	SetMotor(rightBack, -80, true);
	SetMotor(leftFront, -80, true);
	SetMotor(rightFront, -80, true);

	wait1Msec(500);
	stopDriving("backward");
	wait1Msec(500);


	//Close calw to flip it out
	setMotor(claw, -127);
	wait1Msec(50);
	setMotor(claw,0);
	wait1Msec(500);
	setMotor (claw, 127);
	wait1Msec(200);
	setMotor (claw, 0);
	wait1Msec(500);

	//drive forward to get cone
	while (distance < 39){
		SetMotor(leftBack, 60);
		SetMotor(rightBack, 60);
		SetMotor(leftFront, 60);
		SetMotor(rightFront, 60);

		distance = SensorValue[SonicSensor];
		wait1Msec(20);

	}

	stopDriving("forward");
	wait1Msec(500);


	//close and hold claw
	SetMotor(claw, -100);
	wait1Msec(300);

	SetMotor(claw, -20);
	wait1Msec(1000);

	//Raise lift (80 power, 1500 Top POT, up, 6000(6 sec) kill time)
	LiftUsingPOT(80, 1500, "up",6000);
	distance = SensorValue[SonicSensor];


	//drive forward to tower
	while (distance < 62){
		SetMotor(leftBack, 60);
		SetMotor(rightBack, 60);
		SetMotor(leftFront, 60);
		SetMotor(rightFront, 60);

		wait1Msec(20);
		distance = SensorValue[SonicSensor];

	}

	//stop driving forward
	stopDriving("forward");
	wait1Msec(500);

	//move lift down with cone (-40 power, top POT 1400, down, 4000(4 sec) kill switch)
	//open claw
	LiftUsingPOT(-40, 1400, "down", 4000);
	SetMotor(claw, 80);
	wait1Msec(500);
	SetMotor(claw, 0);
	wait1Msec(500);

	// drive backward	to move away from tower after cone is on
	while (distance > 50){
		SetMotor(leftBack, -70);
		SetMotor(rightBack, -70);
		SetMotor(leftFront, -70);
		SetMotor(rightFront, -70);

		wait1Msec(20);
		distance = SensorValue[SonicSensor];
	}

	stopDriving("backward");

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

void pre_auton(){

	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.

	SmartMotorsInit();
	SmartMotorRun();

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	bStopTasksBetweenModes = false;
	bDisplayCompetitionStatusOnLcd = false;




	startTask(LCDControl);
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
	stopTask(LCDControl);
	switch(auton){
	case 1:
		break;
	case -1:
		defaultAuton();
		break;
	default:
		defaultAuton();
		break;

	}
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


	bLCDBacklight = false;

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

	//mobile goal variables -------
	int openFlipper;
	int closeFlipper;
	int MGflipperPower = 0;


#define MAX_POWER 127
#define DEADBAND 5

	while (true){
		////////// Chassis //////////
		leftpower = (vexRT[Ch3] + vexRT[Ch4]);
		rightpower = (vexRT[Ch3] - vexRT[Ch4]);

		if (abs(rightpower) > MAX_POWER){
			rightpower = sgn(rightpower) * MAX_POWER;
		}

		if (abs(leftpower) > MAX_POWER){
			leftpower = sgn(leftpower) * MAX_POWER;
		}

		if (abs(leftpower) < DEADBAND) leftpower = 0;
		if (abs(rightpower) < DEADBAND) rightpower = 0;

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

		// small adjust buttons
		adjLeft = vexRT[Btn7L];
		adjRight = vexRT[Btn7R];

		if (adjLeft == 1){
			SetMotor(rightFront, 70);
			SetMotor(rightBack, 70);
			SetMotor(leftFront, 0);
			SetMotor(leftBack, 0);
		}
		else if (adjRight == 1) {
			SetMotor(leftFront, 70);
			SetMotor(leftBack, 70);
			SetMotor(rightFront, 0);
			SetMotor(rightBack, 0);
		}


		////////// Claw //////////
		// 5U hold
		// 6U claw close
		// 6D claw open
		if (vexRT[Btn5UXmtr2] == 1) {
			clawPower = -20;
		}
		else if (vexRT[Btn6UXmtr2] == 1) {
			clawPower = -100;
		}
		else if (vexRT[Btn6DXmtr2] == 1) {
			clawPower = 100;
		}

		else clawPower = 0;


		SetMotor(claw, clawPower);

		////////// Lift //////////

		//potentiometer values
		topPOTvalue = SensorValue[topPOT];
		bottomPOTvalue = SensorValue[bottomPOT];


		// lift to joystick
		topPower = vexRT[Ch3Xmtr2];
		bottomPower = vexRT[Ch3Xmtr2];
		// Limitrs how high the lift can go
		if (topPOTvalue >= maxPOTtop || bottomPOTvalue >= maxPOTbottom){
			if (vexRT[Ch3Xmtr2] > 0 ){
				topPower = 0;
				bottomPower = 0;
			}
		}
		SetMotor(bottomLift, bottomPower);
		SetMotor(topLift, topPower);

		////////////// Mobile Goal Flipper Buttons //////////////

		openFlipper = vexRT[Btn8D];
		closeFlipper = vexRT[Btn8U];

		if (openFlipper == 1) {
			MGflipperPower = -127;
		}
		else if (closeFlipper == 1) {
			MGflipperPower = 127;
		}
		else MGflipperPower = 0;

		SetMotor(MGflipper, MGflipperPower);
	}
}
