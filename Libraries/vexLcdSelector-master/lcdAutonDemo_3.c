/*-----------------------------------------------------------------------------*/
/*  LCD autonomous demo 3                                                      */
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

/*-----------------------------------------------------------------------------*/
/*  Definition of the menus and global variables for the autonomous selection  */
/*-----------------------------------------------------------------------------*/

typedef enum vexAlliance {
    kAllianceBlue = 0,
    kAllianceRed
} vexAlliance;

typedef enum vexStartposition {
    kStartHanging = 0,
    kStartMiddle
} vexStartposition;

typedef enum vexLcdMenus {
    kMenuStart    = 0,

    kMenuAlliance = 0,
    kMenuStartpos,
    kMenuAutonSelect,

    kMenuMax
} vexLcdMenus;

static  vexAlliance         vAlliance = kAllianceBlue;
static  vexStartposition    vPosition = kStartHanging;
static  short               vAuton = 0;

/*-----------------------------------------------------------------------------*/
/*    Display menus and selections                                             */
/*-----------------------------------------------------------------------------*/

void
LcdAutonomousDisplay( vexLcdMenus menu )
{
    // Cleat the lcd
    clearLCDLine(0);
    clearLCDLine(1);

    // Display the selection arrows
    displayLCDString(1,  0, l_arr_str);
    displayLCDString(1, 13, r_arr_str);
    displayLCDString(1,  5, "CHANGE");

    // Show the autonomous names
    switch( menu ) {
        case    kMenuAlliance:
            if( vAlliance == kAllianceBlue )
                displayLCDString(0, 0, "Alliance - BLUE");
            else
                displayLCDString(0, 0, "Alliance - RED");
            break;
        case    kMenuStartpos:
            if( vPosition == kStartHanging )
                displayLCDString(0, 0, "Start - Hanging");
            else
                displayLCDString(0, 0, "Start - Middle");
            break;
        case    kMenuAutonSelect:
            switch( vAuton ) {
                case    0:
                    displayLCDString(0, 0, "Default");
                    break;
                case    1:
                    displayLCDString(0, 0, "Special 1");
                    break;
                default:
                    char    str[20];
                    sprintf(str,"Undefined %d", vAuton );
                    displayLCDString(0, 0, str);
                    break;
                }
            break;

        default:
            displayLCDString(0, 0, "Unknown");
            break;
        }
}



/*-----------------------------------------------------------------------------*/
/*  Rotate through a number of menus and use center button to select choices   */
/*-----------------------------------------------------------------------------*/

void
LcdAutonomousSelection()
{
    TControllerButtons  button;
    vexLcdMenus  menu = kMenuStart;

    // Turn on backlight
    bLCDBacklight = true;

    // diaplay default choice
    LcdAutonomousDisplay(0);

    while( bIfiRobotDisabled )
        {
        // this function blocks until button is pressed
        button = getLcdButtons();

        // Display and select the autonomous routine
        if( ( button == kButtonLeft ) || ( button == kButtonRight ) ) {
            // previous choice
            if( button == kButtonLeft )
                if( --menu < kMenuStart ) menu = kMenuMax-1;
            // next choice
            if( button == kButtonRight )
                if( ++menu >= kMenuMax ) menu = kMenuStart;
            }

        // Select this choice for the menu
        if( button == kButtonCenter )
            {
            switch( menu ) {
                case    kMenuAlliance:
                    // alliance color
                    vAlliance = (vAlliance == kAllianceBlue) ? kAllianceRed : kAllianceBlue;
                    break;
                case    kMenuStartpos:
                    // start position
                    vPosition = (vPosition == kStartHanging) ? kStartMiddle : kStartHanging;
                    break;
                case    kMenuAutonSelect:
                    // specific autonomous routine for this position
                    if( ++vAuton == 3 )
                        vAuton = 0;
                    break;
                }

            }

        // redisplay
        LcdAutonomousDisplay(menu);

        // Don't hog the cpu !
        wait1Msec(10);
        }
}

void pre_auton()
{
    bStopTasksBetweenModes = true;

    LcdAutonomousSelection();
}

void
autonomousRed()
{
    // code is the same as the blue code
}

void
autonomousBlue()
{
    if( vPosition == kStartHanging ) {
        switch( vAuton ) {
            case    0:
                // run some autonomous code
                break;
            case    1:
                // run some other autonomous code
                break;
            default:
                break;
        }
    }
    else { // middle zone
        switch( vAuton ) {
            case    0:
                // run some autonomous code
                break;
            case    1:
                // run some other autonomous code
                break;
            default:
                break;
        }
    }
}

task autonomous()
{
    if( vAlliance == kAllianceBlue )
        autonomousBlue();
    else
        autonomousRed();
}

task usercontrol()
{
    while (true) {
        wait1Msec(10);
        }
}
