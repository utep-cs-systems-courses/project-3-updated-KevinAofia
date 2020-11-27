#ifndef stateMachine_included
#define stateMachine_included

void turn_on_red();
void turn_on_green();
void turn_on_all();
void blinkbuzz();
void siren();
void dim();
void dim25();
void dim50();
void dim75();

extern char blinkbuzz_state; //for our blinkbuzz() function

extern char siren_state; //for our siren() function
extern int frequency; //for our siren() function

extern int dim_state;
extern int dim_state2;

#endif // included
