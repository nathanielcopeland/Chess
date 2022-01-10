#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader_s.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
bool drawBoard(char piece, unsigned int & VAO2, int x, int y, unsigned int & texture);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int width, height, nrChannels;
//unsigned char* data = stbi_load("pawn.png", &width, &height, &nrChannels, 0);



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f / 255 * 242, 1.0f / 255 * 234, 1.0f / 255 * 211, 1.0f);\n"
"}\n\0";

const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char* fragmentShader3Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.5f, 0.2f, 0.0f, 1.0f);\n"
"}\n\0";

bool renderWindow(Board b) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//initialise glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	 /*build and compile our shader program
	 ------------------------------------
	 vertex shader*/
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShaderWhite = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderWhite, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderWhite);
	// check for shader compile errors
	glGetShaderiv(fragmentShaderWhite, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderWhite, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderBlack = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderBlack, 1, &fragmentShader2Source, NULL);
	glCompileShader(fragmentShaderBlack);
	// check for shader compile errors
	glGetShaderiv(fragmentShaderBlack, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderBlack, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &fragmentShader3Source, NULL);
	glCompileShader(fragmentShaderColor);
	// check for shader compile errors
	glGetShaderiv(fragmentShaderColor, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderColor, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	unsigned int shaderProgramWhite = glCreateProgram();
	glAttachShader(shaderProgramWhite, vertexShader);
	glAttachShader(shaderProgramWhite, fragmentShaderWhite);
	glLinkProgram(shaderProgramWhite);

	unsigned int shaderProgramBlack = glCreateProgram();
	glAttachShader(shaderProgramBlack, vertexShader);
	glAttachShader(shaderProgramBlack, fragmentShaderBlack);
	glLinkProgram(shaderProgramBlack);

	unsigned int shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShader);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);

	// check for linking errors
	glGetProgramiv(shaderProgramWhite, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramWhite, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderWhite);
	glDeleteShader(fragmentShaderBlack);
	glDeleteShader(fragmentShaderColor);


	Shader ourShader("4.1.texture.vs", "4.1.texture.fs");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float triangles[] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 

	};

	float trianglesWhite[576];

	int counter = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			trianglesWhite[counter] = -1.0f + (y  * 0.25 * 2) + (i % 2 * 0.25);
			counter++;
			trianglesWhite[counter] = 0.75f  +(i * -0.25);
			counter++;
			trianglesWhite[counter] = 0.0f;
			counter++;

			trianglesWhite[counter] = -0.75f  +(y * 0.25 * 2) + (i % 2 * 0.25);
			counter++;
			trianglesWhite[counter] = 0.75f  +(i * -0.25);
			counter++;
			trianglesWhite[counter] = 0.0f;
			counter++;

			trianglesWhite[counter] = -1.0f  +(y  * 0.25 * 2) + (i % 2 * 0.25);
			counter++;
			trianglesWhite[counter] = 1.0f   +(i * -0.25);
			counter++;
			trianglesWhite[counter] = 0.0f;
			counter++;

			//second triangle
			trianglesWhite[counter] = -1.0f  +(y * 0.25 * 2) + (i % 2 * 0.25);
			counter++;
			trianglesWhite[counter] = 1.0f  +(i * -0.25);
			counter++;
			trianglesWhite[counter] = 0.0f;
			counter++;

			trianglesWhite[counter] = -0.75f  +(y * 0.25 * 2) + (i % 2 * 0.25);
			counter++;
			trianglesWhite[counter] = 0.75f +(i * -0.25);
			counter++;
			trianglesWhite[counter] = 0.0f;
			counter++;

			trianglesWhite[counter] = -0.75f +(y * 0.25 * 2) + (i % 2 * 0.25);
			counter++;
			trianglesWhite[counter] = 1.0f +(i * -0.25);
			counter++;
			trianglesWhite[counter] = 0.0f;
			counter++;

		}
	}
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle

	};
	unsigned int VBO, VAO, EBO, VAO2;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglesWhite), trianglesWhite, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	//glGenTextures(1, &texture1);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//// set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	//// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	//unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);
	//// texture 2
	//// ---------
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//// set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// load image, create texture and generate mipmaps
	//data = stbi_load("p6.png", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	//// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	//// -------------------------------------------------------------------------------------------
	//ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	//// either set it manually like so:
	//glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	//// or set it via the texture class
	//ourShader.setInt("texture2", 1);

	int var = 20;
	int* test;
	
	test = &var;
	glfwSetWindowUserPointer(window, &b);


	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// draw our first triangle
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		glUseProgram(shaderProgramWhite);
		


		
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 576);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// render
		// ------
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		
		for (int i = 0; i < 8; i++)
		{
			for (int y = 0; y < 8; y++)
			{
				if (drawBoard(b.grid[i][y].getPiece(), VAO2, i, y, texture2)) {
					//glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, texture2);
					ourShader.use();

					glBindVertexArray(VAO2);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
		}

		
		

		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES, 0, 576);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		//game.driver();
		b.resetList();
		b.calculateAttackedSquares();
		b.testPinned();
		//b.move();
		//b.displayBoard();
	}
	

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgramWhite);
	//glDeleteProgram(shaderProgramBlack);
	//glDeleteProgram(shaderProgramColor);

	glfwTerminate();
	return 0;


}

void processInput(GLFWwindow* window)
{
	double  mX, mY;
	int xPos, yPos;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		
		glfwGetCursorPos(window, &mX, &mY);

		
		mX = mX / (SCR_HEIGHT / 8);
		mY = mY / ((SCR_WIDTH - 200) / 8);

		xPos = mX;
		yPos = mY;
			std::cout << "xPos: " << xPos << " yPos: " << yPos << " \n";
	}*/
}

bool drawBoard(char piece, unsigned int & VAO2, int x, int y, unsigned int & texture) {
	
	float triangles2[32];

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	
	float triangles3[]
	{
		-0.75 + (y * 0.25) , 1.0f + (x * -0.25), 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right 
		-0.75f + (y * 0.25), 0.75f + (x * -0.25), 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // right
		-1.0f + (y * 0.25), 0.75f + (x * -0.25), 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// top
		-1.0f + (y * 0.25), 1.0f + (x * -0.25), 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f// left 


	};

	switch (piece)
	{
	
	case 'r': {
			 
			  unsigned char* data = stbi_load("rBlack.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);
			  
			  break; }
	case 'h': {

			  unsigned char* data = stbi_load("nBlack.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'b': {

			  unsigned char* data = stbi_load("bBlack.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'q': {

			  unsigned char* data = stbi_load("qBlack.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'k': {

			  unsigned char* data = stbi_load("kBlack.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'p': {
			 
			  unsigned char* data = stbi_load("pBlack.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);
			  
			  break; }
	case 'R': {

			  unsigned char* data = stbi_load("rWhite.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'H': {

			  unsigned char* data = stbi_load("nWhite.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'B': {

			  unsigned char* data = stbi_load("bWhite.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'Q': {

			  unsigned char* data = stbi_load("qWhite.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'K': {

			  unsigned char* data = stbi_load("kWhite.png", &width, &height, &nrChannels, 0);
			  if (data)
			  {
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);
			  }
			  else
			  {
				  std::cout << "Failed to load texture" << std::endl;
			  }
			  stbi_image_free(data);

			  break; }
	case 'P': {
		unsigned char* data = stbi_load("pWhite.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		break; }
	default: return false;
		break;
	}

	std::copy(triangles3, triangles3 + 32, triangles2);
	//float triangles2[]
	//{
	//	-1.0f + (y * 0.25) , 0.75f + (x * -0.25), 0.0f,  // left 
	//	-0.75f + (y * 0.25), 0.75f + (x * -0.25), 0.0f,  // right
	//	-1.0f + (y * 0.25), 1.0f + (x * -0.25), 0.0f,  // top

	//	-1.0f + (y * 0.25), 1.0f + (x * -0.25), 0.0f,  // left 
	//	-0.75f + (y * 0.25), 0.75f + (x * -0.25), 0.0f,  // right
	//	-0.75f + (y * 0.25), 1.0f + (x * -0.25), 0.0f,  // top

	//};
	//std::copy(triangles3, triangles3 + 18, triangles2);

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles2), triangles2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //uncomment for wireframe

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	VAO2 = VAO;

	glBindVertexArray(0);
	return true;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




 void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	 double  mX, mY;
	 int xPos, yPos, xPos2, yPos2;

	 glfwGetWindowUserPointer(window);
	 Board* b = reinterpret_cast<Board*>(glfwGetWindowUserPointer(window));

	 if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		 glfwGetCursorPos(window, &mY, &mX);

		 mX = mX / ((SCR_HEIGHT - 200) / 8);
		 mY = mY / (SCR_WIDTH / 8);

		 xPos = mX;
		 yPos = mY;
		 

		 if (b->isFirstPress(xPos, yPos)) {
			 

			 b->setFirstMove(xPos, yPos);
		 }
		 else {


			 b->setSecond(xPos, yPos);

			 b->move2();
		 }

	 };

	 if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {

		 glfwGetCursorPos(window, &mY, &mX);


		 
		 //b->displayBoard();
	 };
	
}
