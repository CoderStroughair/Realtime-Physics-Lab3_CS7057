
#include <windows.h>
#include "Antons_maths_funcs.h"
#include "teapot.h" // teapot mesh
#include "Utilities.h"
#include "Particle.h"
#include "Collision.h"
using namespace std;

const float width = 900, height = 900;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/
Mesh monkeyhead_object, torch_object, wall_object, cube, bear_object, signReflect_object, signRefract_object, signNormal_object;
Mesh gem_object, sphere_object, particle_object, cube_object;

GLuint vao;
//Cube Vertices for the Skybox
GLfloat vertices[] = {
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,

	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,

	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,

	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,

	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,

	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f
}; 

vec3 mouseLocation = vec3(0.0f, 0.5f, 0.0f);
vec3 closestPoint = vec3(0.5f, -0.5f, 0.0f);

vec3 triangle[] =
{
	vec3(0.0f, 0.5f, 0.0f),
	vec3(0.5f, -0.5f, 0.0f),
	vec3(-0.5f, -0.5f, 0.0f)
};


/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint noTextureShaderID, textureShaderID, cubeMapTextureID, refractiveShaderID, cubeMapShaderID, simpleShaderID;
GLuint testID, normalisedShaderID, reflectiveShaderID, multiTextureShaderID, mirrorShaderID, debugSkyboxID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/

vec3 startingPos = { 0.0f, 0.0f, -20.0f };
vec3 startingFront = { 0.0f, 0.0f, 1.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;
Camera cam(startingPos, startingFront, vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.0f);
RigidBody body;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
bool torque = false;
bool force = false;
bool gravity = false;
bool force1 = false, force2 = false;
/*----------------------------------------------------------------------------
						FUNCTION DEFINITIONS
----------------------------------------------------------------------------*/

void drawloop(mat4 view, mat4 proj, GLuint framebuffer);

/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	cam.setSensitivity(2.0f);
	//*************************//
	//*****Compile Shaders*****//
	//*************************//
	refractiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, FRESNEL_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory.CompileShader(SKY_VERT, SKY_FRAG);
	textureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	normalisedShaderID = shaderFactory.CompileShader(NORMAL_VERT, NORMAL_FRAG);
	reflectiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, REFLECTIVE_FRAG);
	multiTextureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	mirrorShaderID = shaderFactory.CompileShader(MIRROR_VERT, MIRROR_FRAG);
	debugSkyboxID = shaderFactory.CompileShader(DEBUG_VERT, DEBUG_FRAG);
	testID = shaderFactory.CompileShader(TEST_VERT, TEST_FRAG);
	simpleShaderID = shaderFactory.CompileShader(SIMPLE_VERT, SIMPLE_FRAG);
	//*********************//
	//*****Init Objects*****//
	//*********************//
	monkeyhead_object.init(MONKEYHEAD_MESH);
	cube.initCubeMap(vertices, 36, "desert");
	torch_object.init(TORCH_MESH);
	bear_object.init(BEAR_MESH, BEAR_TEXTURE, BEAR_TEXTURE2);
	wall_object.init(WALL_MESH, NULL, BRICK_NORMAL);
	signReflect_object.init(SIGN_MESH, REFLECT_TEXTURE);
	signRefract_object.init(SIGN_MESH, REFRACT_TEXTURE);
	signNormal_object.init(SIGN_MESH, NORMAL_TEXTURE);
	sphere_object.init(SPHERE_MESH);
	gem_object.init(GEM_MESH);
	particle_object.init(GEM_MESH);
	cube_object.init(CUBE_MESH, BEAR_TEXTURE);

	body = RigidBody(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 5, 1.0, 1.0, 1.0, cube_object);
}

void display() 
{
	mat4 proj = perspective(87.0, (float)width / (float)(height), 1, 1000.0);
	mat4 view;
	glViewport(0, 0, width, height);
		
	view = look_at(cam.getPosition(), cam.getPosition() + cam.getFront(), cam.getUp());
	proj = perspective(60.0, (float)width / (float)height, 1, 1000.0);
	glViewport(0, 0, width, height);
	drawloop(view, proj, 0);
	draw_texts();
	glutSwapBuffers();
}

void updateScene() {
	static DWORD  last_frame;	//time when last frame was drawn
	static DWORD last_timer = 0;	//time when timer was updated
	DWORD  curr_time = timeGetTime();//for frame Rate Stuff.
	static bool first = true;
	if (first)
	{
		last_frame = curr_time;
		first = false;
	}
	float  delta = (curr_time - last_frame) * 0.001f;
	if (delta >= 0.03f) 
	{
		last_frame = curr_time;
		glutPostRedisplay();
		rotateLight = rotateLight + 0.01f;
		if (rotateLight >= 360.0f)
			rotateLight = 0.0f;

		body.force = vec3(0.0, 0.0, 0.0);
		body.torque = vec3(0.0, 0.0, 0.0);
		body.addForce(vec3(50.0, 0.0, 0.0)*force1, vec3(0.5, 0.0, 0.5));
		body.addForce(vec3(2.0, 50.0, 0.0)*force2, vec3(0.0, 1.0, 0.0));
		if (gravity)
			body.torque += vec3(0.0, 0.0, 50.0);
		if (torque)
			body.torque += vec3(50.0, 0.0, 0.0);
		if (force)
			body.torque += vec3(0.0, 50.0, 0.0);
		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);

		body.resolveForce(delta);
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) 
{
	if (key == (char)27)	//Pressing Escape Ends the game
	{
		exit(0);
	}
	else if (key == 'w' || key == 'W')
		frontCam = 1;
	else if ((key == 's') || (key == 'S'))
		frontCam = -1;
	if ((key == 'a') || (key == 'A'))
		sideCam = -1;
	else if ((key == 'd') || (key == 'D'))
		sideCam = 1;
	if ((key == 't') || (key == 'T'))
		torque = true;
	if ((key == 'f') || (key == 'F'))
		force = true;
	if ((key == 'g') || key == 'G')
		gravity = true;
	if (key == '1')
		force1 = true;
	if (key == '2')
		force2 = true;
}

void keypressUp(unsigned char key, int x, int y)
{
	if ((key == 'w') || (key == 'W'))
		frontCam = 0;
	else if ((key == 's') || (key == 'S'))
		frontCam = 0;
	if ((key == 'a') || (key == 'A'))
		sideCam = 0;
	else if ((key == 'd') || (key == 'D'))
		sideCam = 0;
	if ((key == 't') || (key == 'T'))
		torque = false;
	if ((key == 'f') || (key == 'F'))
		force = false;
	if (key == '1')
		force1 = false;
	if (key == '2')
		force2 = false;
	if (key == ' ')
	{
		body.reset(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0));
	}
	else if (key == '\\')
	{
		body.reset(vec3(0.0, 0.0, 0.0), vec3(500.0, 500.0, 500.0));
	}
	if ((key == 'g') || key == 'G')
	{
		gravity = false;
	}
}

void specialKeypress(int key, int x, int y) 
{
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 4;
		break;
	case (GLUT_KEY_LEFT):
		yawCam = -1;
		break;
	case (GLUT_KEY_RIGHT):
		yawCam = 1;
		break;
	case (GLUT_KEY_UP):
		pitCam = 1;
		break;
	case (GLUT_KEY_DOWN):
		pitCam = -1;
		break;
	}
}

void specialKeypressUp(int key, int x, int y) 
{
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 1;
		break;
	case (GLUT_KEY_LEFT):
		yawCam = 0;
		break;
	case (GLUT_KEY_RIGHT):
		yawCam = 0;
		break;
	case (GLUT_KEY_UP):
		pitCam = 0;
		break;
	case (GLUT_KEY_DOWN):
		pitCam = 0;
		break;
	}
}

void (mouse)(int x, int y)
{
	mouseLocation.v[0] = ((2 / width) * x) - 1;
	mouseLocation.v[1] = 1 - (((2 / height) * y));

	closestPoint = getClosestPointTriangle(triangle, mouseLocation);

	string text;
	text += "P1 = [" + to_string(triangle[0].v[0]) + "," + to_string(triangle[0].v[1]) + "," + to_string(triangle[0].v[2]) + "]\n";
	text += "P2 = [" + to_string(triangle[1].v[0]) + "," + to_string(triangle[1].v[1]) + "," + to_string(triangle[1].v[2]) + "]\n";
	text += "P3 = [" + to_string(triangle[2].v[0]) + "," + to_string(triangle[2].v[1]) + "," + to_string(triangle[2].v[2]) + "]\n";
	text += "Distance = " + to_string(getDistance(mouseLocation, closestPoint)) + "\n";
	update_text(textID, text.c_str());
}

#pragma endregion INPUT FUNCTIONS

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GameApp");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);


	// Tell glut where the display function is
	glutWarpPointer(width / 2, height / 2);
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	// Input Function Initialisers //
	glutKeyboardFunc(keypress);
	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(specialKeypress);
	glutSpecialUpFunc(specialKeypressUp);
	glutKeyboardUpFunc(keypressUp);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	init();
	textID = add_text("hi",
		-0.95, 0.9, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);

	glutMainLoop();
	return 0;
}

void drawloop(mat4 view, mat4 proj, GLuint framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Create a background

	drawLine(simpleShaderID, identity_mat4(), identity_mat4(), mouseLocation, closestPoint, PURPLE);
	

	if (closestPoint == mouseLocation)
		drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[0], triangle[1], triangle[2], PURPLE);
	else
		drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[0], triangle[1], triangle[2], YELLOW);

	if (closestPoint != triangle[0] && closestPoint != triangle[1] && closestPoint != triangle[2] && closestPoint != mouseLocation)
	{
		//<p1, p2>
		if (dot(normalise(triangle[0] - closestPoint), normalise(triangle[0] - triangle[1])) >= 0.999)
			drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[0], triangle[1], mouseLocation, PURPLE);
		//<p2, p3>
		if (dot(normalise(triangle[1] - closestPoint), normalise(triangle[1] - triangle[2])) >= 0.999)
			drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[1], triangle[2], mouseLocation, PURPLE);
		//<p3, p1>
		if (dot(normalise(triangle[2] - closestPoint), normalise(triangle[2] - triangle[0])) >= 0.999)
			drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[2], triangle[0], mouseLocation, PURPLE);
	}
	vec3 v12 = normalise(triangle[0] - triangle[1]);
	vec3 v23 = normalise(triangle[1] - triangle[2]);
	vec3 v31 = normalise(triangle[2] - triangle[0]);

	string text;
	text += "P1 = [" + to_string(triangle[0].v[0]) + "," + to_string(triangle[0].v[1]) + "," + to_string(triangle[0].v[2]) + "]\n";
	text += "P2 = [" + to_string(triangle[1].v[0]) + "," + to_string(triangle[1].v[1]) + "," + to_string(triangle[1].v[2]) + "]\n";
	text += "P3 = [" + to_string(triangle[2].v[0]) + "," + to_string(triangle[2].v[1]) + "," + to_string(triangle[2].v[2]) + "]\n";
	text += "Distance = " + to_string(getDistance(mouseLocation, closestPoint)) + "\n";
	text += "v12 = [" + to_string(v12.v[0]) + "," + to_string(v12.v[1]) + "," + to_string(v12.v[2]) + "]\n";
	text += "v23 = [" + to_string(v23.v[0]) + "," + to_string(v23.v[1]) + "," + to_string(v23.v[2]) + "]\n";
	text += "v31 = [" + to_string(v31.v[0]) + "," + to_string(v31.v[1]) + "," + to_string(v31.v[2]) + "]\n";
	text += "dot(p0,v12) = " + to_string(dot(normalise(triangle[0] - closestPoint), v12)) + "\n";
	text += "dot(p0,v23) = " + to_string(dot(normalise(triangle[1] - closestPoint), v23)) + "\n";
	text += "dot(p0,v31) = " + to_string(dot(normalise(triangle[2] - closestPoint), v31)) + "\n";
	update_text(textID, text.c_str());


	
	
}

