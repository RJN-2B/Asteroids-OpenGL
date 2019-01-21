#pragma once

#ifndef __Asteroids__Asteroid__
#define __Asteroids__Asteroid__

#include "common.h"

static float floatRand(float low, float high)
{
	return ((float(rand()) / float(RAND_MAX)) * (high - low)) + low;
}


class Asteroid
{

	vec2 asteroidBox[4] = {
		Angel::vec2(-0.15,0.2),
		Angel::vec2(0.15,0.2),
		Angel::vec2(0.15,-0.18),
		Angel::vec2(-0.15,-0.18)
	};




	vec2 asteroid_vert[10] = { Angel::vec2(0.0 * 2, 0.0 * 2),
					  Angel::vec2(0.084 * 2, 0.0 * 2),
					  Angel::vec2(0.076 * 2, 0.074 * 2),
					  Angel::vec2(0.03 * 2, 0.092 * 2),
					  Angel::vec2(-0.07 * 2, 0.09 * 2),
					  Angel::vec2(-0.06 * 2, 0.05 * 2),
					  Angel::vec2(-0.07 * 2, -0.075 * 2),
					  Angel::vec2(0.05 * 2, -0.085 * 2),
					  Angel::vec2(0.084 * 2, -0.075 * 2),
					  Angel::vec2(0.05 * 2, 0.0 * 2)
	};








	vec3 asteroid_color[10] = { Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
								 Angel::vec3(0.4, 0.4, 0.4),
		Angel::vec3(0.4, 0.4, 0.4),
		Angel::vec3(0.4, 0.4, 0.4)

	};


	struct {
		float angle;         //Rotation angle
		vec2 cur_location;
		vec2 velocity;       //Velocity
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
		mat4 M;
		int direction;

		bool visible = false;
		int recursive_depth;
		bool original = true;
		bool canSplit = true;
		bool canHurt = true;
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

	vec2 tempBox[4] = {
		Angel::vec2(-0.15,0.2),
		Angel::vec2(0.15,0.2),
		Angel::vec2(0.15,-0.18),
		Angel::vec2(-0.15,-0.18)
	};

public:
	Asteroid();
	~Asteroid();


	void update_state();
	void gl_init();
	void draw(mat4 proj);
	GLfloat * getVertXArrayAst(Asteroid ast);
	GLfloat * getVertYArrayAst(Asteroid ast);

	vec2 * getAstVertLoc(Asteroid ast);

	
	//Added self
	inline int getDirection() {
		return state.direction;
	}

	inline void setDirection(int newDirection) {
		this->state.direction = newDirection;
	}


	void rotateLeft();

	void rotateRight();

	inline float clamp(float n, float low, float high) {
		return max(low, min(n, high));
	}

	void randomize_asteroid();

	vec2 * getOrigLocationAst();

	void shrink(Asteroid orig);

	void set_location(vec2 formerLoc);

	vec2 directLoc();

	bool getVisibility();

	void setVisibility(bool viz);

	void setLocation(vec2 loca);

	vec2 * getAstBox();

	void setRecursiveDepth(int i);

	bool isOriginal();


	void setOriginality(bool orig);

	bool getCanSplit();

	void setCanSplit(bool can);

	bool getCanHurt();

	void setCanHurt(bool can);

	void updateAstBox();

};


#endif /* defined(__Asteroids__Asteroid__) */