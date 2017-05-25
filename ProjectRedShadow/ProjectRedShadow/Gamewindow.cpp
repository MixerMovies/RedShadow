#include "Gamewindow.h"
#include "ObjModel.h"
#include "SoundTest.h"

SoundTest test = SoundTest();
GLuint fboTextureId;
GLuint fboId;
Space* shaderPreviewContent;

Gamewindow::Gamewindow(Space* space)
{
	shaderPreviewContent = space;
	ObjModel* Spaceship = new ObjModel("models/ship/shipA_OBJ.obj");
	ObjModel* book = new ObjModel("models/book/object.obj");
	ObjModel* cube = new ObjModel("models/cube/cube-textures.obj");
	ObjModel* dragon = new ObjModel("models/dragon/Blue-Eyes White Dragon.obj");
	//ObjModel* library = new ObjModel("models/Library/library.obj");
	shaderPreviewContent->previewModels.push_back(Spaceship);
	shaderPreviewContent->previewModels.push_back(book);
	shaderPreviewContent->previewModels.push_back(cube);
	shaderPreviewContent->previewModels.push_back(dragon);
	//shaderPreviewContent->previewModels.push_back(library);
	
	shaderPreviewContent->music = test.LoadSound("Sound/OdeToJoy(Remix).wav");
	shaderPreviewContent->music->Play();

	//basic shaders
	Shader* shader = new Shader("Shaders/texture.vs", "Shaders/texture.fs", "Shaders/standard.gs");
	shaders.push_back(shader);
	Shader* shader2 = new Shader("Shaders/simple.vs", "Shaders/simple.fs", "Shaders/standard.gs");
	shaders.push_back(shader2);
	Shader* shader3 = new Shader("Shaders/specular.vs", "Shaders/specular.fs", "Shaders/standard.gs");
	shaders.push_back(shader3);
	Shader* shader4 = new Shader("Shaders/textureanim.vs", "Shaders/textureanim.fs", "Shaders/standard.gs");
	shaders.push_back(shader4);
	Shader* shader5 = new Shader("Shaders/tvgrain.vs", "Shaders/tvgrain.fs", "Shaders/standard.gs");
	shaders.push_back(shader5);
	Shader* shader6 = new Shader("Shaders/procedural.vs", "Shaders/procedural.fs", "Shaders/standard.gs");
	shaders.push_back(shader6);
	Shader* shader7 = new Shader("Shaders/toon.vs", "Shaders/toon.fs", "Shaders/standard.gs");
	shaders.push_back(shader7);
	Shader* shader8 = new Shader("Shaders/rainbow.vs", "Shaders/rainbow.fs", "Shaders/standard.gs");
	shaders.push_back(shader8);
	//post processing shaders
	Shader* pShader1 = new Shader("Shaders/blur.vs", "Shaders/blur.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader1);
	Shader* pShader2 = new Shader("Shaders/laplace.vs", "Shaders/laplace.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader2);
	Shader* pShader3 = new Shader("Shaders/sharpening.vs", "Shaders/sharpening.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader3);
	Shader* pShader4 = new Shader("Shaders/filmgrain.vs", "Shaders/filmgrain.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader4);
	Shader* pShader5 = new Shader("Shaders/blackandwhite.vs", "Shaders/blackandwhite.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader5);
	Shader* pShader6 = new Shader("Shaders/negative.vs", "Shaders/negative.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader6);
	Shader* pShader7 = new Shader("Shaders/screenwarp.vs", "Shaders/screenwarp.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader7);
	Shader* pShader8 = new Shader("Shaders/colourwarp.vs", "Shaders/colourwarp.fs", "Shaders/standard.gs");
	postProcessingShaders.push_back(pShader8);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnableVertexAttribArray(0);							// positie
	glEnableVertexAttribArray(1);							// texcoord
	glEnableVertexAttribArray(2);							// normal
	glEnableVertexAttribArray(3);							// tangent

	glGenTextures(1, &fboTextureId);
	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextureId, 0);

	GLuint rboId;
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2048, 2048);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Gamewindow::~Gamewindow()
{
}

void Gamewindow::Setup()
{
	if (postProcessingEnabled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glViewport(0, 0, 2048, 2048);
	}
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(70.0f, screenSize.x / (float) screenSize.y, 0.01f, 1000.0f);		//begin met een perspective matrix
	glm::mat4 view = glm::lookAt(shaderPreviewContent->player.position, shaderPreviewContent->player.rotation, glm::vec3(0, 1, 0));					//vermenigvuldig met een lookat
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 0, -1));													//of verplaats de camera gewoon naar achter
	model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));											//roteer het object een beetje
	
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));										//roteer het object een beetje

	shaders[currentshader]->use();

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glUniformMatrix4fv(shaders[currentshader]->getUniformLocation("modelMatrix"), 1, 0, glm::value_ptr(model));
	glUniformMatrix4fv(shaders[currentshader]->getUniformLocation("viewMatrix"), 1, 0, glm::value_ptr(view));
	glUniformMatrix4fv(shaders[currentshader]->getUniformLocation("projectionMatrix"), 1, 0, glm::value_ptr(projection));
	glUniformMatrix3fv(shaders[currentshader]->getUniformLocation("normalMatrix"), 1, 0, glm::value_ptr(normalMatrix));

	glUniform1f(shaders[currentshader]->getUniformLocation("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniform1i(shaders[currentshader]->getUniformLocation("s_texture"), 0);
	glUniform1f(shaders[currentshader]->getUniformLocation("ambient"), 0.2f);
	glUniform1f(shaders[currentshader]->getUniformLocation("shininess"), 10);
	glUniform1f(shaders[currentshader]->getUniformLocation("alpha"), 0.5f);
	/*glRotatef(Space::Instance()->player.rotation[0], 1, 0, 0);
	glRotatef(Space::Instance()->player.rotation[1], 0, 1, 0);
	glRotatef(Space::Instance()->player.rotation[2], 0, 0, 1);
	glTranslatef(Space::Instance()->player.position[0], Space::Instance()->player.position[1], Space::Instance()->player.position[2]);*/
}

void Gamewindow::Display()
{

	shaderPreviewContent->getCurrentModel()->draw();

	//Space::Instance()->building->draw();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (postProcessingEnabled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenSize.x, screenSize.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::vector<glm::vec2> verts;
		verts.push_back(glm::vec2(-1, -1));
		verts.push_back(glm::vec2(1, -1));
		verts.push_back(glm::vec2(1, 1));
		verts.push_back(glm::vec2(-1, 1));

		postProcessingShaders[currentPostShader]->use();
		glUniform1i(postProcessingShaders[currentPostShader]->getUniformLocation("s_texture"), 0);
		glUniform1f(postProcessingShaders[currentPostShader]->getUniformLocation("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

		glBindTexture(GL_TEXTURE_2D, fboTextureId);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
		glDrawArrays(GL_QUADS, 0, verts.size());
	}

	glutSwapBuffers();
}

void Gamewindow::NextShader()
{
	currentshader = (currentshader + 1) % shaders.size();
}

void Gamewindow::PreviousShader()
{
	currentshader = (currentshader + shaders.size() - 1) % shaders.size();
}

void Gamewindow::NextPostShader()
{
	currentPostShader = (currentPostShader + 1) % postProcessingShaders.size();
}

void Gamewindow::PreviousPostShader()
{
	currentPostShader = (currentPostShader + postProcessingShaders.size() - 1) % postProcessingShaders.size();
}
