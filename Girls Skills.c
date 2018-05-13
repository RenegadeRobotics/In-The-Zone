#pragma config(Sensor, dgtl1,  Arm,            sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rtBack,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  ltBack,         sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  MG,             sensorQuadEncoder)
#pragma config(Motor,  port2,           FR,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           BR,            tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, dgtl3)
#pragma config(Motor,  port4,           ConeArm,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           Roller,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           MGLift,        tmotorVex393_MC29, openLoop, encoderPort, dgtl7)
#pragma config(Motor,  port7,           FL,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           BL,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           MGpusher,      tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Libraries/SmartMotorLib.c"
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
// line below prevents errors from unused functions in SmartMotor Library
#pragma systemFile


int clicksToGo;
float rampingPowerAmount;

//Change values here:
//Be sure to have float as deciaml
float rampUpClicks = 300.0;
float rampDownClicks = 150.0;

//The lowest value that moves the robot
float minToMove = 25.0;

int useThisforRightPower;
int useThisforLeftPower;
int leftAbs;

int error;
int integral;
int derivative;
int lastError;

float Kp = 1.5;
float Ki = 0.1;
float Kd = 0.05;

//Drive Function
void driveFunc(int leftPower, int rightPower){
	SetMotor (FR, rightPower);
	SetMotor (FL, leftPower);
	SetMotor (BR, rightPower);
	SetMotor (BL, leftPower);
}
// Reset everything
void resetEverything () {
	SensorValue[rtBack] = 0;
	SensorValue[ltBack] = 0;

	clicksToGo = 0;
	rampingPowerAmount = 0;
	useThisforRightPower = 0;
	useThisforLeftPower = 0;
	leftAbs = 0;

	error = 0;
	integral = 0;
	derivative = 0;
	lastError = 0;
}

//PID Driving
void encDriving(int Lpower, int targetClicks){

	resetEverything();
	rampingPowerAmount = abs(Lpower) - minToMove;

	while(abs(SensorValue[ltBack]) < targetClicks){

		//Set variables
		leftAbs = abs(SensorValue[ltBack]);
		clicksToGo = targetClicks - leftAbs;

		//Ramp up
		if (leftAbs < rampUpClicks){
			useThisforLeftPower = ((rampingPowerAmount / rampUpClicks) * leftAbs + minToMove) * sgn(Lpower);
		}
		//Ramp Down
		else if(clicksToGo < rampDownClicks){
			useThisforLeftPower = ((rampingPowerAmount / rampDownClicks) * clicksToGo + minToMove) * sgn(Lpower);
		}
		//Drive at Lpower. Use abs to allow negatives
		else {
			useThisforLeftPower = Lpower;
		}

		//Match the sides
		error = SensorValue[ltBack] - SensorValue[rtBack];
		integral = integral + error;
		derivative = error - lastError;
		lastError = error;

		// sgn makes the sign match the origional function call
		useThisforRightPower = useThisforLeftPower + error * Kp + integral * Ki + derivative * Kd;


		driveFunc(useThisforLeftPower, useThisforRightPower);
		wait1Msec(20);

		//Get these values to graph
		datalogDataGroupStart();
		datalogAddValue( 0, error);
		datalogAddValue( 1, integral);
		datalogAddValue( 2, SensorValue[ltBack]);
		datalogAddValue( 3, SensorValue[rtBack]);
		datalogAddValue( 4, useThisforLeftPower );
		datalogAddValue( 5, useThisforRightPower );
		datalogDataGroupEnd();

	}

	// put on the brakes
	driveFunc(-15, -15);
	wait1Msec(100);

	// stop
	driveFunc(0,0);
}


// Setting MG Lift joystick buttons
// into task so that it will allow chassis to run at same time
int MGliftPower = 0;

task MGLiftTask(){

	while(true){
		// set up & down buttons
		int MGliftValue = SensorValue[MG];
		if(vexRT[Btn6DXmtr2] == 1 && MGliftValue > -190){
			MGliftPower = 90;
		}
		else if (vexRT[Btn6UXmtr2] == 1 && MGliftValue < 1100) {
			MGliftPower = -90;
		}
		else{
			MGliftPower = 0;
		}
		SetMotor(MGLift, MGliftPower);

		// MG lift - move-to-bottom button
		if(vexRT[Btn8DXmtr2] == 1){
			if(SensorValue[MG] > 190){
				while(SensorValue[MG] > 550){
					SetMotor(MGLift, 127);
					wait1Msec(20);
				}
				SetMotor(MGLift, 0);
			}

			else{
				while(SensorValue[MG] < 250){
					SetMotor(MGLift, -127);
					wait1Msec(20);
				}
				SetMotor(MGLift, 0);
			}
		} // end auto-down

		// dont' hog cortex
		wait1Msec(20);

	} // end while(true)
} // MGLiftTask


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
	SensorValue[Arm] = 0;
	SensorValue[MG] = 0;
	SensorValue[rtBack] = 0;
	SensorValue[ltBack] = 0;
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = false;
	SmartMotorsInit();
	SmartMotorsAddPowerExtender(ConeArm);
	SmartMotorLinkMotors(BR, FR);
	SmartMotorLinkMotors(BL, FL);
	SmartMotorCurrentMonitorEnable();

	SmartMotorRun();


	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	bDisplayCompetitionStatusOnLcd = false;

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
	// NEED COMMENt HERE
	//startTask(encAvgTask);

	//move backward to distance self from cone and stop
	driveFunc(-90, -90);
	wait1Msec(150);
	driveFunc(0, 0);
	wait1Msec(20);

	//pick up cone(arm down, and roll intake)
	clearTimer(T1);
	int killTimer = 2500;
	while ((SensorValue[Arm] > -264) && (time1[T1] < killTimer)){
		SetMotor(ConeArm, -90);
		wait1Msec(20);
	}
	SetMotor (ConeArm, 0);

	SetMotor (Roller, 127);
	wait1Msec(600);
	SetMotor (Roller, 20);


	//arm up (straight)
	while (SensorValue[Arm] < -190){
		SetMotor(ConeArm, 90);
		wait1Msec(20);
	}
	SetMotor(ConeArm, 0);

	// move MG to lowest level
	while (SensorValue[MG] < 250){
		SetMotor(MGLift, -50);
		wait1Msec(20);
	}
	SetMotor(MGLift, 0);
	wait1Msec(20);

	//drive backward to get mobile goal
	encDriving(-60, 900);

	//move MG picker upper up
	while (SensorValue[MG] < 800){
		SetMotor(MGLift, -90);
		wait1Msec(20);
	}
	SetMotor(MGLift, 0);
	wait1Msec(20);

	// place cone on mobile goal
	clearTimer(T1);
	killTimer = 1500;
	while ((SensorValue[Arm] < -29) && (time1[T1] < killTimer)){
		SetMotor(ConeArm, 25);
		wait1Msec(20);

	}
	SetMotor(ConeArm, 0);
	SetMotor(Roller, 0);
	wait1Msec(20);


	// ----- ZIG-ZAG PATTERN STARTS HERE -----//
	//drive forward straight #1

	//straight #1
	encDriving(80, 750);
	wait1Msec(100);

	//turn zigzag #1
	driveFunc(-75, 75);
	wait1Msec(700);

	driveFunc(0, 0);
	wait1Msec(500);

	//straight #2
	encDriving(-127,200);
	wait1Msec(400);

	//turn fig zag #2
	driveFunc(-65, 65);
	wait1Msec(775);

	driveFunc(0, 0);
	wait1Msec(500);

	// rolly outake lift up arm
	SetMotor(Roller, -127);
	wait1Msec(200);
	while (SensorValue[Arm] > -100){
		SetMotor(ConeArm, -127);
		wait1Msec(20);
	}
	SetMotor (ConeArm, 0);
	SetMotor (Roller, 0);
	wait1Msec(50);

	//move MG picker upper up
	while (SensorValue[MG] < 1000){
		SetMotor(MGLift, -90);
		wait1Msec(20);
	}
	SetMotor(MGLift, 0);
	wait1Msec(20);

	//drive to 20 point zone
	driveFunc(-80,-80);
	wait1Msec(1204);


	//turn off MGLift
	SetMotor(MGLift, 0);
	wait1Msec(50);

	// MGpusher to push MG off robot
	SetMotor (MGpusher, 127);
	wait1Msec(1500);

	SetMotor (MGpusher, 0);


	//Drive backwards
	driveFunc(70,70);
	wait1Msec(900);

	driveFunc(0,0);
	wait1Msec(500);

	//turn towards parking
	driveFunc(-60, 60);
	wait1Msec(500);
	driveFunc(0, 0);
	wait1Msec(500);

	//Drive backwards
	encDriving(60, 1800);
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
	stopTask(autonomous);
	startTask(MGLiftTask);

	// WHY IS THIS COMMENT HERE?
	// User control code here, inside the loop

	// variables -------
	int rightpower = 0;
	int leftpower = 0;

	int ConeArmPower = 0;


#define MAX_POWER 127
#define DEADBAND 15

	while (true)
	{
		// Chassis
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


		// set left side motors;
		SetMotor(FL, leftpower);
		SetMotor(BL, leftpower);

		// set right side motors;
		SetMotor(FR, rightpower);
		SetMotor(BR, rightpower);

		// MGpusher
		// LET'S CODE THIS WITH OUR NEW 1-LINE CODE
		if (vexRT[Btn5UXmtr2] == 1) {
			SetMotor(MGpusher, 127);
		}
		else if (vexRT[Btn5DXmtr2] == 1) {
			SetMotor(MGpusher, -127);
		}
		else SetMotor(MGpusher, 0);

		// new roller button code
		// MGliftPower = (vexRT[Btn6UXmtr2] - vexRT[Btn6DXmtr2]) * 80;


		// Arm

		ConeArmPower = vexRT[Ch3Xmtr2];
		SetMotor(ConeArm, ConeArmPower);

	}
	stopTask(MGLiftTask);
}
