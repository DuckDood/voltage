#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "voltage.h"
#include <stb_image.h>

#if USING_IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>
#endif


#define INIT_SCR_WIDTH 1366
#define INIT_SCR_HEIGHT 768


/*void getMouseDelta(float mouseX, float mouseY, float *mouseReturnX, float *mouseReturnY) {
	static double lastX = 0;
	static double lastY = 0;

	*mouseReturnX = mouseX - lastX;
	*mouseReturnY = mouseY - lastY;

	lastX = mouseX;
	lastY = mouseY;
}*/


#if USING_IMGUI
void Model_ImGui_Window(Model *model, std::string label) {
	ImGui::Begin(label.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::Text("Position");
	ImGui::DragFloat(("x##Pos##" + label).c_str(), &model->position.x, 0.01);
	ImGui::DragFloat(("y##Pos##" + label).c_str(), &model->position.y, 0.01);
	ImGui::DragFloat(("z##Pos##" + label).c_str(), &model->position.z, 0.01);
	ImGui::Text("Scale");
	ImGui::DragFloat(("x##Scale##" + label).c_str(), &model->scale.x, 0.01);
	ImGui::DragFloat(("y##Scale##" + label).c_str(), &model->scale.y, 0.01);
	ImGui::DragFloat(("z##Scale##" + label).c_str(), &model->scale.z, 0.01);

	ImGui::Text("Rotation");
	ImGui::DragFloat(("x##Rotation##" + label).c_str(), &model->yaw, 0.003);
	ImGui::DragFloat(("y##Rotation##" + label).c_str(), &model->pitch, 0.003);
	ImGui::DragFloat(("z##Rotation##" + label).c_str(), &model->roll, 0.003);

	ImGui::Text("Material");

	ImGui::ColorPicker4(("diffuseColor##" + label).c_str(), glm::value_ptr(model->mat.diffuseColor));
	ImGui::ColorPicker4(("specularColor##" + label).c_str(), glm::value_ptr(model->mat.specularColor));

	ImGui::DragFloat(("Shininess##" + label).c_str(), &model->mat.shininess, 1);
	ImGui::Checkbox(("Use Lighting##" + label).c_str(), &model->mat.useLighting);
	ImGui::Checkbox(("Use DiffuseTex##" + label).c_str(), &model->mat.useDiffuseTex);
	ImGui::Checkbox(("Use SpecTex##" + label).c_str(), &model->mat.useSpecularTex);
	ImGui::Checkbox(("Use Normal Map##" + label).c_str(), &model->mat.useNormalMap);
	ImGui::End();
}
void Light_ImGui_Window(Light *light, std::string label) {
	ImGui::Begin(label.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::ColorPicker3(("diffuse##" + label).c_str(), glm::value_ptr(light->diffuse));
	ImGui::ColorPicker3(("specular##" + label).c_str(), glm::value_ptr(light->specular));
	ImGui::ColorPicker3(("ambient##" + label).c_str(), glm::value_ptr(light->ambient));
	ImGui::DragFloat3(("position##" + label).c_str(), glm::value_ptr(light->position), 0.05);
	ImGui::DragFloat3(("direction##" + label).c_str(), glm::value_ptr(light->direction), 0.05);

	ImGui::DragFloat(("incutoff##" + label).c_str(), &light->innerCutOff, 0.01);
	ImGui::DragFloat(("outcutoff##" + label).c_str(), &light->outerCutOff, 0.01);

	ImGui::Text("Attenuation");
	ImGui::DragFloat(("constant##" + label).c_str(), &light->constant, 0.01);
	ImGui::DragFloat(("linear##" + label).c_str(), &light->linear, 0.01);
	ImGui::DragFloat(("quadratic##" + label).c_str(), &light->quadratic, 0.01);

	ImGui::Text("Type");
	ImGui::RadioButton(("directional##" + label).c_str(), &light->type, 0);
	ImGui::SameLine();
	ImGui::RadioButton(("point##" + label).c_str(), &light->type, 1);
	ImGui::SameLine();
	ImGui::RadioButton(("spotlight##" + label).c_str(), &light->type, 2);

	ImGui::End();
}
#endif

float quadVertices[] = {  
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};


int main() {
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Failed to intialise SDL: " << SDL_GetError() << std::endl;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window *window = SDL_CreateWindow("voltage", INIT_SCR_WIDTH, INIT_SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(window == NULL) {
		std::cerr << "Failed to create SDL3 window:" << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_GLContext sdlGlContext = SDL_GL_CreateContext(window);
	if(!sdlGlContext) {
		std::cerr << "Failed to create SDL3 OpenGL context:" << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	SDL_GL_MakeCurrent(window, sdlGlContext);
	SDL_SetWindowRelativeMouseMode(window, true);
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		std::cerr << "Failed to init GLEW: " << glewGetErrorString(err) << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	glViewport(0,0,INIT_SCR_WIDTH,INIT_SCR_HEIGHT);
	bool capture = true;
	#if USING_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	
	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(window, sdlGlContext);
	ImGui_ImplOpenGL3_Init();
	#endif

	unsigned int qVAO;
	unsigned int qVBO;
	glGenVertexArrays(1, &qVAO);
	glBindVertexArray(qVAO);
	glGenBuffers(1, &qVBO);
	glBindBuffer(GL_ARRAY_BUFFER, qVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width, height, channelCount;
	unsigned char *data = stbi_load("resources/textures/brickwall.jpg", &width, &height, &channelCount, 0);
	unsigned int texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(data);

	data = stbi_load("resources/textures/brickwall.jpg", &width, &height, &channelCount, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(data);

	data = stbi_load("resources/textures/nmap.jpg", &width, &height, &channelCount, 0);
	unsigned int texture3;
	glGenTextures(1, &texture3);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(data);

	Framebuffer mainFramebuffer(INIT_SCR_WIDTH, INIT_SCR_HEIGHT);

	std::vector<Hitbox*> hitboxes;

	Material material;
	material.diffuseTex = texture;
	material.specularTex = texture2;
	material.normal = texture3;
	material.shininess = 32;
	Object model(load3dCache("resources/cache/cube.vtcache"), material);
	model.position.z = -2;
	model.position.y = -1.8;
	model.cullType = BACK;

	Object floor(load3dCache("resources/cache/cube.vtcache"), material);
	hitboxes.push_back(&model.hitbox);
	hitboxes.push_back(&floor.hitbox);

	model.hitbox.hitboxes = &hitboxes;
	floor.hitbox.hitboxes = &hitboxes;

	floor.position.y = -2;
	floor.scale.x = 100;
	floor.scale.x = 100;
	floor.scale.z = 100;

	Camera cam;

	glm::mat4 perspective = glm::perspective(cam.fov, (float)INIT_SCR_WIDTH/(float)INIT_SCR_HEIGHT, 0.1f, 100.f);

	Shader program("shaders/vertex.glsl", "shaders/fragment.glsl");
	Shader screen("shaders/screenVert.glsl", "shaders/screenFrag.glsl");
	#if HITBOX_VIEW
	Shader hitShader("shaders/hitVert.glsl", "shaders/hitFrag.glsl");
	#endif

	Hitbox cameraBound;
	cameraBound.id = 1;
	hitboxes.push_back(&cameraBound);
	cameraBound.hitboxes = &hitboxes;
	cameraBound.scale = glm::vec3(0.2,0.35,0.2);

	
	std::vector<Light*> lights;
	
	Light light;
	light.type = 1;
	light.ambient = glm::vec3(0.2,0.2,0.2);
	light.diffuse = glm::vec3(1.5, 1.5 ,1.5);
	light.specular = glm::vec3(1.f, 1.f, 1.f);
	light.position = glm::vec3(0.f, 1.f, 0.3f);
	light.direction = glm::vec3(0.f, 1.f, 0.0f);
	light.constant = 1.f;
	light.linear = 0.09f;
	light.quadratic = 0.032f;
	light.innerCutOff = glm::cos(glm::radians(12.5f));
	light.outerCutOff = glm::cos(glm::radians(12.5f));
	Light sun;
	sun.type = 0;
	sun.ambient = glm::vec3(0.0,0.0,0.0);
	sun.diffuse = glm::vec3(0.2, 0.2 ,0.2);
	sun.specular = glm::vec3(0.4f, 0.4f, 0.4f);
	sun.direction = glm::vec3(0.f, 1.f, 0.0f);
	
	lights.push_back(&light);

	float mouseX;
	float mouseY;

	int winX = INIT_SCR_WIDTH;
	int winY = INIT_SCR_HEIGHT;

	glm::vec3 velocity(0,0,0);

	#if HITBOX_VIEW
	HitboxRenderer hitRenderer;
	#endif

	double prevTime = SDL_GetTicks()/1000.;
	int frameCount = 0;
	bool screenShader = true;

	bool running = true;
	SDL_Event event;
	while(running) {
		#if USING_IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplSDL3_ProcessEvent(&event);
		ImGui::NewFrame();
		#endif

		while(SDL_PollEvent(&event)) {
			#if USING_IMGUI
			ImGui_ImplSDL3_ProcessEvent(&event);
			#endif
			switch(event.type) {
				case SDL_EVENT_KEY_DOWN:
					switch(event.key.scancode) {
						case SDL_SCANCODE_ESCAPE:
							capture = !capture;
							break;

						default:
							break;
					}
					break;

				case SDL_EVENT_QUIT:
					running = false;
					break;

				case SDL_EVENT_MOUSE_MOTION:
					mouseX = event.motion.xrel;
					mouseY = event.motion.yrel;
					if(capture) {
						cam.yaw -= mouseX/300;
						cam.pitch -= mouseY/300;
					}
					break;

				case SDL_EVENT_WINDOW_RESIZED:
					winX = event.window.data1;
					winY = event.window.data2;
					glViewport(0, 0, winX, winY);
					break;

				default:
					break;
			}
		}
		const bool *keyStates = SDL_GetKeyboardState(nullptr);
		double currentTime = SDL_GetTicks()/1000.;
		frameCount++;
		if((currentTime - prevTime) >= 1) {
			std::cout << frameCount << std::endl;
			frameCount = 0;
			prevTime = currentTime;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		


		if(capture) {
			SDL_SetWindowRelativeMouseMode(window, true);
		} else {
			SDL_SetWindowRelativeMouseMode(window, false);
		}

		#if USING_IMGUI
		Model_ImGui_Window(&model, "Model");
		Model_ImGui_Window(&floor, "Model2");

		//Light_ImGui_Window(&sun, "sun");
		Light_ImGui_Window(&light, "light");
	

		ImGui::Begin("Screen");
		ImGui::Checkbox("Use Screen Shader", &screenShader);
		ImGui::End();
		#endif

		if(screenShader) {
		mainFramebuffer.Start(winX, winY);
		}
		glUseProgram(program.programID);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		if(keyStates[SDL_SCANCODE_S]) {
			//velocity.z += cos(cam.yaw)*0.03*cos(cam.pitch);
			//velocity.x += sin(cam.yaw)*0.03*cos(cam.pitch);

			//velocity.y -= sin(cam.pitch)*0.03;
			
			velocity.z += cos(cam.yaw)*0.03;
			velocity.x += sin(cam.yaw)*0.03;
		}
		if(keyStates[SDL_SCANCODE_W]) {
			//velocity.z -= cos(cam.yaw)*0.03*cos(cam.pitch);
			//velocity.x -= sin(cam.yaw)*0.03*cos(cam.pitch);

			//velocity.y += sin(cam.pitch)*0.03;
			
			velocity.z -= cos(cam.yaw)*0.03;
			velocity.x -= sin(cam.yaw)*0.03;
		}
		if(keyStates[SDL_SCANCODE_A]) {
			velocity.z += sin(cam.yaw)*0.02;
			velocity.x -= cos(cam.yaw)*0.02;
		}
		if(keyStates[SDL_SCANCODE_D]) {
			velocity.z -= sin(cam.yaw)*0.02;
			velocity.x += cos(cam.yaw)*0.02;
		}
		velocity.y -= 0.006;

		cam.position.x += velocity.x;
		cameraBound.position = cam.position;
		cameraBound.Update();
		if(cameraBound.isOverlapped()) {
			cam.position.x -= velocity.x;	
			velocity.x = 0;
		}
		cameraBound.position = cam.position;


		cam.position.y += velocity.y;
		cameraBound.position = cam.position;
		cameraBound.Update();
		if(cameraBound.isOverlapped()) {
			cam.position.y -= velocity.y;	
			velocity.y = 0;
		}
		cameraBound.position = cam.position;

		cam.position.z += velocity.z;
		cameraBound.position = cam.position;
		cameraBound.Update();
		if(cameraBound.isOverlapped()) {
			cam.position.z -= velocity.z;	
			velocity.z = 0;
		}
		cameraBound.position = cam.position;

		velocity.x *= 0.7;
		velocity.z *= 0.7;
		if(velocity.y < -0.5) {
			velocity.y = -0.5;
		}
		cameraBound.Update();

		Hitbox cameraFloorBound = cameraBound;
		cameraFloorBound.scale.z *= 0.9;
		cameraFloorBound.scale.x *= 0.9;
		cameraFloorBound.position.y -=0.1;
		cameraFloorBound.Update();
		if(cameraFloorBound.isOverlapped()) {
			if(keyStates[SDL_SCANCODE_SPACE]) {
				velocity.y = 0.1;
			}
		}

		if(keyStates[SDL_SCANCODE_COMMA]) {
			cam.fov += glm::radians(.5);
		}
		if(keyStates[SDL_SCANCODE_PERIOD]) {
			cam.fov -= glm::radians(.5);
		}

		//if(capture) {
		//	cam.yaw -= mouseX/300;
		//	cam.pitch -= mouseY/300;
		//}
		if(cam.pitch > 1.5708) cam.pitch = 1.5708;
		if(cam.pitch < -1.5708) cam.pitch = -1.5708;

		setSceneLights(program, lights);


		cam.UpdateRotation();

		model.UpdateHitbox();
		floor.UpdateHitbox();


		perspective = glm::perspective(cam.fov, (float)winX/(float)winY, 0.1f, 100.f);

		glUniformMatrix4fv(glGetUniformLocation(program.programID, "viewMat"), 1, GL_FALSE, glm::value_ptr(cam.invTransformMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program.programID, "perspMat"), 1, GL_FALSE, glm::value_ptr(perspective));
		glUniform3fv(glGetUniformLocation(program.programID, "viewPos"), 1, glm::value_ptr(cam.position));

		RenderModel(floor, program);
		RenderModel(model, program);


		if(screenShader) {
		mainFramebuffer.End();
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainFramebuffer.colorBuffer);
		glUseProgram(screen.programID);
		
		glBindVertexArray(qVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);
		}

		#if HITBOX_VIEW
		glUseProgram(hitShader.programID);

		glUniformMatrix4fv(glGetUniformLocation(hitShader.programID, "viewMat"), 1, GL_FALSE, glm::value_ptr(cam.invTransformMatrix));
		glUniformMatrix4fv(glGetUniformLocation(hitShader.programID, "perspMat"), 1, GL_FALSE, glm::value_ptr(perspective));
		hitShader.setUniformVec3Floats("color", 0., 1., 0.);

		glDisable(GL_DEPTH_TEST);
		hitRenderer.Update(&model.hitbox);
		glBindVertexArray(hitRenderer.VAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		hitRenderer.Update(&floor.hitbox);
		glBindVertexArray(hitRenderer.VAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		hitRenderer.Update(&cameraBound);
		glBindVertexArray(hitRenderer.VAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		hitShader.setUniformVec3Floats("color", 1., 0., 0.);
		hitRenderer.Update(&cameraFloorBound);
		glBindVertexArray(hitRenderer.VAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		glEnable(GL_DEPTH_TEST);
		#endif

		#if USING_IMGUI	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#endif

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DestroyContext(sdlGlContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
