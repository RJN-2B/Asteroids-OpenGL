//
//  Ship.h
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#ifndef __Asteroids__Ship__
#define __Asteroids__Ship__

#include "common.h"
#include <vector>
#include "Bullet.h"
#include "Asteroid.h"

#define _MAX_SPEED 10
//#define _MAX_SPEED 4
#define _DAMPING 0.98
#define _ACC 3
#define _ROT 15

class Ship {


	

	std::vector < Bullet * > bullets;



	int bullet_count = 0;
	vec2 ship_box[4] = {
		vec2(0.07, -0.02),
		vec2(-0.07, -0.02),
		vec2(-0.07, 0.12),
		vec2(0.07, 0.12) };

	vec2 ship_vert[8] = {  //make ships center 0,0
								Angel::vec2(0,10.0 * 0.01),
								Angel::vec2((5.0) * 0.01, (5.0 / 2.0) * 0.01),
								Angel::vec2(0,0),
								Angel::vec2(-5.0 * 0.01,(5.0 / 2.0) * 0.01),
								Angel::vec2(0,10.0 * 0.01) };





	vec3 ship_color[8] = {		 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0),
								 Angel::vec3(1.0, 1.0, 1.0) };

	vec2 ship_flames[3] = { Angel::vec2(0.05 / 2, 0.1/2),
							Angel::vec2(0.0 / 2, 0.05 / 2),//,
							Angel::vec2(-0.05/2, 0.1/2) };

	vec3 ship_flames_color[3] = { 
								Angel::vec3(1.0, 0.0, 1.0),
								Angel::vec3(1.0, 0.0, 1.0),
							    Angel::vec3(1.0, 0.0, 1.0)};

	

	//Record of the ship's state
	struct {
		vec2 cur_location;// = vec2{ 0.0,0.0 };   //Current position of the center
		float angle;         //Rotation angle
		vec2 pointing;       //Vector pointing to the front of the ship
		//This function will be helpful to keep track of the direction the ship
		//is pointing
		mat2 RotateZ2D(const GLfloat theta) {
			GLfloat angle = DegreesToRadians * theta;
			mat2 c;
			c[0][0] = c[1][1] = cos(angle);
			c[1][0] = sin(angle);
			c[0][1] = -c[1][0];
			return c;
		}
		vec2 velocity;       //Velocity
		bool thruster_on;    //Boolean if a thruster is on
		mat4 M; //Modelview matrix

	} state;

	void update_matrix(vec2 temp) {
		if (temp != vec2(0.0, 0.0)) {
			this->state.M = Translate(this->state.cur_location.x, this->state.cur_location.y, 0) * (RotateZ(this->state.angle));
		}
		else {
			this->state.M = Translate(this->state.cur_location.x, this->state.cur_location.y, 0) * ((Translate(vec3(temp,0.0)) * RotateZ(this->state.angle) * Translate(0,0,0)));
		}
	}
	void rotate_matrix() {
		this->state.M = Translate(-this->state.cur_location.x, -this->state.cur_location.y, 0) * RotateZ(this->state.angle) * Translate(this->state.cur_location.x, this->state.cur_location.y, 0);
	}

	//OpenGL variables for a ship
	struct {
		GLuint vao;           //Vertex array object
		GLuint program;       //shader program
		GLuint buffer;        //Vertex buffer objects
		GLuint vertex_shader, fragment_shader;  //Shaders
		GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
		GLint M_location;     //Reference to matrix in shader

		//For Flames
		GLuint vao_flames;
		GLuint program_flames;
		GLuint buffer_flames;
		GLuint vertex_shader_flames, fragment_shader_flames;
		GLint vpos_location_flames, vcolor_location_flames;
		GLint M_location_flames;
	} GLvars;





public:


	Ship();

	inline void start_thruster() { 
		state.thruster_on = true;
	}
	inline void stop_thruster() { state.thruster_on = false; }


	inline void rotateLeft() {
		//Do something






		this->state.angle -= _ROT;
		if (this->state.angle < 0.0) {
			this->state.angle += 360.0;
		}

		GLfloat radians = (this->state.angle * M_PI) / 180;

		this->state.pointing = newPointing(this->state.pointing, (-15.0 * M_PI) / 180.0);

		vec2 temp = this->state.cur_location;
		this->update_matrix(temp);

	}
	inline void rotateRight() {
		//Do something
		this->state.angle += _ROT;
		if (this->state.angle > 360.0) {
			this->state.angle -= 360.0;
		}

		GLfloat radians = (this->state.angle * M_PI) / 180;

		//this->state.pointing = newPointing(this->state.pointing, radians);
		this->state.pointing = newPointing(this->state.pointing, (15.0 * M_PI) / 180.0);


		vec2 temp = this->state.cur_location;
		this->update_matrix(temp);
		//this->rotate_matrix();


	}

	Angel::vec2 newPointing(vec2 in, float rad) {
		Angel::vec2 temp;
		GLfloat x, y;
		x = in[0];
		y = in[1];

		temp[0] = roundf((x * cos(rad) - y * sin(rad)) * 1000) / 1000;
		temp[1] = roundf((x * sin(rad) + y * cos(rad)) * 1000) / 1000;
		return temp;
	}

	void update_state();

	void gl_init();

	void draw(mat4 proj);

	inline float clamp(float n, float low, float high) {
		return max(low, min(n, high));
	}

	void pew_pew();

	std::vector < Bullet * > get_bullets_vector();

	vec2 * getShipVertLoc(Ship ship);

	GLfloat * getShipYArray(Ship ship);

	GLfloat * getShipXArray(Ship ship);

	void kill_bullet(int index);

	vec2 * getShipVertLoc();

	vec2 getLoc();

	vec2 * getShipBox();












};








#endif /* defined(__Asteroids__Ship__) */
