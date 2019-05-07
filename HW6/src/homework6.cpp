#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Camera.h"
#include "shader.h"
const unsigned int SCR_WIDTH =1100;
const unsigned int SCR_HEIGHT = 600;
//camera
Camera camera = Camera(glm::vec3(8.0f, -8.0f, 10.0f));
//light
glm::vec3 lightPos(-5.0f, -5.0f, 5.0f);
//mouse
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;
float xoffset = 0.0, yoffset = 0.0;

void frame_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	xoffset = xpos - lastX;
	yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

}

void processInput(GLFWwindow *window, float cameraSpeed, float sensitivity, Camera & camera)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBack(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(cameraSpeed);
	camera.rotate(xoffset * sensitivity, yoffset * sensitivity);
	xoffset = 0;
	yoffset = 0;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const char* glsl_version = "#version 130";


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "homework6:16340164", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_resize);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float vertices[] =
	{
		-2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,

		-2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		 2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,

		-2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f,  2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,

		 2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,

		-2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,

		-2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f
	};

	
	//VAO,VBO
	unsigned int VAO,VBO;
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Î»ÖÃÊôÐÔ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//lighting VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; 
	//the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//shaders
	Shader lightShader("vlightShader.txt", "flightShader.txt");
	Shader PhongShader("vPhongShader.txt", "fPhongShader.txt");
	Shader GouraudShader("vGouraudShader.txt", "fGouraudShader.txt");
	// imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsLight();
	

	bool bonus = true;
	int radio = 0;
	float ambientStrength = 0.1;
	float diffuseStrength = 1.0;
	float specularStrength = 0.5;
	float shininess = 32.0f;

	int cubeProgram = PhongShader.ID;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window, 0.05f, 0.2f, camera);
		glfwPollEvents();
	
		// imgui
		ImGui_ImplGlfwGL3_NewFrame();
		
		ImGui::Begin("Shading style");
		ImGui::RadioButton("Phong Shading", &radio, 0);
		ImGui::RadioButton("Gouraud Shading", &radio, 1);
		ImGui::Checkbox("bonus", &bonus);
		ImGui::SliderFloat("Ambient", &ambientStrength, 0.0f, 2.0f);
		ImGui::SliderFloat("Diffuse", &diffuseStrength, 0.0f, 2.0f);
		ImGui::SliderFloat("Specular", &specularStrength, 0.0f, 2.0f);
		ImGui::SliderFloat("Shininess", &shininess, 1.0f, 64.0f);
		ImGui::End();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		if (radio == 0) {
			cubeProgram = PhongShader.ID;
		}
		else {
			cubeProgram = GouraudShader.ID;
		}

		if (bonus)
		{
			lightPos.y = sin(glfwGetTime()) * 6.0f;
			lightPos.x = cos(glfwGetTime()) * 6.0f;
			lightPos.z = sin(glfwGetTime()) * 6.0f;
		}

		glUseProgram(cubeProgram);
		glUniform3f(glGetUniformLocation(cubeProgram, "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(cubeProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(cubeProgram, "lightPos"), 1, &lightPos[0]);

		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getView();
		glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// pass to the shaders
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(glGetUniformLocation(cubeProgram, "viewPos"), 1, &camera.getCameraPos()[0]);
		glUniform1f(glGetUniformLocation(cubeProgram, "ambientStrength"), ambientStrength);
		glUniform1f(glGetUniformLocation(cubeProgram, "diffuseStrength"), diffuseStrength);
		glUniform1f(glGetUniformLocation(cubeProgram, "specularStrength"), specularStrength);
		glUniform1f(glGetUniformLocation(cubeProgram, "shininessStrength"), shininess);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(lightShader.ID);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
