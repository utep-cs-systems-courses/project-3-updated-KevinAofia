#ifndef aofia_lab3_included
#define aofia_lab3_included

void update_switches();

void draw_solid_diamond(int col,int row, u_int color);
void draw_multi_diamond(int col,int row, u_int color,u_int color2,u_int color3,u_int color4);

void draw_stick_figure();

void G_ON();
void R_ON();
void GR_ON();

void G_OFF();
void R_OFF();
void GR_OFF();

extern char active_switches[5]; //four switches with a terminating character

#endif
