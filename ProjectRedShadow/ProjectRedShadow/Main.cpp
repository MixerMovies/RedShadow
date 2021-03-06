#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Gamewindow.h"
#include "Space.h"
#include "FileLoader.h"

#include <iostream>

#include <openvr.h>

//#define STARTVRMODE

GLint gameWindowInt;
Gamewindow* gamewindow;
Space* space;
float lastTime;
bool wireframeEnabled = false;

vr::IVRSystem *ivrSystem;
bool _vrEnabled = false;

vr::VRActionHandle_t _actionWireframe;
vr::VRActionHandle_t _actionShock;
vr::VRActionHandle_t _actionPreviousShader;
vr::VRActionHandle_t _actionNextShader;
vr::VRActionHandle_t _actionGoForward;
vr::VRActionHandle_t _actionGoBackward;
vr::VRActionHandle_t _actionTurnLeft;
vr::VRActionHandle_t _actionTurnRight;
vr::VRActionHandle_t _actionGrow;
vr::VRActionHandle_t _actionShrink;

vr::VRActionSetHandle_t _actionsetMain = vr::k_ulInvalidActionSetHandle;

bool goToPreviousShader = false;
bool goToNextShader = false;

void Init();
void Idle();
void HandleVRInput();
void Display();
void KeyEvent(unsigned char, int, int);
void KeyEventUp(unsigned char, int, int);
void SpecialKeyEvent(int, int, int);
void MouseEvent(int, int, int, int);
void MouseMotionEvent(int, int);
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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewInit();

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

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

	space->player.Move(elapsed);
	glutPostRedisplay();
	lastTime = time;
}

void Display()
{
	Gamewindow::EyeTextures eyeTextures = gamewindow->Display();

	if (_vrEnabled)
	{
		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)eyeTextures.leftEye, vr::TextureType_OpenGL, vr::ColorSpace::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)eyeTextures.rightEye, vr::TextureType_OpenGL, vr::ColorSpace::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
		
	}
}

void KeyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		space->player.TurnLeft(true);
		break;
	case 'd':
	case 'D':
		space->player.TurnRight(true);
		break;
	case 'w':
	case 'W':
		space->player.GoForward();
		break;
	case 's':
	case 'S':
		space->player.GoBackward();
		break;
	case '[':
		gamewindow->PreviousShader();
		break;
	case ']':
		gamewindow->NextShader();
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
		vr::VRInput()->TriggerHapticVibrationAction(_actionShock, 0, 2, 4, 1, vr::k_ulInvalidInputValueHandle);
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

	std::string path = FileLoader::getMainPath() + "\\VRInput\\vr_bindings.json";
	vr::EVRInputError error = vr::VRInput()->SetActionManifestPath(path.c_str());

	vr::EVRInputError error2 = vr::VRInput()->GetActionHandle("/actions/main/in/wireframe", &_actionWireframe);
	vr::EVRInputError error3 = vr::VRInput()->GetActionHandle("/actions/main/out/shock", &_actionShock);
	vr::VRInput()->GetActionHandle("/actions/main/in/previousShader", &_actionPreviousShader);
	vr::VRInput()->GetActionHandle("/actions/main/in/nextShader", &_actionNextShader);
	vr::VRInput()->GetActionHandle("/actions/main/in/goForward", &_actionGoForward);
	vr::VRInput()->GetActionHandle("/actions/main/in/goBackward", &_actionGoBackward);
	vr::VRInput()->GetActionHandle("/actions/main/in/turnLeft", &_actionTurnLeft);
	vr::VRInput()->GetActionHandle("/actions/main/in/turnRight", &_actionTurnRight);
	vr::VRInput()->GetActionHandle("/actions/main/in/grow", &_actionGrow);
	vr::VRInput()->GetActionHandle("/actions/main/in/shrink", &_actionShrink);

	vr::EVRInputError error4 = vr::VRInput()->GetActionSetHandle("/actions/main", &_actionsetMain);

	vr::VRInput()->GetInputSourceHandle("/user/hand/left", &gamewindow->m_rHand[Gamewindow::EHand::Left].m_source);
	vr::VRInput()->GetInputSourceHandle("/user/hand/right", &gamewindow->m_rHand[Gamewindow::EHand::Right].m_source);
	vr::VRInput()->GetActionHandle("/actions/main/in/handRight", &gamewindow->m_rHand[Gamewindow::EHand::Right].m_actionPose);
	vr::VRInput()->GetActionHandle("/actions/main/in/handLeft", &gamewindow->m_rHand[Gamewindow::EHand::Left].m_actionPose);
	vr::VRInput()->GetActionHandle("/actions/main/in/teleportLeft", &gamewindow->m_rHand[Gamewindow::EHand::Left].m_actionTeleport);
	vr::VRInput()->GetActionHandle("/actions/main/in/teleportRight", &gamewindow->m_rHand[Gamewindow::EHand::Right].m_actionTeleport);

	vr::VRCompositor()->ShowMirrorWindow();

	gamewindow->setVRSystem(ivrSystem);
	_vrEnabled = true;
}

void KeyEventUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		space->player.TurnLeft(false);
		break;
	case 'd':
	case 'D':
		space->player.TurnRight(false);
		break;
	case 'w':
	case 'W':
		space->player.Stop();
		break;
	case 's':
	case 'S':
		space->player.Stop();
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
	case GLUT_KEY_SHIFT_L:
		space->player.isSprinting = !space->player.isSprinting;
		break;
	}
}

void MouseEvent(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		space->player.MouseEnabled(true);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		space->player.MouseEnabled(false);
	}
}

void MouseMotionEvent(int x, int y)
{
	space->player.mousePositionOffset.x = ((float)x / (float)gamewindow->screenSize.x * 2.0f - 1.0f);
	space->player.mousePositionOffset.y = ((float)y / (float)gamewindow->screenSize.y * 2.0f - 1.0f);
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
	actionSet.ulActionSet = _actionsetMain;
	vr::EVRInputError error = vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

	vr::VRInputValueHandle_t ulWireframe;
	if (GetDigitalActionState(_actionWireframe, &ulWireframe))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (GetDigitalActionState(_actionPreviousShader) && !goToPreviousShader)
	{
		gamewindow->PreviousShader();
		goToPreviousShader = true;
	}
	else if (!GetDigitalActionState(_actionPreviousShader) && goToPreviousShader)
	{
		goToPreviousShader = false;
	}
	
	if (GetDigitalActionState(_actionNextShader) && !goToNextShader)
	{
		gamewindow->NextShader();
		goToNextShader = true;
	}
	else if (!GetDigitalActionState(_actionNextShader) && goToNextShader)
	{
		goToNextShader = false;
	}

	if (GetDigitalActionState(_actionGoForward))
		space->player.GoForward();
	else if (GetDigitalActionState(_actionGoBackward))
		space->player.GoBackward();
	else
		space->player.Stop();

	if (GetDigitalActionState(_actionTurnLeft))
		space->player.TurnLeft(true);
	else if (GetDigitalActionState(_actionTurnRight))
		space->player.TurnRight(true);
	else
	{
		space->player.TurnLeft(false);
		space->player.TurnRight(false);
	}

	if (GetDigitalActionState(_actionGrow))
		space->Grow();
	else if (GetDigitalActionState(_actionShrink))
		space->Shrink();

	//Hand specific interaction
	for (int i = Gamewindow::EHand::Left; i <= Gamewindow::EHand::Right; i++)
	{
		if (GetDigitalActionState(gamewindow->m_rHand[i].m_actionTeleport))
		{
			space->teleporters[i].startTeleporting = true;
		}
		else if (space->teleporters[i].startTeleporting)
		{
			glm::mat4 transformation = glm::rotate(glm::mat4(), space->player.rotation.x, glm::vec3(1, 0, 0));
			transformation = glm::rotate(transformation, -space->player.rotation.y, glm::vec3(0, 1, 0));
			transformation = glm::rotate(transformation, space->player.rotation.z, glm::vec3(0, 0, 1));
			transformation = glm::translate(transformation, space->teleporters[i].getTeleportLocation() / space->VRScale);
			
			glm::vec4 pos = transformation * glm::vec4(0, 0, 0, 1);
			space->player.position -= glm::vec3(pos.x, pos.y, pos.z);

			space->teleporters[i].startTeleporting = false;
		}
	}
}

void Reshape(int newWidth, int newHeight)
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
	glutMouseFunc(MouseEvent);
	glutMotionFunc(MouseMotionEvent);
	glutPassiveMotionFunc(MouseMotionEvent);
	glutReshapeFunc(Reshape);
	Init();

	space = new Space();
	gamewindow = new Gamewindow(space, ivrSystem);

#ifdef STARTVRMODE
	StartVR();
#endif

	glutMainLoop();
}
