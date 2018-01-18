#pragma config(Sensor, dgtl1,  Arm,            sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rtBack,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  ltBack,         sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  MG,             sensorQuadEncoder)
#pragma config(Motor,  port2,           FR,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           BR,            tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, dgtl3)
#pragma config(Motor,  port4,           FL,            tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           BL,            tmotorVex393HighSpeed_MC29, openLoop, encoderPort, dgtl5)
#pragma config(Motor,  port6,           MGLift,        tmotorVex393_MC29, openLoop, encoderPort, dgtl7)
#pragma config(Motor,  port7,           ConeArm,       tmotorVex393_MC29, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port8,           Roller,        tmotorVex393_MC29, openLoop)
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
	SmartMotorsInit();
	SmartMotorRun();
	bStopTasksBetweenModes = false;

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
	//move backward to distance self from cone at 40 power and stop
	SetMotor (FR, -90);
	SetMotor (FL, -90);
	SetMotor (BR, -90);
	SetMotor (BL, -90);
	wait1Msec(175);
	SetMotor (FR, 0);
	SetMotor (FL, 0);
	SetMotor (BR, 0);
	SetMotor (BL, 0);
	wait1Msec(20);

	//pick up cone(arm down, and roll intake)
	while (SensorValue[Arm] > -264){
		SetMotor(ConeArm, -90);
		wait1Msec(20);
	}
	SetMotor (ConeArm, 0);

	SetMotor (Roller, 80);
	wait1Msec(500);
	SetMotor (Roller, 20);


	//arm up (straight)
	while (SensorValue[Arm] < -190){
		SetMotor(ConeArm, 90);
		wait1Msec(20);
	}
	SetMotor(ConeArm, 0);

	// move MG to lowest level
	while (SensorValue[MG] < 250){
		SetMotor(MGLift,-50);
		wait1Msec(20);
	}
	SetMotor(MGLift, 0);
	wait1Msec(20);

	//drive backward to get mobile goal
	while (SensorValue[rtBack] < 1850){
		SetMotor (FR, -80);
		SetMotor (FL, -80);
		SetMotor (BR, -80);
		SetMotor (BL, -80);
		wait1Msec(20);
	}
	SetMotor (FR, 0);
	SetMotor (FL, 0);
	SetMotor (BR,0);
	SetMotor (BL, 0);


	//move MG picker upper up
	while (SensorValue[MG] < 800){
		SetMotor(MGLift,-90);
		wait1Msec(20);
	}
	SetMotor(MGLift,0);
	wait1Msec(20);

	// place cone on mobile goal
	clearTimer(T1);
	int killTimer = 1500;
	while ((SensorValue[Arm] < -29) && (time1[T1] < killTimer)){
		SetMotor(ConeArm, 25);
		wait1Msec(20);

	}
	SetMotor(ConeArm, 0);
	SetMotor(Roller, 0);
	wait1Msec(20);

	// drive back to 5-point zone
	while (SensorValue[rtBack] > 600){
		SetMotor (FR, 80);
		SetMotor (FL, 80);
		SetMotor (BR, 80);
		SetMotor (BL, 80);
		wait1Msec(20);
	}
	SetMotor (FR, 0);
	SetMotor (FL, 0);
	SetMotor (BR, 0);
	SetMotor (BL, 0);
	wait1Msec(50);

	// Turn in 5-point zone
	SetMotor (FR, 80);
	SetMotor (FL, -80);
	SetMotor (BR, 80);
	SetMotor (BL, -80);
	wait1Msec(1200);

	SetMotor (FR, 0);
	SetMotor (FL, 0);
	SetMotor (BR, 0);
	SetMotor (BL, 0);
	wait1Msec(50);

	//rolly outake lift up arm
	SetMotor(Roller, -127);
	wait1Msec(200);
	while (SensorValue[Arm] > -100){
		SetMotor(ConeArm, -127);
		wait1Msec(20);
	}
	SetMotor (ConeArm, 0);
	SetMotor (Roller, 0);
	wait1Msec(50);


	//lower MG
	while (SensorValue[MG] > 450){
		SetMotor(MGLift,90);
		wait1Msec(20);
	}
	SetMotor(MGLift, 0);

	// drive away
	SetMotor (FR, 127);
	SetMotor (FL, 127);
	SetMotor (BR, 127);
	SetMotor (BL, 127);
	wait1Msec(500);

	SetMotor (FR, 0);
	SetMotor (FL, 0);
	SetMotor (BR, 0);
	SetMotor (BL, 0);

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

	// User control code here, inside the loop

	// variables -------
	int rightpower = 0;
	int leftpower = 0;

	int MGliftPower = 0;
	int MGliftValue;
	int ConeArmPower = 0;


#define MAX_POWER 127
#define DEADBAND 15

	while (true)
	{
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


		//MGLift
		MGliftValue = SensorValue[MG];
		if(vexRT[Btn6D] == 1 && MGliftValue > -190){
			MGliftPower = 90;
		}
		else if (vexRT[Btn6U] == 1 && MGliftValue < 1100){
			MGliftPower = -50;
		}
		else{
			MGliftPower = 0;
		}
		SetMotor(MGLift,MGliftPower);

		// MG lift - move to bottom button
		if(vexRT[Btn5D] == 1){
			if(SensorValue[MG] > 190){
				while (SensorValue[MG] > 600){
					SetMotor(MGLift,90);
					wait1Msec(20);
				}
				SetMotor(MGLift,0);
			}

			else{
				while (SensorValue[MG] < 250){
					SetMotor(MGLift,-50);
					wait1Msec(20);
				}
				SetMotor(MGLift,0);

			}

		}

		//MGpusher
		if (vexRT[Btn7U] == 1) {
			SetMotor(MGpusher, 127);
		}
		else if (vexRT[Btn7D] == 1) {
			SetMotor(MGpusher, -127);
		}
		else SetMotor(MGpusher,0);




		//Rolly Intake
		if(vexRT[Btn6UXmtr2] == 1){
			SetMotor(Roller, 80);
		}

		else if (vexRT[Btn6DXmtr2] == 1){
			SetMotor(Roller, -80);
		}
		else{
			SetMotor(Roller, 0);
		}


		// Arm

		ConeArmPower = vexRT[Ch3Xmtr2];
		SetMotor(ConeArm,ConeArmPower);

	}
}
