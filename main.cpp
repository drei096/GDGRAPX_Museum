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
		grassPatchOffsets
	);

	ObjData spaceship;
	LoadObjFile(&spaceship, "mars/Mars Lander Space Capsule.obj");
	GLfloat spaceshipOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&spaceship,
		1.0f,
		spaceshipOffsets
	);

	ObjData serverrack;
	LoadObjFile(&serverrack, "laptop/ServerV2+console.obj");
	GLfloat serverrackOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&serverrack,
		1.0f,
		serverrackOffsets
	);

	ObjData csuTower;
	LoadObjFile(&csuTower, "comp/10104_Computer_CPU_Case_v1_L3.obj");
	GLfloat csuTowerOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&csuTower,
		1.0f,
		csuTowerOffsets
	);

	ObjData woodHouse;
	LoadObjFile(&woodHouse, "house/farmhouse_obj.obj");
	GLfloat woodHouseOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&woodHouse,
		1.0f,
		woodHouseOffsets
	);

	ObjData solarPanel;
	LoadObjFile(&solarPanel, "solarpanel/10781_Solar-Panels_V1.obj");
	GLfloat solarPanelOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&solarPanel,
		1.0f,
		solarPanelOffsets
	);

	ObjData rocket;
	LoadObjFile(&rocket, "rocket/12215_rocket_v3_l1.obj");
	GLfloat rocketOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&rocket,
		1.0f,
		rocketOffsets
	);

	ObjData basket;
	LoadObjFile(&basket, "basket/14029_Plastic_Fruit_Crate_v1_L1.obj");
	GLfloat basketOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&basket,
		1.0f,
		basketOffsets
	);

	ObjData lamppost;
	LoadObjFile(&lamppost, "lamppost/11832_Lamp_v1_l2.obj");
	GLfloat lamppostOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&lamppost,
		1.0f,
		lamppostOffsets
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

	glm::mat4 trans6 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans6));

	glm::mat4 trans7 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans7));

	glm::mat4 trans8 = glm::mat4(1.0f); // identity
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans8));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//setup light shading
	//POINT LIGHT
	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	//DIRECTIONAL LIGHT
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	//SPOT LIGHT
	GLuint lightSpotLoc = glGetUniformLocation(shaderProgram, "u_light_spot");
	//LIGHT CENTER
	GLuint lightSpotCenterLoc = glGetUniformLocation(shaderProgram, "u_light_spot_center");
	//glUniform3f(lightPosLoc, 3.0f, 0.0f, 0.0f);
	
	//for multitexturing
	GLuint diffuseTexLoc = glGetUniformLocation(shaderProgram, "texture_diffuse"); 
	GLuint secondaryDiffuseTexLoc = glGetUniformLocation(shaderProgram, "texture_secondary_diffuse");
	GLuint tertiaryDiffuseTexLoc = glGetUniformLocation(shaderProgram, "texture_tertiary_diffuse");
	GLuint normalTexLoc = glGetUniformLocation(shaderProgram, "texture_normal");
	glUniform1i(diffuseTexLoc, 0);
	glUniform1i(normalTexLoc, 1);
	glUniform1i(secondaryDiffuseTexLoc, 2);
	glUniform1i(tertiaryDiffuseTexLoc, 3);

	//flag for shading
	GLuint modelIdLoc = glGetUniformLocation(shaderProgram, "u_model_id");
	GLuint texTypeIdLoc = glGetUniformLocation(shaderProgram, "u_tex_type");


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
	float lightX = 45.0f;
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
	glm::vec3 cPos = glm::vec3(-5.0f, -3.0f, 1.0f);
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

		std::cout << lightX << std::endl;

		//std::cout << skyTicks << std::endl;

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

		glUniform3f(lightSpotLoc, cPos.x, cPos.y, cPos.z);
		
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
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, (cPos + cFront).x, (cPos + cFront).y, (cPos + cFront).z);
			glUniform3f(lightDirLoc, (cPos+cFront).x, (cPos + cFront).y, (cPos + cFront).z);

			glUniform1f(modelIdLoc, 1.4f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

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
	

		//draw grass
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawElements(GL_TRIANGLES, grassPatch.numFaces, GL_UNSIGNED_INT, (void*)0);
		glDisable(GL_BLEND);

		//---------------------------------------------------------------------

		
		//----------------------------------------------------------------
		//draw spaceship
		
		glBindVertexArray(spaceship.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

		// transforms
		trans1 = glm::mat4(1.0f); // identity
		//trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans1 = glm::translate(trans1, glm::vec3(0.0f, -2.0f, 80.0f)); // matrix * translate_matrix
		//trans1 = glm::rotate(trans1, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f));
		trans1 = glm::scale(trans1, glm::vec3(0.1f, 0.1f, 0.1f));
		
		glm::mat4 normalTrans1 = glm::transpose(glm::inverse(trans1));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans1));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans1));

		glActiveTexture(GL_TEXTURE0);
		GLuint spaceshipTexture = spaceship.textures[spaceship.materials[1].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, spaceshipTexture);

		glDrawElements(GL_TRIANGLES, spaceship.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------
		

		//----------------------------------------------------------------
		//server rack

		glBindVertexArray(serverrack.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

		// transforms
		trans2 = glm::mat4(1.0f); // identity
		trans2 = glm::translate(trans2, glm::vec3(0.0f, 0.0f, 0.0f));
		trans2 = glm::rotate(trans2, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		trans2 = glm::rotate(trans2, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans2 = glm::rotate(trans2, glm::radians(rotFactor), glm::vec3(1.0f, 0.0f, 0.0f));
		trans2 = glm::scale(trans2, glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(trans2));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));

		glActiveTexture(GL_TEXTURE0);
		GLuint serverrackTexture = serverrack.textures[serverrack.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, serverrackTexture);

		glDrawElements(GL_TRIANGLES, serverrack.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		//----------------------------------------------------------------
		//house

		glBindVertexArray(woodHouse.vaoId);
		glUniform1f(modelIdLoc, 1.3f);
		glUniform1f(texTypeIdLoc, 1.0f);

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

		glActiveTexture(GL_TEXTURE0);
		GLuint woodHouseTexture = woodHouse.textures[woodHouse.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, woodHouseTexture);

		glActiveTexture(GL_TEXTURE1);
		GLuint woodHouseNormal = woodHouse.textures[woodHouse.materials[0].bump_texname];
		glBindTexture(GL_TEXTURE_2D, woodHouseNormal);

		glDrawElements(GL_TRIANGLES, woodHouse.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		//----------------------------------------------------------------
		//draw cpu tower

		glBindVertexArray(csuTower.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

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

		glActiveTexture(GL_TEXTURE0);
		GLuint csuTowerTexture = csuTower.textures[csuTower.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, csuTowerTexture);

		glDrawElements(GL_TRIANGLES, csuTower.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------


		//----------------------------------------------------------------
		//draw solarPanel

		glBindVertexArray(solarPanel.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.1f);

		// transforms
		trans5 = glm::mat4(1.0f); // identity
		trans5 = glm::translate(trans5, glm::vec3(20.0f, -5.5f, 0.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans5 = glm::rotate(trans5, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // matrix * rotation_matrix
		trans5 = glm::rotate(trans5, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		trans5 = glm::scale(trans5, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 normalTrans5 = glm::transpose(glm::inverse(trans5));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans5));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans5));

		
		glActiveTexture(GL_TEXTURE0);
		GLuint solarPanelTexture = solarPanel.textures[solarPanel.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, solarPanelTexture);
		
		
		glActiveTexture(GL_TEXTURE2);
		GLuint solarPanelTexture2 = solarPanel.textures[solarPanel.materials[1].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, solarPanelTexture2);
		

		glDrawElements(GL_TRIANGLES, solarPanel.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		
		//----------------------------------------------------------------
		//draw rocket

		glBindVertexArray(rocket.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

		// transforms
		trans6 = glm::mat4(1.0f); // identity
		trans6 = glm::translate(trans6, glm::vec3(50.0f, 80.0f, -20.0f));
		//trans3 = glm::rotate(trans3, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans6 = glm::rotate(trans6, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // matrix * rotation_matrix
		trans6 = glm::rotate(trans6, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		trans6 = glm::scale(trans6, glm::vec3(0.3f, 0.3f, 0.3f));

		glm::mat4 normalTrans6 = glm::transpose(glm::inverse(trans6));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans6));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans6));

		
		glActiveTexture(GL_TEXTURE0);
		GLuint rocketTexture = rocket.textures[rocket.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, rocketTexture);

		glActiveTexture(GL_TEXTURE2);
		GLuint rocketTexture2 = rocket.textures[rocket.materials[1].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, rocketTexture2);
		

		glDrawElements(GL_TRIANGLES, rocket.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------


		//----------------------------------------------------------------
		//draw basket

		glBindVertexArray(basket.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);

			

			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

		// transforms
		trans7 = glm::mat4(1.0f); // identity
		trans7 = glm::translate(trans7, glm::vec3(-40.0f, 0.0f, 0.0f));
		trans7 = glm::rotate(trans7, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		trans7 = glm::rotate(trans7, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans7 = glm::rotate(trans7, glm::radians(rotFactor), glm::vec3(1.0f, 0.0f, 0.0f));
		trans7 = glm::scale(trans7, glm::vec3(0.03f, 0.03f, 0.03f));

		glm::mat4 normalTrans7 = glm::transpose(glm::inverse(trans7));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans7));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans7));


		glActiveTexture(GL_TEXTURE0);
		GLuint basketTexture = basket.textures[basket.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, basketTexture);


		glDrawElements(GL_TRIANGLES, basket.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------


		//----------------------------------------------------------------
		//draw lamppost

		glBindVertexArray(lamppost.vaoId);
		if (skyTicks >= 40.0f && skyTicks <= 60.0f)
		{
			//POINT LIGHT
			glUniform3f(lightPosLoc, 0.0f, 3.0f, 0.0f);

			glUniform1f(modelIdLoc, 1.1f);
		}
		else
		{
			//DIRECTIONAL LIGHT
			glUniform3f(lightDirLoc, glm::sin(lightX), glm::cos(lightX), 0.0f);


			glUniform1f(modelIdLoc, 1.2f);
		}
		glUniform1f(texTypeIdLoc, 1.0f);

		// transforms
		trans8 = glm::mat4(1.0f); // identity
		trans8 = glm::translate(trans8, glm::vec3(0.0f, 50.0f, 0.0f));
		trans8 = glm::rotate(trans8, glm::radians(rotFactor), glm::vec3(0.0f, 0.0f, 1.0f));
		trans8 = glm::rotate(trans8, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans8 = glm::rotate(trans8, glm::radians(rotFactor), glm::vec3(1.0f, 0.0f, 0.0f));
		trans8 = glm::scale(trans8, glm::vec3(0.1f, 0.1f, 0.1f));

		glm::mat4 normalTrans8 = glm::transpose(glm::inverse(trans8));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans8));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans8));


		glActiveTexture(GL_TEXTURE0);
		GLuint lamppostTexture = lamppost.textures[lamppost.materials[4].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, lamppostTexture);



		glDrawElements(GL_TRIANGLES, lamppost.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------
		

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glBindTexture(GL_TEXTURE_2D, 2);
		glBindTexture(GL_TEXTURE_2D, 3);
		


		//--- stop drawing here ---
#pragma endregion 

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}


/*FOR MULTITEXTURING
* after texture 0, add what's below:
* 
* glActiveTexture(GL_TEXTURE1);
* GLuint secondaryMap = obj.textures[obj.materials[1].diffuse_texname];
* glBindTexture(GL_TEXTURE_2D, secondaryMap);
*/
