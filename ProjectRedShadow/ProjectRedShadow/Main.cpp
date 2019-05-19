#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Gamewindow.h"
#include "Space.h"
#include <iostream>

#include <openvr.h>

GLint gameWindowInt;
Gamewindow* gamewindow;
Space* space;
float lastTime;
bool wireframeEnabled = false;

vr::IVRSystem *ivrSystem;

struct ControllerInfo_t
{
	vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
	glm::mat4 m_rmat4Pose;
	ObjModel *m_pRenderModel = nullptr;
	std::string m_sRenderModelName;
	bool m_bShowController;
};

vr::VRActionHandle_t m_actionWireframe;
vr::VRActionHandle_t m_actionShock;
vr::VRActionHandle_t m_actionPreviousShader;
vr::VRActionHandle_t m_actionNextShader;

vr::VRActionSetHandle_t m_actionsetMain = vr::k_ulInvalidActionSetHandle;

enum EHand
{
	Left = 0,
	Right = 1,
};
ControllerInfo_t m_rHand[2];

bool goToPreviousShader = false;
bool goToNextShader = false;

void Init();
void Idle();
void HandleVRInput();
void Display();
void KeyEvent(unsigned char, int, int);
void KeyEventUp(unsigned char, int, int);
void SpecialKeyEvent(int, int, int);
void ProcessVREvent(const vr::VREvent_t&);
void StartVR();

#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}

void Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glewInit();

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}
}

void Idle()
{
	float time = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = time - lastTime;

	if (ivrSystem)
	{
		HandleVRInput();
	}

	gamewindow->rotation += elapsed / 1000.0f;

	space->player.move(elapsed);
	glutPostRedisplay();
	lastTime = time;
}

void Display()
{
	Gamewindow::EyeTextures eyeTextures = gamewindow->Display();

	if (ivrSystem)
	{
		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)eyeTextures.leftEye, vr::TextureType_OpenGL, vr::ColorSpace::ColorSpace_Linear };
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)eyeTextures.rightEye, vr::TextureType_OpenGL, vr::ColorSpace::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
		
	}
}

void KeyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		space->player.turnLeft(true);
		break;
	case 'd':
	case 'D':
		space->player.turnRight(true);
		break;
	case 'w':
	case 'W':
		space->player.goForward();
		break;
	case 's':
	case 'S':
		space->player.goBackward();
		break;
	case '[':
		gamewindow->PreviousShader();
		break;
	case ']':
		gamewindow->NextShader();
		break;
	case '.':
		space->PreviousModel();
		break;
	case '/':
		space->NextModel();
		break;
	case 'p':
	case 'P':
		gamewindow->postProcessingEnabled = !gamewindow->postProcessingEnabled;
		break;
	case 'l':
	case 'L':
		gamewindow->PreviousPostShader();
		break;
	case ';':
		gamewindow->NextPostShader();
		break;
	case '-':
		if(ivrSystem)
		vr::VRInput()->TriggerHapticVibrationAction(m_actionShock, 0, 2, 4, 1, vr::k_ulInvalidInputValueHandle);
		break;
	case 't':
	case 'T':
		if (wireframeEnabled)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		wireframeEnabled = !wireframeEnabled;
		break;
	case 'v':
	case 'V':
		StartVR();
		break;
	case 27:
		exit(0);
		break;
	}
}

void StartVR()
{
	vr::EVRInitError eError = vr::VRInitError_None;
	ivrSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		ivrSystem = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
	}

	vr::EVRInitError peError = vr::VRInitError_None;

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
	}

	vr::EVRInputError error = vr::VRInput()->SetActionManifestPath("D:\\Users\\Remco\\Documents\\Github\\RedShadow\\ProjectRedShadow\\ProjectRedShadow\\VRInput\\vr_bindings.json");

	vr::EVRInputError error2 = vr::VRInput()->GetActionHandle("/actions/main/in/wireframe", &m_actionWireframe);
	vr::EVRInputError error3 = vr::VRInput()->GetActionHandle("/actions/main/out/shock", &m_actionShock);
	vr::VRInput()->GetActionHandle("/actions/main/in/previousShader", &m_actionPreviousShader);
	vr::VRInput()->GetActionHandle("/actions/main/in/nextShader", &m_actionNextShader);

	vr::EVRInputError error4 = vr::VRInput()->GetActionSetHandle("/actions/main", &m_actionsetMain);

	vr::EVRInputError error5 = vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
	vr::EVRInputError error6 = vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);

	vr::VRCompositor()->ShowMirrorWindow();

	gamewindow->setVRSystem(ivrSystem);
}

void KeyEventUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		space->player.turnLeft(false);
		break;
	case 'd':
	case 'D':
		space->player.turnRight(false);
		break;
	case 'w':
	case 'W':
		space->player.stop();
		break;
	case 's':
	case 'S':
		space->player.stop();
		break;
	}
}

void SpecialKeyEvent(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		break;
	}
}

//---------------------------------------------------------------------------------------------------------------------
// Purpose: Returns true if the action is active and its state is true
//---------------------------------------------------------------------------------------------------------------------
bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t *pDevicePath = nullptr)
{
	vr::InputDigitalActionData_t actionData;
	vr::EVRInputError error = vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
	if (pDevicePath)
	{
		*pDevicePath = vr::k_ulInvalidInputValueHandle;
		if (actionData.bActive)
		{
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				*pDevicePath = originInfo.devicePath;
			}
		}
	}
	return actionData.bActive && actionData.bState;
}

void HandleVRInput()
{
	// Process SteamVR events
	vr::VREvent_t event;
	while (ivrSystem->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event); 
	}

	vr::VRActiveActionSet_t actionSet = { 0 };
	actionSet.ulActionSet = m_actionsetMain;
	vr::EVRInputError error = vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);
	//std::cout << error << std::endl;

	vr::VRInputValueHandle_t ulWireframe;
	if (GetDigitalActionState(m_actionWireframe, &ulWireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (GetDigitalActionState(m_actionPreviousShader) && !goToPreviousShader)
	{
		gamewindow->PreviousShader();
		goToPreviousShader = true;
	}
	else if (!GetDigitalActionState(m_actionPreviousShader) && goToPreviousShader)
	{
		goToPreviousShader = false;
	}
	
	if (GetDigitalActionState(m_actionNextShader) && !goToNextShader)
	{
		gamewindow->NextShader();
		goToNextShader = true;
	}
	else if (!GetDigitalActionState(m_actionNextShader) && goToNextShader)
	{
		goToNextShader = false;
	}

	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		vr::InputPoseActionData_t poseData;
		m_rHand[eHand].m_rmat4Pose = Gamewindow::ConvertSteamVRMatrixToMatrix4(poseData.pose.mDeviceToAbsoluteTracking);


	}
}

void reshape(int newWidth, int newHeight)
{
	gamewindow->screenSize.x = newWidth;
	gamewindow->screenSize.y = newHeight;
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------
void ProcessVREvent(const vr::VREvent_t & event)
{
	switch (event.eventType)
	{
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		printf("Device %u detached.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		printf("Device %u updated.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceActivated:
	{
		printf("Device %u activated.\n", event.trackedDeviceIndex);
	}
	break;
	}
}

int main(int argc, char *argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutInitWindowSize(1920, 1080);
	gameWindowInt = glutCreateWindow("Tower City");
	glutIdleFunc(Idle);
	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyEvent);
	glutKeyboardUpFunc(KeyEventUp);
	glutSpecialFunc(SpecialKeyEvent);
	glutReshapeFunc(reshape);
	Init();

	space = new Space();
	gamewindow = new Gamewindow(space, ivrSystem);

	glutMainLoop();
}
