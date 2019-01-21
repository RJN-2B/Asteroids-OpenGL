#include "Asteroid.h"
#include "common.h"
#include <ctime>


Asteroid::Asteroid(){

	this->state.angle = 0.0;
	this->state.cur_location = vec2(floatRand(-1.0, 1.0), floatRand(-1.0, 1.0));
	this->state.velocity = vec2(floatRand(-.1, .1), (-.1, .1));
	this->state.direction = (rand() * RAND_MAX) % 2;
}
Asteroid::~Asteroid()
{
}
;

void Asteroid::update_state() {

	if (state.visible) {
		GLfloat dt = 0.0333333;

		vec2 force = this->state.pointing;
		vec2 accel = force * _ACC;
		//Clamp acceleration at some maximum value


		accel[0] = clamp(accel[0], -15.0, _MAX_SPEED - 5);
		accel[1] = clamp(accel[1], -15.0, _MAX_SPEED - 5);



		//v  = old_velocity + a*dt
		this->state.velocity = this->state.velocity + accel * dt;
		// clamp velocity at a maximum value
		this->state.velocity[0] = clamp(this->state.velocity[0], -15.0, _MAX_SPEED - 5);
		this->state.velocity[1] = clamp(this->state.velocity[1], -15.0, _MAX_SPEED - 5);

		this->state.cur_location = this->state.cur_location + this->state.velocity * dt;






		if (this->state.cur_location[0] > 1) {
			this->state.cur_location[0] = -2.0 + this->state.cur_location[0];
		}

		if (this->state.cur_location[0] < -1) {
			this->state.cur_location[0] = 2.0 + this->state.cur_location[0];
		}

		if (this->state.cur_location[1] > 1) {
			this->state.cur_location[1] = -2.0 + this->state.cur_location[1];
		}

		if (this->state.cur_location[1] < -1) {
			this->state.cur_location[1] = 2.0 + this->state.cur_location[1];
		}
	}
	else {
		this->state.cur_location = (-999999, -9999999);
	}

	this->update_matrix();

	


		asteroidBox[0] = tempBox[0] + state.cur_location;
		asteroidBox[1] = tempBox[1] + state.cur_location;
		asteroidBox[2] = tempBox[2] + state.cur_location;
		asteroidBox[3] = tempBox[3] + state.cur_location;

	


	
}

//Initialize the gl state and variables
void Asteroid::gl_init() {
	//Ship
	//!!!!!!!!Populate ship_vert and ship_color

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(asteroid_vert) + sizeof(asteroid_color), NULL, GL_STATIC_DRAW);
	//First part of array holds vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(asteroid_vert), asteroid_vert);
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(asteroid_vert), sizeof(asteroid_color), asteroid_color);

	glEnableVertexAttribArray(GLvars.vpos_location);
	glEnableVertexAttribArray(GLvars.vcolor_location);

	glVertexAttribPointer(GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(asteroid_vert)));

	glBindVertexArray(0);




}

//Draw an asteroid
void Asteroid::draw(mat4 proj) {

	glUseProgram(GLvars.program);
	glBindVertexArray(GLvars.vao);

	//If you have a modelview matrix, pass it with proj
	glUniformMatrix4fv(GLvars.M_location, 1, GL_TRUE, proj * this->state.M);
	

	//Draw something
	glDrawArrays(GL_TRIANGLE_FAN, 0, 10);

	glBindVertexArray(0);
	glUseProgram(0);




}

void Asteroid::rotateLeft()
{
	this->state.angle += 1;
	update_matrix();
}

void Asteroid::rotateRight(){
	this->state.angle -= 1;
	update_matrix();
}


void Asteroid::randomize_asteroid()
{
	if (state.original) {
		this->state.cur_location = vec2(floatRand(-1.0, 1.0), floatRand(-1.0, 1.0));
	}
	this->state.velocity = vec2(floatRand(-.1, .1), (-.1, .1));
	this->state.direction = (rand() * RAND_MAX) % 2;

}

vec2 * Asteroid::getOrigLocationAst()
{
	return asteroid_vert;
}

void Asteroid::shrink(Asteroid orig){
	for (int i = 0; i < 10; i++) {
		asteroid_vert[i] = orig.asteroid_vert[i] / 1.5;
	}

	for (int i = 0; i < 4; i++) {
		asteroidBox[i] = orig.asteroidBox[i] / 1.5;
		tempBox[i] = orig.tempBox[i] / 1.5;
	}
	state.original = false;
}

void Asteroid::set_location(vec2 formerLoc)
{
	this->state.cur_location = formerLoc;
}

vec2 Asteroid::directLoc()
{
	return state.cur_location;
}

bool Asteroid::getVisibility()
{
	return state.visible;
}

void Asteroid::setVisibility(bool viz)
{
	state.visible = viz;
}

void Asteroid::setLocation(vec2 loca)
{
	state.cur_location = loca;
}

vec2 * Asteroid::getAstBox()
{
	return asteroidBox;
}

void Asteroid::setRecursiveDepth(int i)
{
	state.recursive_depth = i;
}

bool Asteroid::isOriginal()
{
	return state.original;
}

void Asteroid::setOriginality(bool orig)
{
	state.original = false;
}

bool Asteroid::getCanSplit()
{
	return state.canSplit;
}

void Asteroid::setCanSplit(bool can)
{
	state.canSplit = can;
}

bool Asteroid::getCanHurt()
{
	return state.canHurt;
}

void Asteroid::setCanHurt(bool can)
{
	state.canHurt = can;
}

void Asteroid::updateAstBox()
{
	for (int i = 0; i < 4; i++) {
		asteroidBox[i] = tempBox[i] + state.cur_location;
	}
}




GLfloat * Asteroid::getVertXArrayAst(Asteroid ast)
{
	GLfloat vertx[10];

	for (int i = 0; i < 10; i++){
		vertx[i] = ast.state.cur_location[0];
		vertx[i] += ast.asteroid_vert[i][0];

	}
	
	return vertx;
}


GLfloat * Asteroid::getVertYArrayAst(Asteroid ast)
{
	GLfloat vertY[10];

	for (int i = 0; i < 10; i++) {
		vertY[i] = ast.state.cur_location[1];
		vertY[i] += ast.asteroid_vert[i][1];

	}
	return vertY;
}

vec2 * Asteroid::getAstVertLoc(Asteroid ast)
{
	vec2 vert[10];

	for (int i = 0; i < 10; i++) {
		vert[i] = ast.state.cur_location;
		vert[i] += ast.asteroid_vert[i];
	}
	return vert;
}


