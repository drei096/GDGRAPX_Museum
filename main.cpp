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
	window = glfwCreateWindow(1024, 768, "Gaurana_Aldrey", NULL, NULL);
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

	ObjData trees;
	LoadObjFile(&trees, "trees/Tree/Tree.obj");
	GLfloat treesOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&trees,
		1.0f,
		treesOffsets,
		"trees"
	);

	/*
	ObjData sun;
	LoadObjFile(&sun, "earth/earth.obj");
	GLfloat sunOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&sun,
		1.0f,
		sunOffsets,
		"sun"
	);

	ObjData moon;
	LoadObjFile(&moon, "earth/earth.obj");
	GLfloat moonOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&moon,
		1.0f,
		moonOffsets,
		"moon"
	);

	ObjData earth;
	LoadObjFile(&earth, "earth/earth.obj");
	GLfloat earthOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&earth,
		1.0f,
		earthOffsets,
		"earth"
	);
	*/


	std::vector<std::string> faces
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};
	SkyBoxData skybox = loadSkyBox("Assets/skybox", faces);
#pragma endregion

#pragma region Shader Loading

	//load skybox shader program
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");

	//load shader program
	GLuint shaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/phong_directional_fragment.shader");
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

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//setup light shading
	//POINT LIGHT
	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	//DIRECTIONAL LIGHT
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	//glUniform3f(lightPosLoc, 3.0f, 0.0f, 0.0f);
	glUniform3f(lightDirLoc, -2.0f, 1.0f, 0.0f);

	//flag for shading
	GLuint modelIdLoc = glGetUniformLocation(shaderProgram, "u_model_id");
	glUniform1f(modelIdLoc, 1.0f);


#pragma endregion

	// set bg color to green
	glClearColor(0.0f, 0.0f, 0.15f, 0.0f);

	// var for rotations
	float xFactor = 0.0f;
	float xSpeed = 1.0f;
	float zFactor = 0.0f;
	float zSpeed = 1.0f;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;
	float rotFactor = 0.0f;
	float rotSpeed = 10.0f;
	float eyeY = 0.0f;
	float eyeSpeed = 1.5f;
	

	//depth testing
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // set the depth test function

	//face culling
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // set which face to cull
	//glFrontFace(GL_CCW); // set the front face orientation

	//std::cout << moon.textures[0] << std::endl;

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
		// Orthopgraphic projection but make units same as pixels. origin is lower left of window
		// projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 10.0f); // when using this scale objects really high at pixel unity size

		// Orthographic with stretching
		//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);

		// Orthographic with corection for stretching, resize window to see difference with previous example
		//projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 10.0f);

		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 100.0f),

			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		

#pragma endregion

#pragma region View

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::mat4 view = glm::lookAt(
			cameraPos,  //EYE
			//glm::vec3(0.5f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),  //CENTER
			//trans[3][0], trans[3][1], trans[3][2]
			glm::vec3(0.0f, 1.0f, 0.0f)  //UP
		);
		glUniform3f(cameraPosLoc,cameraPos.x, cameraPos.y, cameraPos.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		//-------------------------------------------------------------
#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw

		//--------------------------------------------
		//DRAWING SKYBOX
		//DrawSkybox(skybox, skyboxShaderProgram, view, projection);
		glUseProgram(shaderProgram);


		//--------------------------------------------------------
		//DELTA TIME

		// incerement rotation by deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		rotFactor += deltaTime * rotSpeed;


		
		xFactor += deltaTime * xSpeed;
		//zFactor -= deltaTime * zSpeed;
		if (xFactor > 90.0f)
		{
			xSpeed = -3.0f;
		}
		else if (xFactor < -90.0f)
		{
			xSpeed = 3.0f;
		}
		
		/*
		if (zFactor < -8.0f)
		{
			zSpeed = 4.0f;
		}
		else if (zFactor > 4.0f)
		{
			zSpeed = -4.0f;
		}
		*/

		prevTime = currentTime;

		



		//----------------------------------------------------------
		//draw TREE
		glUniform1f(modelIdLoc, 1.0f);
		glBindVertexArray(trees.vaoId);

		// transforms
		trans = glm::mat4(1.0f); // identity
		//trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans = glm::translate(trans, glm::vec3(3.0f, 0.0f, -7.0f)); // matrix * translate_matrix
		trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
		
		//send to shader
		glm::mat4 normalTrans = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		

		glActiveTexture(GL_TEXTURE0);
		GLuint treesTexture = trees.textures[trees.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, treesTexture);
	

		//drawbackpack
		glDrawElements(GL_TRIANGLES, trees.numFaces, GL_UNSIGNED_INT, (void*)0);

		//---------------------------------------------------------------------

		/*
		//----------------------------------------------------------------
		//draw MOON
		glUniform1f(modelIdLoc, 1.1f);
		glBindVertexArray(moon.vaoId);

		// transforms
		trans1 = glm::mat4(1.0f); // identity
		//trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix
		trans1 = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f));
		trans1 = glm::translate(trans1, glm::vec3(-0.4f, 0.0f, -7.0f)); // matrix * translate_matrix
		trans1 = glm::scale(trans1, glm::vec3(0.3f, 0.3f, 0.3f));
		
		glm::mat4 normalTrans1 = glm::transpose(glm::inverse(trans1));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans1));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans1));

		GLuint moonTexture = moon.textures[moon.materials[2].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, moonTexture);

		glDrawElements(GL_TRIANGLES, moon.numFaces, GL_UNSIGNED_INT, (void*)0);
		//------------------------------------------------------------------

		
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