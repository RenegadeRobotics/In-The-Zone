#pragma config(Sensor, dgtl1,  Arm,            sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rtBack,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  ltBack,         sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  MG,             sensorQuadEncoder)
#pragma config(Motor,  port2,           BL,            tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           BR,            tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port4,           FL,            tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           FR,            tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port6,           MGLift,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           ConeArm,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           Roller,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           MGpusher,      tmotorVex393_MC29, openLoop)
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
	//move backward to distance self from cone at 40 power and stop
	setMotor (FR, 40);
	setMotor (FL, 40);
	setMotor (BR, 40);
	setMotor (BL, 40);
	wait1Msec(250);
	setMotor (FR, 0);
	setMotor (FL, 0);
	setMotor (BR, 0);
	setMotor (BL, 0);
	wait1Msec (100);

	//pick up cone(arm down, and roll intake)
	while (SensorValue[Arm] > -264){
		setMotor(ConeArm, 60);
		wait1Msec(20);
		setMotor (Roller, 80);
		wait1Msec(200);
		setMotor (Roller, 0);


		//arm up (straight)
		while (SensorValue[Arm] < -132){
			setMotor(ConeArm, 60);
			wait1Msec(20);

			//drive backward to get mobile goal
				while (SensorValue[rtBack] < 999){
						setMotor (FR, 40);
						setMotor (FL, 40);
						setMotor (BR, 40);
						setMotor (BL, 40);
						wait1Msec(20);
				}

			//lift MG picker upper up

			// place cone on mobile goal

			// turn 180 degrees

			//drive backward
			//lower MG lifer in 5 point zone

			// move forward away from cone


		}


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
		// User control code here, inside the loop
		// chassis variables -------
		int rightpower = 0;
		int leftpower = 0;

		int MGliftPower = 0;
		int MGliftValue;
		int ConeArmValue;
		int ConeArmPower = 0;
		int MGpusherPower;

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
			setMotor(FL, leftpower);
			setMotor(BL, leftpower);

			// set right side motors;
			setMotor(FR, rightpower);
			setMotor(BR, rightpower);


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
			setMotor(MGLift,MGliftPower);

			if(vexRT[Btn5D] == 1){
				if(SensorValue[MG] > 190){
					while (SensorValue[MG] > 600){
						setMotor(MGLift,90);
						wait1Msec(20);
					}
					setMotor(MGLift,0);
				}

				else{
					while (SensorValue[MG] < 250){
						setMotor(MGLift,-50);
						wait1Msec(20);
					}
					setMotor(MGLift,0);

				}


			}

			//MGpusher
			if(vexRT[Btn7L] == 1){
				MGpusherPower = 90;
			}
			else if (vexRT[Btn7R] == 1){
				MGpusherPower = -90;
			}
			else{
				MGpusherPower = 0;
			}
			setMotor(MGpusher,MGpusherPower);

			//Rolly Intake
			if(vexRT[Btn6UXmtr2] == 1){
				setMotor(Roller, 80);
			}

			else if (vexRT[Btn6DXmtr2] == 1){
				setMotor(Roller, -80);
			}
			else{
				setMotor(Roller, 0);
			}


			// Arm
			// ConeArmValue = SensorValue[Arm];
			ConeArmPower = vexRT[Ch3Xmtr2];
			/* if(ConeArmValue < -264 && ConeArmPower > 0){
			ConeArmPower = 0;
			}
			if(ConeArmValue > 0 && ConeArmPower < 0){
			ConeArmPower = 0;

			}
			setMotor(ConeArm,ConeArmPower); */
		}
	}
}
