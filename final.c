//Paul Lee and Kevin Shin
//plee4 and kshin1
//Adventure Rooms

#include <stdio.h>
#include <math.h>
#include "gfx4.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 800

typedef struct hero{
	double width;		// width of hero
	double height;		// height of hero
	double xc;		// top left corner of hero (x value)
	double yc;		// top left corner of hero (y value)
	int herostatus;		// armour
	int lives;		// number of hits available
}hero;

hero p1;

void makehero();
void drawhero();
void drawroom(int);
void movehero(int);
int boulderball(double, double, int ballcoord[2]);
void item();
int blocks(double, double, int ballcoord[2]);
void shoot(double ybullet, int ballcoord[2]);
int drawrotate();



int room = 1;			// starting room number
double i = 0;			// initializes angle
int win = 0;			// win???

int main(){
	int xball = 200, xball2 = 400, xball3 = 600, yball = 400, yball2 = 600, yball3 = 700, dead1, dead2, dead3, dead4; // initializing variables
	int xblock = 400, yblock = 300, xblock2 = 400, yblock2 = 500, xblock3 = 400,  yblock3 = 700; // initializing variables
	int ballcoord1[2] = {5, 3}; // initializing arrays for speeds
	int ballcoord2[2] = {6, 3}; // initializing arrays for speeds
	int ballcoord3[2] = {3, 4}; // initializing arrays for speeds
	double deltat = 10000;		// used in usleep later in the function
	double speed = .01;	// initializing the speed at which the final obstacle rotates
	char c;
	makehero();		// calls makehero function
	gfx_open(WIDTH, HEIGHT, "Adventure Rooms");		// creates 800 by 800 window
	gfx_clear();
	while(1){
		gfx_clear();
		usleep(deltat);
		drawroom(room);	// calls drawroom function
		drawhero();	// calls drawhero function
		if(gfx_event_waiting() == 1){			// moves hero based on what key is pressed
			c = gfx_wait();
			if ( c == 'w'){
				movehero(1);
			}
			if ( c == 'a'){
				movehero(2);
			}
			if ( c == 's'){
				movehero(3);
			}
			if ( c == 'd'){
				movehero(4);
			}
			if (c == 'q'){
				break;
			}
		}
		if(room == 2){					// animates bouncing balls in room 2
			xball = xball + ballcoord1[0];
			yball = yball + ballcoord1[1];
			xball2 = xball2 + ballcoord2[0];
			yball2 = yball2 + ballcoord2[1];
			xball3 = xball3 + ballcoord3[0];
			yball3 = yball3 + ballcoord3[1];
			gfx_color(96,96,96);			// ball one
			dead1 = boulderball(xball, yball, ballcoord1);		// collision detection
			gfx_color(100,0,0);			// ball two
			dead2 = boulderball(xball2, yball2, ballcoord2);	// collision detection
			gfx_color(123,132,12);			// ball three
			dead3 = boulderball(xball3, yball3, ballcoord3);	// collision detection
			if(dead1 == 0 || dead2 == 0 || dead3 == 0){	// if collision is detected, program will quit
				return 0;
			} 
		}
		if(room == 4){					// animates blocks moving side to side in room 4
			xblock = xblock + ballcoord1[0];
			xblock2 = xblock2 + ballcoord2[0];
			xblock3 = xblock3 + ballcoord3[0];
			gfx_color(76,89,224);			// block 1
			dead1 = blocks(xblock, yblock, ballcoord1);	// collision detection
			gfx_color(142,241,179);			// block 2
			dead2 = blocks(xblock2, yblock2, ballcoord2);	// collision detection
			gfx_color(172,47,63);			// block 3
			dead3 = blocks(xblock3, yblock3, ballcoord3);	// collision detection
			if(dead1 == 0 || dead2 == 0 || dead3 == 0){		// checks if any collisions has occurred
				p1.lives -= 1;					// takes away armour if hero is hit and possesses armour
				if(p1.lives == 1){				// resets position of hero if hit with one life left
					p1.herostatus = 0;
					p1.xc = 390;
					p1.yc = 0;
				}
				else if(p1.lives == 0){				// if no armour is detected and player has no lives, quit program
					return 0;
				}
			}
		}
		if(room == 5){					// animates rotating balls
			gfx_color(42,179,69);
			dead4 = drawrotate();			// collision detection
			i += speed;				// increments the angle
			if(dead4 == 0){				// checks if any collisions has occurred
				p1.lives -= 1;			// takes away armour if hero is hit and possess armour
				if(p1.lives == 1){		// resets position of hero if hit with one life left
					p1.herostatus = 0;	
					p1.xc = 390;
					p1.yc = 0;
				}		
				else if(p1.lives == 0){		// if no armour is detected and player has no lives, quit program
					return 0;
				}
			}
		}
		if(win == 1){					// if win, then quit program
			return 0;
		}
				
		gfx_flush();
	}
}

void drawroom(int room){					// draws each room depending on what room value is brought into the function
	gfx_color(51,25,0);
	if (room == 1){
		gfx_fill_rectangle(0,0,20,800);		//Left Border
		gfx_fill_rectangle(780,0,20,800);	//Right Border
		gfx_fill_rectangle(0,0,800,20);		//Top Border
		gfx_fill_rectangle(0,780,350,20);	//Bottomleft Border
		gfx_fill_rectangle(450,780,350,20);	//Bottomright border
		
		// create text giving directions for player
		gfx_color(255, 255, 255);
		char str[99];
		sprintf(str, "To Move: W-Up, A-Left, S-Down, D-Right");
		gfx_text(WIDTH/3, HEIGHT/3, str);
		char str2[99];
		sprintf(str2, "Please nagivate through each room... Carefully");
		gfx_text(WIDTH/3, (HEIGHT/3)+20, str2);
	}
	if(room == 2){
		gfx_color(255, 255, 255);
		gfx_fill_rectangle(0,0,20,800);		//Left Border
		gfx_fill_rectangle(780,0,20,800);	//Right Border
		gfx_fill_rectangle(0,0,350,20);		//Topleft Border
		gfx_fill_rectangle(450,0,350,20);	//Topright Border
		gfx_fill_rectangle(0,780,350,20);	//Bottomleft Border	
		gfx_fill_rectangle(450,780,350,20);	//Bottomright border
	}
	if(room == 3){
		gfx_color(55, 155, 255);
		gfx_fill_rectangle(0,0,20,800);		//Left Border
		gfx_fill_rectangle(780,0,20,800);	//Right Border
		gfx_fill_rectangle(0,0,350,20);		//Topleft Border
		gfx_fill_rectangle(450,0,350,20);	//Topright Border
		gfx_fill_rectangle(0,780,350,20);	//Bottomleft Border	
		gfx_fill_rectangle(450,780,350,20);	//Bottomright border
		
		//create text to indicate armour for player is available
		char str3[99];
		sprintf(str3, "Pick up the armour! You'll need it");
		gfx_text(WIDTH/3, (HEIGHT/3)+20, str3);
		
		item(); // calls item function
	}
	if(room == 4){
		gfx_color(0, 100, 105);
		gfx_fill_rectangle(0,0,20,800);		//Left Border
		gfx_fill_rectangle(780,0,20,800);	//Right Border
		gfx_fill_rectangle(0,0,350,20);		//Topleft Border
		gfx_fill_rectangle(450,0,350,20);	//Topright Border
		gfx_fill_rectangle(0,780,350,20);	//Bottomleft Border	
		gfx_fill_rectangle(450,780,350,20);	//Bottomright border		
	}
	if(room == 5){
		gfx_color(242, 100, 105);
		gfx_fill_rectangle(0,0,20,800);		//Left Border
		gfx_fill_rectangle(780,0,20,800);	//Right Border
		gfx_fill_rectangle(0,0,350,20);		//Topleft Border
		gfx_fill_rectangle(450,0,350,20);	//Topright Border
		gfx_fill_rectangle(0,780,350,20);	//Bottomleft Border	
		gfx_fill_rectangle(450,780,350,20);	//Bottomright border
	}
	if(room == 6){
		gfx_color(rand()%255,rand()%255,rand()%255);	// randomizes color for the borders
		gfx_fill_rectangle(0,0,20,800);		//Left Border
		gfx_fill_rectangle(780,0,20,800);	//Right Border
		gfx_fill_rectangle(0,0,350,20);		//Topleft Border
		gfx_fill_rectangle(450,0,350,20);	//Topright Border
		gfx_fill_rectangle(0,780,800,20);	//BottomBorder
		
		// creates a text congratulating player and have the player exit through the door to end program
		char str4[99];
		sprintf(str4, "Congratulations! You have beaten Aventure Rooms. Please exit the last room through the door");
		gfx_text(WIDTH/5, (HEIGHT/3)+20, str4);
		
		// exit door
		gfx_color(51,51,0);
		gfx_fill_rectangle(390,390,20,40);
		gfx_color(32,32,32);
		gfx_fill_rectangle(395,395,10,30);
	}
}

void makehero(){		// defines hero struct
	p1.width = 20;
	p1.height = 40;	
	p1.xc = (WIDTH/2)-10;
	p1.yc = 50;
	p1.herostatus = 0;
	p1.lives = 1;
}


void drawhero(){		// draws the hero
	if (p1.herostatus == 0){	// if hero has no armour, hero is blue
		gfx_color(0, 0, 204);
	}
	if (p1.herostatus == 1){	// if hero has armour, hero is green
		gfx_color(0,153,0);
	}
	gfx_fill_rectangle(p1.xc, p1.yc, p1.width, p1.height);
}

void movehero(int x){		// depending on what button is pressed, moves hero in a direction
	switch (x){
		case 1:		// w is pressed
			if(p1.yc != 20){	// checks boundaries of room
				p1.yc -= 10;
			}	
			break;
		case 2:		// a is pressed
			if(p1.xc != 20){	// checks boundaries of room
				p1.xc -= 10;
			}	
			break;
		case 3:		// s is pressed
			if(p1.yc != 740 || (p1.xc >= 350 && p1.xc <= 430)){	// checks boundaries of room
				p1.yc += 10;
			}	
			
			break;
		case 4:		// d is pressed
			if(p1.xc != 760){	// checks boundaries of room
				p1.xc += 10;
			}	
			break;
	
	}
	if(p1.yc > 780 && p1.xc >= 350 && p1.xc <= 430){	// if hero gets to the end of the door/room, resets position of hero to top of next room
		p1.xc = 390;
		p1.yc = 0;
		room++;
	}
	if(room == 6 && p1.xc == 390 && p1.yc == 390){		// if hero gets on door in room 6, win = 1
		win = 1;
	}	
	
}

int boulderball(double xball, double yball, int ballcoord[2]){		// collision checks and boundary checks for balls in room 2
		
		gfx_fill_circle(xball, yball, 50);
		
		// checks collision with walls
		if (xball+50 > 780){
			ballcoord[0] = -ballcoord[0];
			gfx_fill_circle(730,yball,50);
		}
		if (xball-50 < 20){
			ballcoord[0] = -ballcoord[0];
			gfx_fill_circle(70,yball,50);
		}
		if (yball+50 > 780){
			ballcoord[1] = -ballcoord[1];
			gfx_fill_circle(xball,730,50);
		}
		if (yball-50 < 20){
			ballcoord[1] = -ballcoord[1];
			gfx_fill_circle(xball,70,50);
		}
		
		// checks collision with hero
		if (xball - 50 < p1.xc + 20 && xball - 50 > p1.xc && yball > p1.yc && yball < p1.yc + 40){
			return 0;
		}
		if (xball + 50 > p1.xc && xball + 50 < p1.xc + 20 && yball > p1.yc && yball < p1.yc + 40){
			return 0;
		} 
		if (yball + 50 > p1.yc && yball + 50 < p1.yc + 40 && xball > p1.xc && xball < p1.xc + 20){
			return 0;
		}
		if (yball - 50 < p1.yc + 40 && yball - 50 > p1.yc && xball > p1.xc && xball < p1.xc + 20){
			return 0;
		}
		
		return 1;
}

void item(){				// item in room 3 (the armour)
	if(p1.herostatus == 0){		// item is yellow if not picked up
		gfx_color(255,255,0);
	}
	if(p1.herostatus == 1){		// item is painted black to show that it's picked up
		gfx_color(0,0,0);
	}
	if(p1.xc == 390 && p1.yc == 380){	// when hero walk over the item, give hero armour
		p1.herostatus = 1;
		p1.lives += 1;
		gfx_color(0,0,0);
	}
	gfx_fill_rectangle(390,390,20,20);	// draw item
}
	
int blocks(double xblock, double yblock, int ballcoord[2]){	// draw blocks and detects collision
	gfx_fill_rectangle(xblock, yblock, 200, 20);	// draw blocks
	
	// checks collision with side walls
	if (xblock > 590){
		ballcoord[0] = -ballcoord[0];
		gfx_fill_rectangle(590,yblock,200,20);
		}
	if (xblock < 20){
		ballcoord[0] = -ballcoord[0];
		gfx_fill_rectangle(20,yblock,200,20);
	}
	
	// checks collision with hero
	if (xblock < p1.xc + 20 && xblock > p1.xc && yblock > p1.yc && yblock < p1.yc + 40){
		return 0;
	}
	if (xblock + 200 > p1.xc && xblock + 200 < p1.xc + 20 && yblock > p1.yc && yblock + 20 < p1.yc + 40){
		return 0;
	}
	
	if (yblock < p1.yc + 40 && yblock > p1.yc && xblock < p1.xc && xblock + 200 > p1.xc + 20){
		return 0;
	}
	if (yblock + 20 > p1.yc && yblock +20 < p1.yc + 40 && xblock < p1.xc && xblock + 200 > p1.xc + 20){
		return 0;
	}
		
}

int drawrotate(){		// draws circles for room 5 and rotate them in a wheel-like fashion and also checks for collision
	int x;
	double rotatex, rotatey;
	for(x = 25; x < 375; x += 25){
		rotatex = x*cos(i) + 400;
		rotatey = x*sin(i) + 400;
		gfx_fill_circle(rotatex,rotatey,10);		// draws circles
		
		// checks collision for each circle
		if (rotatex - 10 < p1.xc + 20 && rotatex - 10 > p1.xc && rotatey > p1.yc && rotatey < p1.yc + 40){
			return 0;
		}
		if (rotatex + 10 > p1.xc && rotatex + 10 < p1.xc + 20 && rotatey > p1.yc && rotatey < p1.yc + 40){
			return 0;
		} 
		if (rotatey + 10 > p1.yc && rotatey + 10 < p1.yc + 40 && rotatex > p1.xc && rotatex < p1.xc + 20){
			return 0;
		}
		if (rotatey - 10 < p1.yc + 40 && rotatey - 10 > p1.yc && rotatex > p1.xc && rotatex < p1.xc + 20){
			return 0;
		}
	}
	for(x = 25; x < 375; x += 25){
		rotatex = x*cos(i+3.14) + 400;
		rotatey = x*sin(i+3.14) + 400;
		gfx_fill_circle(rotatex,rotatey,10);		// draws circles
		
		// checks collision for each circle
		if (rotatex - 10 < p1.xc + 20 && rotatex - 10 > p1.xc && rotatey > p1.yc && rotatey < p1.yc + 40){
			return 0;
		}
		if (rotatex +10 > p1.xc && rotatex + 10 < p1.xc + 20 && rotatey > p1.yc && rotatey < p1.yc + 40){
			return 0;
		} 
		if (rotatey + 10 > p1.yc && rotatey + 10 < p1.yc + 40 && rotatex > p1.xc && rotatex < p1.xc + 20){
			return 0;
		}
		if (rotatey - 10 < p1.yc + 40 && rotatey - 10 > p1.yc && rotatex > p1.xc && rotatex < p1.xc + 20){
			return 0;
		}
	}
}
	



