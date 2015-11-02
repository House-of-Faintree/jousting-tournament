/*By default set menu_ref_1 to MANUAL, menu_ref_2 to MAX SPEED and RUN=0*/
intialistion();
welcome();
LCD_disp(menu_ref_1, menu_ref_2);

Main loop:
  if (RUN==0) GOTO Menu_mode;
  else GOTO Motor_On_mode;

/*User pushes left/right on joystick to decrement/increment a value and up/down
to scroll through submenus*/

Menu_mode:
  if (Menu button has been pressed)
  {
  Next menu_ref_1;
  clear menu button flag;
  }

  if (menu_ref_1==MANUAL)
  {
    if (joystick pushed LEFT/RIGHT && values[menu_ref_2]>0)
    {
        values[menu_ref_2]=values[menu_ref_2]-/+5;
    }
    LCD_disp(menu_ref_1, menu_ref_2);  //Update LCD
    delay;                             //Delay to prevent flickering
  }

  else if (menu_ref_1==FACTORY)
  {
    if (joystick pushed LEFT/RIGHT && values[menu_ref_2]>0)
    {
        values[menu_ref_2]=values[menu_ref_2]-/+5;
    }
    else if (joystick pushed UP/DOWN)
    {
      Next/Previous submenu;             //menu_ref_2 ++/--
    }
    LCD_disp(menu_ref_1, menu_ref_2);
    delay;
  }
  else
  {
    display runtime mode only;            //Assisted and Auto have no user
                                          //inputs in Menu
  }

  if (Motor button has been pressed since start of loop)
  {
  RUN=1;
  clear motor button flag;
  }
  GOTO Main Loop;

Motor_On_mode:
  //Simply run in chosen run-time mode until motor button is pressed again
  motor_on(menu_ref_1);
  if (Motor button has been pressed since start of loop)
  {
  RUN=0;
  clear motor button flag;
  }
  GOTO Main Loop
