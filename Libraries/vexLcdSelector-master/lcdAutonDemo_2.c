/*-----------------------------------------------------------------------------*/
/*  LCD autonomous demo 2                                                      */
/*  Copyright (c) 2013 James Pearman                                           */
/*                                                                             */
/*  Licensed under the Apache License, Version 2.0 (the "License");            */
/*  you may not use this file except in compliance with the License.           */
/*  You may obtain a copy of the License at                                    */
/*                                                                             */
/*      http://www.apache.org/licenses/LICENSE-2.0                             */
/*                                                                             */
/*  Unless required by applicable law or agreed to in writing, software        */
/*  distributed under the License is distributed on an "AS IS" BASIS,          */
/*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */
/*  See the License for the specific language governing permissions and        */
/*  limitations under the License.                                             */
/*                                                                             */
/*  The author can be contacted on the vex forums as jpearman                  */
/*  or electronic mail using jbpearman_at_mac_dot_com                          */
/*  Mentor for team 8888 RoboLancers, Pasadena CA.                             */
/*-----------------------------------------------------------------------------*/

//Competition Control and Duration Settings
#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

// Include the lcd button get utility function
#include "getlcdbuttons.c"

// global hold the auton selection
static int MyAutonomous = 0;

/*-----------------------------------------------------------------------------*/
/*  Display autonomous selection                                               */
/*-----------------------------------------------------------------------------*/

// max number of auton choices
#define MAX_CHOICE  3

void
LcdAutonomousSet( int value, bool select = false )
{
    // Cleat the lcd
    clearLCDLine(0);
    clearLCDLine(1);

    // Display the selection arrows
    displayLCDString(1,  0, l_arr_str);
    displayLCDString(1, 13, r_arr_str);

    // Save autonomous mode for later if selected
    if(select)
        MyAutonomous = value;

    // If this choice is selected then display ACTIVE
    if( MyAutonomous == value )
        displayLCDString(1, 5, "ACTIVE");
    else
        displayLCDString(1, 5, "select");

    // Show the autonomous names
    switch(value) {
        case    0:
            displayLCDString(0, 0, "auton 0");
            break;
        case    1:
            displayLCDString(0, 0, "auton 1");
            break;
        case    2:
            displayLCDString(0, 0, "auton 2");
            break;
        case    3:
            displayLCDString(0, 0, "auton 3");
            break;
        default:
            displayLCDString(0, 0, "Unknown");
            break;
        }
}

/*-----------------------------------------------------------------------------*/
/*  Rotate through a number of choices and use center button to select         */
/*-----------------------------------------------------------------------------*/

void
LcdAutonomousSelection()
{
    TControllerButtons  button;
    int  choice = 0;

    // Turn on backlight
    bLCDBacklight = true;

    // diaplay default choice
    LcdAutonomousSet(0);

    while( bIfiRobotDisabled )
        {
        // this function blocks until button is pressed
        button = getLcdButtons();

        // Display and select the autonomous routine
        if( ( button == kButtonLeft ) || ( button == kButtonRight ) ) {
            // previous choice
            if( button == kButtonLeft )
                if( --choice < 0 ) choice = MAX_CHOICE;
            // next choice
            if( button == kButtonRight )
                if( ++choice > MAX_CHOICE ) choice = 0;
            LcdAutonomousSet(choice);
            }

        // Select this choice
        if( button == kButtonCenter )
            LcdAutonomousSet(choice, true );

        // Don't hog the cpu !
        wait1Msec(10);
        }
}

void pre_auton()
{
    bStopTasksBetweenModes = true;

    LcdAutonomousSelection();
}

task autonomous()
{
    switch( MyAutonomous ) {
        case    0:
            // run auton code
            break;

        case    1:
            // run some other auton code
            break;

        default:
            break;
        }
}

task usercontrol()
{
    while (true) {
        wait1Msec(10);
        }
}
