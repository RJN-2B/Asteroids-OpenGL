//
//  Ship.cpp
//  Asteroids
//
//
//

#include "common.h"



//Ship constructor
Ship::Ship() {
	//Set up initial state here
	//this->state.pointing = vec2(0, -1);
	//this->state.pointing = vec2(0, 10.0 * 0.01);
	this->state.pointing = vec2(0, 0.1);
	//this->state.angle = 0.0;



};



//Called everytime an animation tick happens
void Ship::update_state() {



		// Things to do:
		//a = F
		//Force is in the direction the ship is pointing
		GLfloat dt = 0.033; //fixed timestep based on 30 fps
		if (this->state.thruster_on) {
			vec2 force = this->state.pointing;
			vec2 accel = force * _ACC;
			//Clamp acceleration at some maximum value


			accel[0] = clamp(accel[0], -10.0, _MAX_SPEED + 5);
			accel[1] = clamp(accel[1], -10.0, _MAX_SPEED + 5);

			//v  = old_velocity + a*dt
			this->state.velocity = this->state.velocity + accel * dt;
			// clamp velocity at a maximum value



			// Dampen the velocity at every timestep to lessen intertia


		}


		this->state.velocity[0] = clamp(this->state.velocity[0], -10.0, _MAX_SPEED + 5);
		this->state.velocity[1] = clamp(this->state.velocity[1], -10.0, _MAX_SPEED + 5);
		this->state.velocity *= _DAMPING;


		//p  = old_position + v*dt
		this->state.cur_location = this->state.cur_location + this->state.velocity * dt;

		// Wrap the ship position when at the boundary

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

		this->update_matrix(vec2(0.0, 0.0));


		ship_box[0] = vec2(0.07, -0.02) + state.cur_location;
		ship_box[1] = vec2(-0.07, -0.02) + state.cur_location;
		ship_box[2] = vec2(-0.07, 0.12) + state.cur_location;
		ship_box[3] = vec2(0.07, 0.12) + state.cur_location;

	

	
}



void Ship::pew_pew() {
	Bullet *b = new Bullet(this->state.velocity, this->state.pointing, this->state.cur_location);
	bullets.push_back(b);
	bullets[bullet_count]->gl_init();
	bullet_count++;
}

std::vector<Bullet*> Ship::get_bullets_vector()
{
	return bullets;
}












//Initialize the gl state and variables
void Ship::gl_init() {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(ship_vert) + sizeof(ship_color), NULL, GL_STATIC_DRAW);

	//First part of array holds vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ship_vert), ship_vert);
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ship_vert), sizeof(ship_color), ship_color);





	glEnableVertexAttribArray(GLvars.vpos_location);
	glEnableVertexAttribArray(GLvars.vcolor_location);



	glVertexAttribPointer(GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ship_vert)));

	glBindVertexArray(0);


	//Below is for flames

		// Create a vertex array object
	glGenVertexArrays(1, &GLvars.vao_flames);
	//Set GL state to use vertex array object
	glBindVertexArray(GLvars.vao_flames);

	//Generate buffer to hold our vertex data
	glGenBuffers(1, &GLvars.buffer_flames);
	//Set GL state to use this buffer
	glBindBuffer(GL_ARRAY_BUFFER, GLvars.buffer_flames);

	//Create GPU buffer to hold vertices and color
	glBufferData(GL_ARRAY_BUFFER, sizeof(ship_flames) + sizeof(ship_flames_color), NULL, GL_STATIC_DRAW);
	//First part of array holds vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ship_flames), ship_flames);
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ship_flames), sizeof(ship_flames_color), ship_flames_color);

	glEnableVertexAttribArray(GLvars.vpos_location_flames);
	glEnableVertexAttribArray(GLvars.vcolor_location_flames);

	glVertexAttribPointer(GLvars.vpos_location_flames, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(GLvars.vcolor_location_flames, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ship_flames)));

	glBindVertexArray(0);

}

//Bullet Ship::shoot()
//{
//	Bullet b = Bullet(this->state.velocity, this->state.pointing);
//	return b;
//}


//Draw a ship
void Ship::draw(mat4 proj) {

	glUseProgram(GLvars.program);
	glBindVertexArray(GLvars.vao);

	
	//If you have a modelview matrix, pass it with proj
	glUniformMatrix4fv(GLvars.M_location, 1, GL_TRUE, proj * this->state.M);


	//Draw something
	glDrawArrays(GL_LINE_STRIP, 0, 8);



	if (state.thruster_on) {
		//Maybe draw something different if the thruster is on
		//glUseProgram(GLvars.program);
		//glBindVertexArray(GLvars.vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
		//glBindVertexArray(0);
		//glUseProgram(0);

	}

	glBindVertexArray(0);
	glUseProgram(0);

}



GLfloat * Ship::getShipXArray(Ship ship)
{
	GLfloat vertx[8];

	for (int i = 0; i < 8; i++) {
		vertx[i] = ship.state.cur_location[0];
		vertx[i] += ship.ship_vert[i][0];

	}

	return vertx;
}

void Ship::kill_bullet(int index)
{
	//this->bullets
}

vec2 * Ship::getShipVertLoc()
{
	vec2 vert[8];

	for (int i = 0; i < 8; i++) {
		vert[i] = state.cur_location;
		vert[i] += ship_vert[i];
	}
	return vert;
}

vec2 Ship::getLoc()
{
	return this->state.cur_location;
}

vec2 * Ship::getShipBox()
{
	return ship_box;
}


GLfloat * Ship::getShipYArray(Ship ship)
{
	GLfloat vertY[8];

	for (int i = 0; i < 8; i++) {
		vertY[i] = ship.state.cur_location[1];
		vertY[i] += ship.ship_vert[i][1];

	}
	return vertY;
}

vec2 * Ship::getShipVertLoc(Ship ship)
{
	vec2 vert[8];

	for (int i = 0; i < 8; i++) {
		vert[i] = ship.state.cur_location;
		vert[i] += ship.ship_vert[i];
	}
	return vert;
}
