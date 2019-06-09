#pragma once

#include "Shader.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#include "Space.h"
#include "ObjModel.h"
#pragma comment(lib, "glew32.lib")
#include <openvr.h>

class Gamewindow
{
public:
	struct EyeTextures
	{
		GLuint leftEye;
		GLuint rightEye;
	};

	struct ControllerInfo_t
	{
		vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
		glm::mat4 m_rmat4Pose;
		ObjModel *m_pRenderModel = nullptr;
		std::string m_sRenderModelName;
		bool m_bShowController;
	};

	enum EHand
	{
		Left = 0,
		Right = 1,
	};
	ControllerInfo_t m_rHand[2];

	Gamewindow(Space* space, vr::IVRSystem* vrSystem);
	~Gamewindow();
	void initVRShaders();
	EyeTextures Display();
	void NextShader();
	void PreviousShader();
	void NextPostShader();
	void PreviousPostShader();
	void setVRSystem(vr::IVRSystem*);
	static glm::mat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &);
	std::vector<Shader*> shaders;
	std::vector<Shader*> postProcessingShaders;
	int currentshader = 0;
	int currentPostShader = 0;
	glm::ivec2 screenSize;
	float rotation = 0;
	bool postProcessingEnabled = false;
private:
	void RenderWorld(glm::mat4);
	void RenderControllers(glm::mat4);
	void UpdateHMDMatrixPose();
};
