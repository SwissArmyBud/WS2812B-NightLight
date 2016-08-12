#include <Arduino.h>
using namespace std;

enum button_state{
	NO_PRESS,
	SHORT_PRESS,
	LONG_PRESS
};

class QuadController{
  public:
    QuadController(){};
    QuadController(byte _swtA, byte _swtB, byte _swtC, byte _swtD){
      
      this->buttons[0] = _swtA;
      this->buttons[1] = _swtB;
      this->buttons[2] = _swtC;
      this->buttons[3] = _swtD;
	  
  	  for(int i=0; i<4; i++){
  		  pinMode(buttons[i], INPUT);
  		  digitalWrite(buttons[i], LOW);
  		  curSwitch[i] = false;
  		  lstSwitch[i] = curSwitch[i];
  		  pressedState[i] = NO_PRESS;
        DBTimeout[i] = 0;
  	  }
      
    }
    
    void update(){

      unsigned long cTime = millis();
  	  for(int i=0; i<4; i++){

        // -----* Update all the debounce and sensing for button states *----- //
  		  curSwitch[i] = digitalRead(buttons[i]); // get the current state of the switch
  		  if(lstSwitch[i] == curSwitch[i]){ // if nothing has changed
    			if(cTime>DBTimeout[i]){ // and the debounce or press timer is clear
    			  pressedState[i] = curSwitch[i]; // the button is now in whatever pressed state the physical switch is
            if(pressedState[i] && ((DBTimeout[i]+500) < cTime)){ // if the button has been pressed for longer than 1/2 of a second
              buttonState[i] = LONG_PRESS; // the button has been long pressed and the status is updated to LONG_PRESS
            }
            else if(!pressedState[i] && (buttonState[i] == LONG_PRESS)){ // if the button is not pressed and was in a LONG_PRESS state
              buttonState[i] = NO_PRESS; // the button state is now back to NO_PRESS status
            }
    			}
  		  }
  		  else{                           // otherwise something has changed
    			lstSwitch[i] = curSwitch[i]; // the watcher for the switch is updated
          if(!curSwitch[i] && ((DBTimeout[i]+75) < cTime)){ // if the switch was pressed for longer than 3/4 of a 1/10th of a second
            buttonState[i] = SHORT_PRESS; // then the button was short pressed and the status is updated
          }
    			DBTimeout[i] = cTime + 10; //the debounce for that switch is also updated, state is left for capture through getButton()
  		  }
       
  	  }
     
    }

    // This allows for a complete state return/poll/reset on each button
    button_state getButton(byte a){
      this->update();
      button_state tmpState = buttonState[a];
      buttonState[a] = NO_PRESS;
      return tmpState;
    }

    // This is a lazy check on the switch level status for each button
    bool getPress(byte a){
      return pressedState[a];
    }
    
  private:
  
    byte  buttons[4];
    bool  lstSwitch[4];
    bool curSwitch[4];
    bool  pressedState[4];
    button_state buttonState[4];
    unsigned long DBTimeout [4];
    
};

