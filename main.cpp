#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "skybox.h"

float skyTicks = 0.0f;

int main() 
{
	
	stbi_set_flip_vertically_on_load(true);

#pragma region Initialization
	//initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Gaurana_Aldrey, Tallador_Eryn", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Mesh Loading

	ObjData grassPatch;
	LoadObjFile(&grassPatch, "grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
	GLfloat grassPatchOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&grassPatch,
		1.0f,
		grassPatchOffsets,
		"grassPatch"
	);

	ObjData spaceship;
	LoadObjFile(&spaceship, "mars/Mars Lander Space Capsule.obj");
	GLfloat spaceshipOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&spaceship,
		1.0f,
		spaceshipOffsets,
		"spaceship"
	);

	ObjData serverrack;
	LoadObjFile(&serverrack, "laptop/ServerV2+console.obj");
	GLfloat serverrackOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&serverrack,
		1.0f,
		serverrackOffsets,
		"serverrack"
	);

	ObjData csuTower;
	LoadObjFile(&csuTower, "comp/10104_Computer_CPU_Case_v1_L3.obj");
	GLfloat csuTowerOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&csuTower,
		1.0f,
		csuTowerOffsets,
		"csuTower"
	);

	ObjData woodHouse;
	LoadObjFile(&woodHouse, "house/farmhouse_obj.obj");
	GLfloat woodHouseOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&woodHouse,
		1.0f,
		woodHouseOffsets,
		"woodHouse"
	);

	ObjData planet;
	LoadObjFile(&planet, "mars/Earth.obj");
	GLfloat planetOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&planet,
		1.0f,
		planetOffsets,
		"planet"
	);
	
	std::vector<std::string> faces_evening
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};

	std::vector<std::string> faces_morning
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};

	std::vector<std::string> faces_afternoon
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};

	SkyBoxData skybox_night = loadSkyBox("Assets/skybox", faces_evening);
	SkyBoxData skybox_morning = loadSkyBox("Assets/skybox/day", faces_morning);
	SkyBoxData skybox_afternoon = loadSkyBox("Assets/skybox/afternoon", faces_afternoon);
#pragma endregion

#pragma region Shader Loading

	//load skybox shader program
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");

	//load shader program
	GLuint shaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/phong_fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");

	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");
	
	//initialize normal transformation
	GLuint normalTransformLoc = glGetUniformLocation(shaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_pos");
	GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.0f, 0.0f, 0.15f);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glm::mat4 trans1 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans1));

	glm::mat4 trans2 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

	glm::mat4 trans3 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans3));

	glm::mat4 trans4 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans4));

	glm::mat4 trans5 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans5));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//setup light shading
	//POINT LIGHT
	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	//DIRECTIONAL LIGHT
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	//glUniform3f(lightPosLoc, 3.0f, 0.0f, 0.0f);
	

	//flag for shading
	GLuint modelIdLoc = glGetUniformLocation(shaderProgram, "u_model_id");


#pragma endregion

	// set bg color to green
	glClearColor(0.0f, 0.0f, 0.15f, 0.0f);

	// var for rotations and delta time
	float xFactor = 0.0f;
	float xSpeed = 1.0f;
	
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;

	float rotFactor = 0.0f;
	float rotSpeed = 10.0f;

	//movement
	float walkSpeed = 10.0f;

	//light directions and control
	float lightX = -20.0f;
	float lightY = 0.0f;
	float lightSlow = 0.1f;

	//stopping time of day
	bool isTimeStopped = false;

	//Keyboard Input Check
	bool checkPress = false;

	//Cursor Position Check
	double initX = 0.0f, initY = 0.0f;
	double currX = 0.0f, currY = 0.0f;
	bool cursorMove = false;

	//Camera Definitions
	glm::vec3 cFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cPos = glm::vec3(0.0f, -3.0f, 1.0f);
	glm::vec3 cUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cRight = glm::vec3(1.0f, 0.0f, 0.0f);
	float yaw = 0.0f;
	float pitch = 0.0f;
	float sensitivity = 0.5f;
	

	//depth testing
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // set the depth test function

	//face culling
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // set which face to cull
	//glFrontFace(GL_CCW); // set the front face orientation


	while (!glfwWindowShouldClose(window)) 
	{

#pragma region Viewport
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
#pragma endregion

#pragma region Projection

		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 1000.0f),

			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		

#pragma endregion

#pragma region View
		glm::mat4 viewHold = glm::mat4(1.0f); // identity

	//Mouse Hover Rotation
		glfwGetCursorPos(window, &initX, &initY);

		if (cursorMove)
		{
			currX = initX;
			currY = initY;
			cursorMove = false;
		}

		float mouseOffsetX = initX - currX;
		float mouseOffsetY = currY - initY;

		mouseOffsetX *= sensitivity;
		mouseOffsetY *= sensitivity;

		currX = initX;
		currY = initY;

		yaw += mouseOffsetX;
		pitch += mouseOffsetY;

		//ANGLE LOCKS
		if (pitch < -80.0f)
		{
			pitch = -80.0f;
		}

		if (pitch > 60.0f)
		{
			pitch = 60.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cFront = glm::normalize(front);
		cRight = glm::normalize(glm::cross(cFront, cUp));

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;

		if (isTimeStopped == false)
		{
			skyTicks += deltaTime;
			lightX += deltaTime * lightSlow;
			lightY += deltaTime * lightSlow;
		}

		//Keyboard Input
		if (checkPress == false) {
			//Forward
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				checkPress = true;
			}
			//Left
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				checkPress = true;
			}
			//Right
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				checkPress = true;
			}
			//Backward
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				checkPress = true;
			}
			if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
				checkPress = true;
			}
		}
		else {
			//Forward
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				cPos += cFront * deltaTime * walkSpeed;
				if (cPos.y > -3.0f)
				{
					cPos.y = -3.0f;
				}
				else if (cPos.y < -4.0f)
				{
					cPos.y = -4.0f;
				}
			}
			//Left
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				cPos -= cRight * deltaTime * walkSpeed;
			}
			//Backward
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				cPos -= cFront * deltaTime * walkSpeed;
				if (cPos.y > -3.0f)
				{
					cPos.y = -3.0f;
				}
				else if (cPos.y < -4.0f)
				{
					cPos.y = -4.0f;
				}
			}
			//Right
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				cPos += cRight * deltaTime * walkSpeed;
			}

			if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
				if(isTimeStopped == false)
					isTimeStopped = true;
				else if (isTimeStopped == true)
				{
					isTimeStopped = false;
				}
			}
		}

		//DIRECTIONAL LIGHT
		glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);
		std::cout << glm::sin(lightX) << " " << glm::cos(lightX) << std::endl;
		//POINT LIGHT
		//glUniform3f(lightPosLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
		glm::mat4 view = glm::lookAt(
			cPos,  //EYE
			cPos + cFront,  //CENTER
			cUp  //UP
		);

		//camera pos locks
		if (cPos.z < -48.0f)
		{
			cPos.z = -48.0f;
		}
		if (cPos.z > 98.0f)
		{
			cPos.z = 98.0f;
		}
		if (cPos.x > 73.0f)
		{
			cPos.x = 73.0f;
		}
		if (cPos.x < -72.0f)
		{
			cPos.x = -72.0f;
		}

		glUniform3f(cameraPosLoc,cPos.x, cPos.y, cPos.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		//-------------------------------------------------------------
#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw

		//--------------------------------------------
		//DRAWING SKYBOX
		if (skyTicks >= 0.0f && skyTicks < 20.0f)
		{
			DrawSkybox(skybox_morning, skyboxShaderProgram, view, projection);
		}
		else if (skyTicks >= 20.0f && skyTicks < 40.0f)
		{
			DrawSkybox(skybox_afternoon, skyboxShaderProgram, view, projection);
		}
		else if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			DrawSkybox(skybox_night, skyboxShaderProgram, view, projection);
		}

		if (skyTicks > 60.0f)
		{
			skyTicks = 0.0f;
		}
		
		glUseProgram(shaderProgram);


		//--------------------------------------------------------
		//DELTA TIME

		// incerement rotation by deltaTime
		
		rotFactor += deltaTime * rotSpeed;

		prevTime = currentTime;

		



		//----------------------------------------------------------
		//draw GRASS
		glBindVertexArray(grassPatch.vaoId);
		glUniform1f(modelIdLoc, 1.2f);

		// transforms
		trans = glm::mat4(1.0f); // identity
		//trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::translate(trans, glm::vec3(0.0f, -10.0f, 25.0f)); // matrix * translate_matrix
		//trans = glm::rotate(trans, glm::radians(rotFactor * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::rotate(trans, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		
		//send to shader
		glm::mat4 normalTrans = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		

		glActiveTexture(GL_TEXTURE0);
		GLuint grassPatchTexture = grassPatch.textures[grassPatch.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, grassPatchTexture);
	

		//drawbackpack
		glDrawElements(GL_TRIANGLES, grassPatch.numFaces, GL_UNSIGNED_INT, (void*)0);

		//---------------------------------------------------------------------

		
		//----------------------------------------------------------------
		//draw spaceship
		
		glBindVertexArray(spaceship.vaoId);
		glUniform1f(modelIdLoc, 1.2f);

		// transforms
		trans1 = glm::mat4(1.0f); // identity
		//trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans1 = glm::translate(trans1, glm::vec3(0.0f, -2.0f, 80.0f)); // matrix * translate_matrix
		//trans1 = glm::rotate(trans1, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f));
		trans1 = glm::scale(trans1, glm::vec3(0.1f, 0.1f, 0.1f));
		
		glm::mat4 normalTrans1 = glm::transpose(glm::inverse(trans1));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans1));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans1));

		GLuint spaceshipTexture2 = spaceship.textures[spaceship.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, spaceshipTexture2);
		GLuint spaceshipTexture = spaceship.textures[spaceship.materials[1].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, spaceshipTexture);

		glDrawElements(GL_TRIANGLES, spaceship.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------
		

		//----------------------------------------------------------------
		//server rack

		glBindVertexArray(serverrack.vaoId);
		glUniform1f(modelIdLoc, 1.2f);

		// transforms
		trans2 = glm::mat4(1.0f); // identity
		trans2 = glm::translate(trans2, glm::vec3(0.0f, -4.0f, 0.0f));
		trans2 = glm::rotate(trans2, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		trans2 = glm::rotate(trans2, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans2 = glm::rotate(trans2, glm::radians(rotFactor), glm::vec3(1.0f, 0.0f, 0.0f));
		trans2 = glm::scale(trans2, glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(trans2));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

		GLuint serverrackTexture = serverrack.textures[serverrack.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, serverrackTexture);

		glDrawElements(GL_TRIANGLES, serverrack.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		//----------------------------------------------------------------
		//house

		glBindVertexArray(woodHouse.vaoId);
		glUniform1f(modelIdLoc, 1.2f);

		// transforms
		trans3 = glm::mat4(1.0f); // identity
		trans3 = glm::translate(trans3, glm::vec3(-20.0f, -7.0f, 0.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(1.0f, 0.0f, 0.0f));
		trans3 = glm::scale(trans3, glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 normalTrans3 = glm::transpose(glm::inverse(trans3));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans3));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans3));

		GLuint woodHouseTexture = woodHouse.textures[woodHouse.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, woodHouseTexture);

		glDrawElements(GL_TRIANGLES, woodHouse.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		//----------------------------------------------------------------
		//draw cpu tower

		glBindVertexArray(csuTower.vaoId);
		glUniform1f(modelIdLoc, 1.2f);

		// transforms
		trans4 = glm::mat4(1.0f); // identity
		trans4 = glm::translate(trans4, glm::vec3(5.0f, 0.0f, 30.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans4 = glm::rotate(trans4, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		trans4 = glm::scale(trans4, glm::vec3(0.05f, 0.05f, 0.05f));

		glm::mat4 normalTrans4 = glm::transpose(glm::inverse(trans4));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans4));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans4));

		GLuint csuTowerTexture = csuTower.textures[csuTower.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, csuTowerTexture);

		glDrawElements(GL_TRIANGLES, csuTower.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------


		//----------------------------------------------------------------
		//draw planet

		glBindVertexArray(planet.vaoId);
		glUniform1f(modelIdLoc, 1.2f);

		// transforms
		trans5 = glm::mat4(1.0f); // identity
		trans5 = glm::translate(trans5, glm::vec3(150.0f, 100.0f, 0.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		trans5 = glm::rotate(trans5, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // matrix * rotation_matrix
		trans5 = glm::rotate(trans5, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		trans5 = glm::scale(trans5, glm::vec3(10.0f, 10.0f, 10.0f));

		glm::mat4 normalTrans5 = glm::transpose(glm::inverse(trans5));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans5));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans5));

		GLuint planetTexture = planet.textures[planet.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, planetTexture);

		glDrawElements(GL_TRIANGLES, planet.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		/*
		//----------------------------------------------------------------
		//draw EARTH
		glBindVertexArray(earth.vaoId);

		// transforms
		trans2 = glm::mat4(1.0f); // identity
		//trans2 = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans2 = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f));
		trans2 = glm::translate(trans2, glm::vec3(-3.0f, 0.0f, -7.0f)); // matrix * translate_matrix
		trans2 = glm::scale(trans2, glm::vec3(0.6f, 0.6f, 0.6f));
		
		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(trans2));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

		GLuint earthTexture = earth.textures[earth.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, earthTexture);

		glDrawElements(GL_TRIANGLES, earth.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------
		*/

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);
		


		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}