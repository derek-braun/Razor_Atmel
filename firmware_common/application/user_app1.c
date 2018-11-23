/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  /* All discrete LEDs to off */
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(RED);
  LedOff(WHITE);
  
  /* Backlight to white */
  LedOn(LCD_RED);
  LedOn(LCD_GREEN);
  LedOn(LCD_BLUE);
  
  HEARTBEAT_OFF();
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static int BlinkCount = 0;
  static u8 u8Index = 0;
  static u32 u32Counter = 0;
  static u8 u8Letter = 0;
  static u8 u8MorseIndex = 0;
  static bool selectLetter = TRUE;
  static bool inter = FALSE;
  
  static int Morse[26][5] = {{2,1,0,0}, {2,1,1,1,0}, {2,1,2,1,0}, {2,1,1,0,0}, {1,0,0,0,0}, 
  {1,1,2,1,0}, {2,2,1,0,0}, {1,1,1,1,0}, {1,1,0,0,0}, {1,2,2,2,0}, {2,1,2,0,0}, {1,2,1,1,0}, {2,2,0,0,0}, 
  {2,1,0,0,0}, {2,2,2,0,0}, {1,2,2,1,0}, {2,2,1,2,0}, {1,2,1,0,0}, {1,1,1,0,0}, {2,0,0,0,0}, {1,1,2,0,0}, 
  {1,1,1,2,0}, {1,2,2,0,0}, {2,1,1,2,0}, {2,1,2,2,0}, {2,2,1,1,0}};
  
  static char message[] = "HELLOWORLD";
  
  static bool bLightIsOn = FALSE;
  
  /* Increment u32Counter every 1ms cycle */
  u32Counter++;
  
  /* Check and roll over */
  if(u32Counter == COUNTER_LIMIT_MS)
  {
    u32Counter = 0;
    
    if(bLightIsOn)
    {
      HEARTBEAT_OFF();
    }
    else
    {
      HEARTBEAT_ON();
    }
    bLightIsOn = !bLightIsOn;
  }
  if(WasButtonPressed(BUTTON0))
  {
    BlinkCount++;
    if(BlinkCount == 250)
    {
      BlinkCount = 0;
      if(selectLetter == TRUE)
      {
        if(u8Index == strlen(message))
        {
          u8Index = 0;
          LedOff(RED);
          LedOn(GREEN);
        }
        else
        {
          LedOff(GREEN);
          u8Letter = ((int)message[u8Index] - 65);
          u8Index++;
          selectLetter = FALSE;
        }    
      }
      else
      {
        if(inter == FALSE){
          LedOff(BLUE);
          if(Morse[u8Letter][u8MorseIndex] == 2)
          {
            LedOn(RED);
            u8MorseIndex++;
            inter = TRUE;
            BlinkCount = -250;
          }
          else if(Morse[u8Letter][u8MorseIndex] == 1)
          {
            LedOn(RED);
            u8MorseIndex++;
            inter = TRUE;
            BlinkCount = 0;
          }
          else
          {
            LedOff(RED);
            LedOn(BLUE);
            u8MorseIndex = 0;
            BlinkCount = -500;
            inter = FALSE;
            selectLetter = TRUE;
          }
        }
        else
        {
          LedOff(RED);
          inter = FALSE;
          BlinkCount = 0;
        }
      }
    }
  }
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
