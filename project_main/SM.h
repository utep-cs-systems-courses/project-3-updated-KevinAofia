#ifndef stateMachine_included
#define stateMachine_included
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//No longer using these variables, keeping them here for referencing
//but they are now defined inside the assembly file and used as static short variables
//void siren();                   //method for siren noises
//extern char siren_state = 1;    //for our siren() function
//extern int frequency = 700;     //for our siren() function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void blinkbuzz();    

void state_advance();
void off();          //0%brightness
void d10();          //10%brightness
void d20();          //20%brightness
void d30();          //30%brightness
void d40();          //40%brightness
void d50();          //50%brightness
void d60();          //60%brightness
void d70();          //70%brightness
void d80();          //80%brightness
void d90();          //90%brightness
void on();           //100%brightness

extern char blinkbuzz_state;    //for our blinkbuzz() function

extern char dim_state;          //for our dim state intensity
extern char start;              //for our dim intesity state

#endif // included
