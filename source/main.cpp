#include "common.h"

//Added
#include <ctime>
#include <math.h>


//Roland Nguyen

using namespace Angel;





Ship ship;







std::vector < Asteroid > asteroids (1); 







bool PointInPoly2(const vec2* polygon, const vec2& point, int numVert) {
	bool inside = false;
	vec2 p1;
	vec2 p2;
	for (int i = 0; i < numVert - 1; i++) { //9 vertices of asteroid
		p1 = polygon[i];
		p2 = polygon[i + 1];
		if (p1[1] > p2[1]) std::swap(p1,p2);
		if (point[1] > p1[1]) {
			if (point[1] <= p2[1]) {
				if (p1[1] != p2[1]) {
					if ((point[0] - p1[0]) * (p2[1] - p1[1]) - ((p2[0] - p1[0]) * ((point[1] - p1[1]))) > 0) {
					inside = !inside;
					}
				}
			}
		}
	}
	return inside;
}

bool checkRects(vec2 l1, vec2 r1, vec2 l2, vec2 r2)
{
	
	if (l1[0] > r2[0] || l2[0] > r1[0])
		return false;

	// If one rectangle is above other 
	if (l1[1] < r2[1] || l2[1] < r1[1])
		return false;

	return true;
}














int cn_PnPoly(vec2 P, vec2* V, int n)
{
	int cn = 0;
	for (int i = 0; i < n; i++) {
		if (((V[i].y <= P.y) && (V[i + 1].y > P.y)) || ((V[i].y > P.y) && (V[i + 1].y <= P.y))) {
			float vt = (float)(P.y - V[i].y) / (V[i + 1].y - V[i].y);
			if (P.x < V[i].x + vt * (V[i + 1].x - V[i].x)) ++cn;
		}
	}
	return (cn & 1);

}






static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		ship.rotateLeft();
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		ship.rotateRight();
	if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			ship.start_thruster();


		}
		if (action == GLFW_RELEASE) {
			ship.stop_thruster();
		}
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		//!!!!!!!!Fire bullet
		ship.pew_pew();
	}


}



void init() {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	ship.gl_init();


	for (int i = 0; i < asteroids.size(); i++) {
		if (asteroids[i].isOriginal()) {
			asteroids[i].randomize_asteroid();
			asteroids[i].gl_init();
			asteroids[i].setCanSplit(true);
			asteroids[i].setVisibility(true);
		}
	}
}

//Call update function 30 times a second
void animate() {
	if (glfwGetTime() > 0.0025) { // 120 times a sec for the checking bullet in asteroid part
		for (int j = 0; j < asteroids.size(); j++) {
			for (int i = 0; i < ship.get_bullets_vector().size(); i++) {

				if (ship.get_bullets_vector()[i]->getCanHit()) {
					if (asteroids[j].getVisibility() != false) {
						asteroids[j].updateAstBox();
						if (cn_PnPoly(ship.get_bullets_vector()[i]->getCur_loc(), asteroids[j].getAstBox(), 4) == 1) {
							ship.get_bullets_vector()[i]->setCanHit(false);
							if (asteroids[j].getCanSplit()) {

								for (int k = 0; k < 4; k++) {


									asteroids.emplace_back();
									asteroids[k + j].setVisibility(true);
									asteroids[k + j].setOriginality(false);
									asteroids[k + j].randomize_asteroid();
									asteroids[k + j].shrink(asteroids[j]);
									asteroids[k + j].setLocation(asteroids[j].directLoc());
									asteroids[k + j].gl_init();
								}

								asteroids[j].setCanSplit(false);
								ship.get_bullets_vector()[i]->setCanHit(false);
								delete ship.get_bullets_vector()[i];


							}
							ship.get_bullets_vector()[i]->setLoc(vec2(-6666666, -6666666));
							ship.get_bullets_vector()[i]->setCanHit(false);
							asteroids[j].setVisibility(false);
							asteroids[j].setLocation(vec2(-99999999, -999999999));

						}

					}
				}
			}
		}



	}


	if (glfwGetTime() > 0.033) {
		glfwSetTime(0.0);



		ship.update_state();

		for (int i = 0; i < ship.get_bullets_vector().size(); i++) {
			ship.get_bullets_vector()[i]->update_state();
		}






		for (int i = 0; i < asteroids.size(); ++i) {
			if (asteroids[i].getDirection() == 0) {
				asteroids[i].rotateRight();
			}
			else {
				asteroids[i].rotateLeft();
			}
			
		}



		for (std::vector < Asteroid > ::iterator it = asteroids.begin(); it != asteroids.end(); ++it) {
			(*it).update_state();
		}





		for (int i = 0; i < asteroids.size(); i++) {
			if(asteroids[i].getCanHurt() && asteroids[i].getVisibility()){
				if (checkRects(asteroids[i].getAstBox()[0], asteroids[i].getAstBox()[2], ship.getShipBox()[2], ship.getShipBox()[0])) {
					glClearColor(1.0, 1.0, 1.0, 0.0);
				}
			}
		}










	}
}



int main(void)
{





	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i].randomize_asteroid();
	}
	

	srand(time(NULL));

	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 10);


	window = glfwCreateWindow(1024, 768, "Asteroids!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	init();

	while (!glfwWindowShouldClose(window)) {

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//Pick a coordinate system that makes the most sense to you
		//(left, right, top, bottom)
		mat4 proj = Ortho2D(1.0, -1.0, -1.0, 1.0); //Original

		animate();

		glClear(GL_COLOR_BUFFER_BIT);




		ship.draw(proj);

		if (asteroids.size() > 0) {
			for (int i = 0; i < asteroids.size(); ++i) {
				asteroids[i].draw(proj);
				
			}

			for (int i = 0; i < ship.get_bullets_vector().size(); i++) {
				if (ship.get_bullets_vector()[i]->getVisibility() != false) {
					ship.get_bullets_vector()[i]->draw(proj);
				}
			}



			
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}




