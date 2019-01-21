#ifndef __Bullet__
#define __Bullet__
#include "common.h"


#define _MAX_SPEED 10
//#define _MAX_SPEED 4
#define _DAMPING 0.98
#define _ACC 3
#define _ROT 15


class Bullet
{

	vec2 bullet_vert[2] = {
		vec2(0.0,0.01),
		vec2(0.0,-0.01)
	};

	vec3 bullet_color[2] = {
		vec3(1.0,1.0,1.0),
		vec3(1.0,1.0,1.0)
	};




	//Record of the bullet's state
	struct {
		vec2 cur_location;//Current position of the center
		float angle;         //Rotation angle
		vec2 pointing;
		mat2 RotateZ2D(const GLfloat theta) {
			GLfloat angle = DegreesToRadians * theta;
			mat2 c;
			c[0][0] = c[1][1] = cos(angle);
			c[1][0] = sin(angle);
			c[0][1] = -c[1][0];
			return c;
		}
		vec2 velocity;       //Velocity
		mat4 M; //Modelview matrix
		bool visible;
		bool canHit = true;
	} state;


	void update_matrix() {
		this->state.M = Translate(this->state.cur_location.x, this->state.cur_location.y, 0) * RotateZ(this->state.angle);
	}



	struct {
		GLuint vao;           //Vertex array object
		GLuint program;       //shader program
		GLuint buffer;        //Vertex buffer objects
		GLuint vertex_shader, fragment_shader;  //Shaders
		GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
		GLint M_location;     //Reference to matrix in shader
	} GLvars;


public:
	Bullet();

	Bullet(vec2 vel, vec2 point);

	Bullet(vec2 vel, vec2 point, vec2 loc);

	~Bullet();

	inline void set_vel_point(vec2 vel, vec2 point){
		this->state.velocity = vel;
		this->state.pointing = point;
	}

	vec2 getCur_loc();






	void update_state();

	void gl_init();

	void draw(mat4 proj);

	inline float clamp(float n, float low, float high) {
		return max(low, min(n, high));
	}
	
	bool getVisibility();

	void setVisbility(bool viz);

	void setLoc(vec2 loca);

	bool getCanHit();

	void setCanHit(bool can);

	void checkLoc();

};


#endif /* defined(__Bullet__) */

