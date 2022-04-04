#pragma once

#include "../ProjectRedShadow/Shader.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#include "../ProjectRedShadow/Space.h"
#include "../ProjectRedShadow/ObjModel.h"
#pragma comment(lib, "glew32.lib")
#include "../ProjectRedShadow/VRUtil.h"

class Gamewindow
{
public:
	VRUtil* vrUtil;

	struct EyeTextures
	{
		GLuint leftEye;
		GLuint rightEye;
	};

	struct ControllerInfo_t
	{
		vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionTeleport = vr::k_ulInvalidActionHandle;
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

	Gamewindow(Space* space, VRUtil* vr);
	~Gamewindow();
	void initVRShaders();
	EyeTextures Display();
	void NextShader();
	void PreviousShader();
	void NextPostShader();
	void PreviousPostShader();
	std::vector<Shader*> shaders;
	std::vector<Shader*> postProcessingShaders;
	int currentshader = 0;
	int currentPostShader = 0;
	glm::ivec2 screenSize;
	bool postProcessingEnabled = false;
private:
	void RenderWorld(glm::mat4 view, glm::mat4 projection);
	void RenderControllers(glm::mat4 view, glm::mat4 projection);
	void RenderEye(GLuint eyeId, vr::EVREye evrEye, GLuint eyeResolveFramebufferId);
	void UpdateHMDMatrixPose();
	void ConfigureMatrices(glm::mat4& view, glm::mat4& projection);
};
