﻿#define _USE_MATH_DEFINES

#include "Gamewindow.h"
#include "../ProjectRedShadow/SoundTest.h"
#include "../ProjectRedShadow/FileLoader.h"

#include <math.h>

SoundTest test = SoundTest();
GLuint fboTextureId;
GLuint fboId;

GLuint eyeLeftId;
GLuint eyeLeftDepthId;
GLuint eyeLeftTextureId;
GLuint eyeLeftResolveTextureId;
GLuint eyeLeftResolveFramebufferId;

GLuint eyeRightId;
GLuint eyeRightDepthId;
GLuint eyeRightTextureId;
GLuint eyeRightResolveTextureId;
GLuint eyeRightResolveFramebufferId;

vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
glm::mat4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

glm::mat4 m_mat4HMDPose;

float currentTime = 0;

Space* city;

Gamewindow::Gamewindow(Space* space, VRUtil* vr)
{
	vrUtil = vr;

	city = space;
	FileLoader::loadMap("TowerCity", space);
	
	city->music = test.LoadSound("Sound/OdeToJoy(Remix).wav");
	//city->music->Play();

	shaders.push_back(nullptr);		//added null pointer shader to test objects with shaders (it will skip this one and use the shaders of the object itself instead)
	//basic shaders
	Shader* shader = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader);
	Shader* shader2 = new Shader("Shaders/Standard/simple.vs", "Shaders/Standard/simple.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader2);
	Shader* shader3 = new Shader("Shaders/Standard/specular.vs", "Shaders/Standard/specular.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader3);
	Shader* shader4 = new Shader("Shaders/Standard/textureanim.vs", "Shaders/Standard/textureanim.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader4);
	Shader* shader5 = new Shader("Shaders/Standard/tvgrain.vs", "Shaders/Standard/tvgrain.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader5);
	Shader* shader6 = new Shader("Shaders/Standard/procedural.vs", "Shaders/Standard/procedural.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader6);
	Shader* shader7 = new Shader("Shaders/Standard/toon.vs", "Shaders/Standard/toon.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader7);
	//Shader* shader8 = new Shader("Shaders/rainbow.vs", "Shaders/rainbow.fs", "Shaders/standard.gs");
	//shaders.push_back(shader8);
	Shader* shader9 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/explosion.gs");
	shaders.push_back(shader9);
	Shader* shader10 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/implosion.gs");
	shaders.push_back(shader10);
	//post processing shaders
	Shader* pShader1 = new Shader("Shaders/Post/blur.vs", "Shaders/Post/blur.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader1);
	Shader* pShader2 = new Shader("Shaders/Post/laplace.vs", "Shaders/Post/laplace.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader2);
	Shader* pShader3 = new Shader("Shaders/Post/sharpening.vs", "Shaders/Post/sharpening.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader3);
	Shader* pShader4 = new Shader("Shaders/Post/filmgrain.vs", "Shaders/Post/filmgrain.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader4);
	Shader* pShader5 = new Shader("Shaders/Post/blackandwhite.vs", "Shaders/Post/blackandwhite.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader5);
	Shader* pShader6 = new Shader("Shaders/Post/negative.vs", "Shaders/Post/negative.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader6);
	Shader* pShader7 = new Shader("Shaders/Post/screenwarp.vs", "Shaders/Post/screenwarp.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader7);
	Shader* pShader8 = new Shader("Shaders/Post/colourwarp.vs", "Shaders/Post/colourwarp.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader8);
	Shader* pShader9 = new Shader("Shaders/Post/scope.vs", "Shaders/Post/scope.fs", "Shaders/Geometry/standardpost.gs");
	postProcessingShaders.push_back(pShader9);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnableVertexAttribArray(0);							// position
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

	initVRShaders();
}

Gamewindow::~Gamewindow()
{
}

void Gamewindow::initVRShaders()
{
	//Left eye VR

	glGenFramebuffers(1, &eyeLeftId);
	glBindFramebuffer(GL_FRAMEBUFFER, eyeLeftId);

	glGenRenderbuffers(1, &eyeLeftDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, eyeLeftDepthId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, 1000, 1000);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, eyeLeftDepthId);

	glGenTextures(1, &eyeLeftTextureId);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, eyeLeftTextureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, 1000, 1000, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, eyeLeftTextureId, 0);

	glGenFramebuffers(1, &eyeLeftResolveFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, eyeLeftResolveFramebufferId);

	glGenTextures(1, &eyeLeftResolveTextureId);
	glBindTexture(GL_TEXTURE_2D, eyeLeftResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1000, 1000, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, eyeLeftResolveTextureId, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Right Eye VR

	glGenFramebuffers(1, &eyeRightId);
	glBindFramebuffer(GL_FRAMEBUFFER, eyeRightId);

	glGenRenderbuffers(1, &eyeRightDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, eyeRightDepthId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, 1000, 1000);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, eyeRightDepthId);

	glGenTextures(1, &eyeRightTextureId);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, eyeRightTextureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, 1000, 1000, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, eyeRightTextureId, 0);

	glGenFramebuffers(1, &eyeRightResolveFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, eyeRightResolveFramebufferId);

	glGenTextures(1, &eyeRightResolveTextureId);
	glBindTexture(GL_TEXTURE_2D, eyeRightResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1000, 1000, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, eyeRightResolveTextureId, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void Gamewindow::UpdateHMDMatrixPose()
{
	if (!vrUtil->HasVRSystem())
		return;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	int m_iValidPoseCount = 0;
	std::string m_strPoseClasses = "";
	for (unsigned int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_iValidPoseCount++;
			m_rmat4DevicePose[nDevice] = vrUtil->ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (vrUtil->GetVRSystem()->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		m_mat4HMDPose = inverse(m_mat4HMDPose);
	}

	for (Gamewindow::EHand eHand = Gamewindow::Left; eHand <= Gamewindow::Right; ((int&)eHand)++)
	{
		vr::InputPoseActionData_t poseData;
		vr::EVRInputError error = vr::VRInput()->GetPoseActionDataRelativeToNow(m_rHand[eHand].m_actionPose, vr::TrackingUniverseStanding, 0, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle);
		if ( error != vr::VRInputError_None
			|| !poseData.bActive || !poseData.pose.bPoseIsValid)
		{
			m_rHand[eHand].m_bShowController = false;
		}
		else
		{
			m_rHand[eHand].m_rmat4Pose = vrUtil->ConvertSteamVRMatrixToMatrix4(poseData.pose.mDeviceToAbsoluteTracking);
			glm::vec3 position = glm::vec3(m_rHand[eHand].m_rmat4Pose[3][0], m_rHand[eHand].m_rmat4Pose[3][1], m_rHand[eHand].m_rmat4Pose[3][2]);
			city->teleporters[eHand].setCurrentLocation(position);
			glm::mat4 rotated = glm::rotate(m_rHand[eHand].m_rmat4Pose, (float)M_PI_2, glm::vec3(1, 0, 0));
			glm::vec3 forward = normalize(glm::vec3(rotated[2]));
			city->teleporters[eHand].setCurrentRotation(forward);

			vr::InputOriginInfo_t originInfo;
			if (vr::VRInput()->GetOriginTrackedDeviceInfo(poseData.activeOrigin, &originInfo, sizeof(originInfo)) == vr::VRInputError_None
				&& originInfo.trackedDeviceIndex != vr::k_unTrackedDeviceIndexInvalid && m_rHand[eHand].m_pRenderModel == nullptr)
			{
				std::string sRenderModelName = vrUtil->GetTrackedDeviceString(originInfo.trackedDeviceIndex, vr::Prop_RenderModelName_String);

				vr::RenderModel_t *controllerModel;
				vr::EVRRenderModelError error;
				while (1)
				{
					error = vr::VRRenderModels()->LoadRenderModel_Async(sRenderModelName.c_str(), &controllerModel);

					if (error != vr::VRRenderModelError_Loading)
						break;

					Sleep(1);
				}

				if (error != vr::VRRenderModelError_None)
				{
					printf("Unable to load render model %s - %s\n", sRenderModelName, vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error));
				}

				vr::RenderModel_TextureMap_t *pTexture;
				while (1)
				{
					error = vr::VRRenderModels()->LoadTexture_Async(controllerModel->diffuseTextureId, &pTexture);
					if (error != vr::VRRenderModelError_Loading)
						break;

					Sleep(1);
				}

				if (error != vr::VRRenderModelError_None)
				{
					printf("Unable to load render texture id:%d for render model %s\n", controllerModel->diffuseTextureId, sRenderModelName);
					vr::VRRenderModels()->FreeRenderModel(controllerModel);
				}

				std::vector<float> vertices;
				std::vector<float> normals;
				std::vector<float> texcoords;
				std::vector<uint16_t> indices;

				for (unsigned int i = 0; i < controllerModel->unVertexCount; i++)
				{
					vertices.push_back(controllerModel->rVertexData[i].vPosition.v[0]);
					vertices.push_back(controllerModel->rVertexData[i].vPosition.v[1]);
					vertices.push_back(controllerModel->rVertexData[i].vPosition.v[2]);
				}

				for (unsigned int i = 0; i < controllerModel->unVertexCount; i++)
				{
					normals.push_back(controllerModel->rVertexData[i].vNormal.v[0]);
					normals.push_back(controllerModel->rVertexData[i].vNormal.v[1]);
					normals.push_back(controllerModel->rVertexData[i].vNormal.v[2]);
				}

				for (unsigned int i = 0; i < controllerModel->unVertexCount; i++)
				{
					texcoords.push_back(controllerModel->rVertexData[i].rfTextureCoord[0]);
					texcoords.push_back(controllerModel->rVertexData[i].rfTextureCoord[1]);
				}

				for (unsigned int i = 0; i < controllerModel->unTriangleCount * 3; i++)
				{
					indices.push_back(controllerModel->rIndexData[i]);
				}

				Texture* texture = new Texture(controllerModel->diffuseTextureId, pTexture->unHeight, pTexture->unWidth, pTexture->rubTextureMapData);
				ObjModel* model = new ObjModel( vertices, normals, texcoords, indices, texture, MaterialInfo::getShaderByIllum(MaterialInfo::Illum::HIGHLIGHT_ON));
				m_rHand[eHand].m_pRenderModel = model;

				vr::VRRenderModels()->FreeRenderModel(controllerModel);
				vr::VRRenderModels()->FreeTexture(pTexture);
			}
		}
		
	}
}

void Gamewindow::RenderWorld(glm::mat4 view, glm::mat4 projection)
{
	for (unsigned int i = 0; i < city->worldModels.size(); i++)
	{
		glm::mat4 model = glm::translate(glm::mat4(), city->worldModels[i].location);
		model = glm::rotate(model, city->worldModels[i].rotation[0], glm::vec3(1, 0, 0));
		model = glm::rotate(model, city->worldModels[i].rotation[1], glm::vec3(0, 1, 0));
		model = glm::rotate(model, city->worldModels[i].rotation[2], glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(city->worldModels[i].scale.x, city->worldModels[i].scale.y, city->worldModels[i].scale.z));			//scale object

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

		//TODO: don't hardcode lights please
		ObjModel::ShaderMatrices matrices = ObjModel::ShaderMatrices();
		matrices.lightColour = city->lights[0].color;
		matrices.lightPosition = city->lights[0].position;
		matrices.model = model;
		matrices.normalMatrix = normalMatrix;
		matrices.projection = projection;
		matrices.view = view;
		matrices.viewPosition = city->player.position;

		//std::cout << city->worldModels[i].name << std::endl;
		city->worldModels[i].objModel->draw(matrices, shaders[currentshader]);
	}

	for (unsigned int i = 0; i < city->lights.size(); i++)
	{
		glm::mat4 model = glm::translate(glm::mat4(), city->lights[i].position);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));			//scale object

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

		ObjModel::ShaderMatrices matrices = ObjModel::ShaderMatrices();
		matrices.lightColour = city->lights[0].color;
		matrices.lightPosition = city->lights[0].position;
		matrices.model = model;
		matrices.normalMatrix = normalMatrix;
		matrices.projection = projection;
		matrices.view = view;
		matrices.viewPosition = city->player.position;

		city->lights[i].model->draw(matrices, shaders[currentshader]);
	}
}

void Gamewindow::RenderControllers(glm::mat4 view, glm::mat4 projection)
{
	for (Gamewindow::EHand eHand = Gamewindow::Left; eHand <= Gamewindow::Right; ((int&)eHand)++)
	{
		glm::mat4 model = m_rHand[eHand].m_rmat4Pose;

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

		ObjModel::ShaderMatrices matrices = ObjModel::ShaderMatrices();
		matrices.lightColour = city->lights[0].color;
		matrices.lightPosition = city->lights[0].position;
		matrices.model = model;
		matrices.normalMatrix = normalMatrix;
		matrices.projection = projection;
		matrices.view = view;
		matrices.viewPosition = city->player.position;

		if(m_rHand[eHand].m_pRenderModel != nullptr)
			m_rHand[eHand].m_pRenderModel->draw(matrices, shaders[currentshader]);

		if (city->teleporters[eHand].startTeleporting)
		{
			matrices.model = glm::translate(glm::mat4(), city->teleporters[eHand].getTeleportLocation());
			matrices.model = glm::scale(matrices.model, glm::vec3(0.01, 0.01, 0.01));

			matrices.normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * matrices.model)));

			if (city->teleporters[eHand].getModel() != nullptr)
				city->teleporters[eHand].getModel()->draw(matrices, shaders[currentshader]);
		}
	}
}

Gamewindow::EyeTextures Gamewindow::Display()
{
	if (!vrUtil->HasVRSystem())
	{
		if (postProcessingEnabled)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fboId);
			glViewport(0, 0, 2048, 2048);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(70.0f, screenSize.x / (float)screenSize.y, 0.1f, 2000.0f);
		glm::mat4 view = glm::lookAt({ 0,0,0 }, { 0, 0, 1 }, glm::vec3(0, 1, 0));

		view = glm::rotate(view, -city->player.rotation[0], { 1, 0, 0 });
		view = glm::rotate(view, city->player.rotation[1], { 0, 1, 0 });
		view = glm::rotate(view, city->player.rotation[2], { 0, 0, 1 });
		view = glm::translate(view, city->player.position);

		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		RenderWorld(view, projection);

		/*glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
		model = glm::scale(model, glm::vec3(city->skybox.x, city->skybox.y, city->skybox.z));*/

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (postProcessingEnabled)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, screenSize.x, screenSize.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			std::vector<glm::vec2> verts;
			verts.push_back(glm::vec2(-1, -1));
			verts.push_back(glm::vec2(-1, 1));
			verts.push_back(glm::vec2(1, 1));
			verts.push_back(glm::vec2(-1, -1));
			verts.push_back(glm::vec2(1, 1));
			verts.push_back(glm::vec2(1, -1));

			postProcessingShaders[currentPostShader]->use();
			glUniform1i(postProcessingShaders[currentPostShader]->getUniformLocation("s_texture"), 0);
			glUniform1f(postProcessingShaders[currentPostShader]->getUniformLocation("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

			glBindTexture(GL_TEXTURE_2D, fboTextureId);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);
			glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
			glDrawArrays(GL_TRIANGLES, 0, verts.size());
		}
	}
	else
	{
		// Left Eye

		//glEnable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_FRAMEBUFFER, eyeLeftId);
		glViewport(0, 0, 1000, 1000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection2 = vrUtil->GetHMDMatrixProjectionEye(vr::Eye_Left);
		glm::mat4 view2 = vrUtil->GetHMDMatrixPoseEye(vr::Eye_Left) * m_mat4HMDPose;

		RenderControllers(view2, projection2);

		view2 = vrUtil->GetHMDMatrixPoseEye(vr::Eye_Left) * m_mat4HMDPose;
		view2 = glm::scale(view2, glm::vec3(city->VRScale, city->VRScale, city->VRScale));

		view2 = glm::rotate(view2, city->player.rotation[1], { 0, 1, 0 });
		view2 = glm::translate(view2, glm::vec3(city->player.position.x, 0, city->player.position.z));

		//city->skybox.draw();

		RenderWorld(view2, projection2);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, eyeLeftId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, eyeLeftResolveFramebufferId);

		glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//glEnable(GL_MULTISAMPLE);

		// Right Eye

		glBindFramebuffer(GL_FRAMEBUFFER, eyeRightId);
		glViewport(0, 0, 1000, 1000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection3 = vrUtil->GetHMDMatrixProjectionEye(vr::Eye_Right);
		glm::mat4 view3 = vrUtil->GetHMDMatrixPoseEye(vr::Eye_Right) * m_mat4HMDPose;

		RenderControllers(view3, projection3);

		view3 = vrUtil->GetHMDMatrixPoseEye(vr::Eye_Right) * m_mat4HMDPose;
		view3 = glm::scale(view3, glm::vec3(city->VRScale, city->VRScale, city->VRScale));

		view3 = glm::rotate(view3, city->player.rotation[1], { 0, 1, 0 });
		view3 = glm::translate(view3, glm::vec3(city->player.position.x, 0, city->player.position.z));

		//city->skybox.draw();

		RenderWorld(view3, projection3);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, eyeRightId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, eyeRightResolveFramebufferId);

		glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000,
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		UpdateHMDMatrixPose();
	}

	glutSwapBuffers();

	//Framerate
	float newTime = glutGet(GLUT_ELAPSED_TIME);
	//if(newTime - currentTime > 0) std::cout << (int) (1000.0f / (newTime - currentTime)) << std::endl;
	currentTime = newTime;

	return EyeTextures{ eyeLeftResolveTextureId, eyeRightResolveTextureId };
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
