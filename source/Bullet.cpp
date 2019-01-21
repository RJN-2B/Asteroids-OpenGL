#include "common.h"
#include "Bullet.h"




Bullet::Bullet()
{
}

Bullet::Bullet(vec2 vel, vec2 point) {
	this->state.velocity = vel;
	this->state.pointing = point;




}

Bullet::Bullet(vec2 vel, vec2 point, vec2 loc)
{
	this->state.velocity = vel;
	this->state.pointing = point;
	this->state.cur_location = loc;


}


Bullet::~Bullet()
{

}


void Bullet::gl_init() {
	//Ship

	std::string vshader = shader_path + "vshader_Ship.glsl";
	std::string fshader = shader_path + "fshader_Ship.glsl";

	GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
	GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

	GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(GLvars.vertex_shader, 1, (const GLchar**)&vertex_shader_source, NULL);
	glCompileShader(GLvars.vertex_shader);
	check_shader_compilation(vshader, GLvars.vertex_shader);

	GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(GLvars.fragment_shader, 1, (const GLchar**)&fragment_shader_source, NULL);
	glCompileShader(GLvars.fragment_shader);
	check_shader_compilation(fshader, GLvars.fragment_shader);

	GLvars.program = glCreateProgram();
	glAttachShader(GLvars.program, GLvars.vertex_shader);
	glAttachShader(GLvars.program, GLvars.fragment_shader);

	glLinkProgram(GLvars.program);
	check_program_link(GLvars.program);

	glBindFragDataLocation(GLvars.program, 0, "fragColor");

	GLvars.vpos_location = glGetAttribLocation(GLvars.program, "vPos");
	GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vColor");
	GLvars.M_location = glGetUniformLocation(GLvars.program, "M");

	// Create a vertex array object
	glGenVertexArrays(1, &GLvars.vao);
	//Set GL state to use vertex array object
	glBindVertexArray(GLvars.vao);

	//Generate buffer to hold our vertex data
	glGenBuffers(1, &GLvars.buffer);

	//Set GL state to use this buffer
	glBindBuffer(GL_ARRAY_BUFFER, GLvars.buffer);

	//Create GPU buffer to hold vertices and color
	glBufferData(GL_ARRAY_BUFFER, sizeof(bullet_vert) + sizeof(bullet_color), NULL, GL_STATIC_DRAW);

	//First part of array holds vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bullet_vert), bullet_vert);
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bullet_vert), sizeof(bullet_vert), bullet_color);


	glEnableVertexAttribArray(GLvars.vpos_location);
	glEnableVertexAttribArray(GLvars.vcolor_location);



	glVertexAttribPointer(GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(bullet_vert)));

	glBindVertexArray(0);


}



void Bullet::update_state() {
	// Things to do:
	//a = F
	//Force is in the direction the ship is pointing
	GLfloat dt = 0.033; //fixed timestep based on 30 fps
	vec2 force = this->state.pointing;
	vec2 accel = force * _ACC;
	//Clamp acceleration at some maximum value


	accel[0] = clamp(accel[0], -10.0, _MAX_SPEED);
	accel[1] = clamp(accel[1], -10.0, _MAX_SPEED);

	this->state.velocity = this->state.velocity + accel * dt;

	this->state.velocity[0] = clamp(this->state.velocity[0], -10.0, _MAX_SPEED);
	this->state.velocity[1] = clamp(this->state.velocity[1], -10.0, _MAX_SPEED);
	this->state.velocity *= _DAMPING;


	//p  = old_position + v*dt
	this->state.cur_location = this->state.cur_location + this->state.velocity * dt;


	if (this->state.cur_location[0] > 1) {
		//this->state.cur_location[0] = -2.0 + this->state.cur_location[0];
		//delete this;
	}

	if (this->state.cur_location[0] < -1) {
		//this->state.cur_location[0] = 2.0 + this->state.cur_location[0];
		//delete this;
	}

	if (this->state.cur_location[1] > 1) {
		//this->state.cur_location[1] = -2.0 + this->state.cur_location[1];
		//delete this;
	}

	if (this->state.cur_location[1] < -1) {
		//this->state.cur_location[1] = 2.0 + this->state.cur_location[1];
		//delete this;
	}

	this->update_matrix();

}
vec2 Bullet::getCur_loc()
{
	return this->state.cur_location;
}



void Bullet::draw(mat4 proj) {
	glUseProgram(GLvars.program);
	glBindVertexArray(GLvars.vao);


	//If you have a modelview matrix, pass it with proj
	glUniformMatrix4fv(GLvars.M_location, 1, GL_TRUE, proj * this->state.M);


	//Draw something
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	//glDrawArrays(GL_POINTS, 0, 2);



	glBindVertexArray(0);
	glUseProgram(0);
}

bool Bullet::getVisibility()
{
	return state.visible;
}

void Bullet::setVisbility(bool viz)
{
	state.visible = viz;
}

void Bullet::setLoc(vec2 loca)
{
	state.cur_location = loca;
}

bool Bullet::getCanHit()
{
	return state.canHit;
}

void Bullet::setCanHit(bool can)
{
	state.canHit = can;
}

void Bullet::checkLoc()
{
	if (state.cur_location[0] > 1 || state.cur_location[0] < -1 || state.cur_location[1] > 1 || state.cur_location[1] < -1) {
		state.canHit = false;
	}
}
