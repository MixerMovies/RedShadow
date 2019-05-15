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
	vr::VRActionHandle_t m_actionShock = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionWireframe = vr::k_ulInvalidActionHandle;
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
		vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Left].m_actionShock, 0, 2, 4, 1, vr::k_ulInvalidInputValueHandle);
		vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Right].m_actionShock, 0, 2, 4, 1, vr::k_ulInvalidInputValueHandle);
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

	vr::VRInput()->SetActionManifestPath("C:\\vr_bindings.json");

	vr::VRInput()->GetActionHandle("/actions/main/in/Wireframe", &m_rHand[Left].m_actionWireframe);
	vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
	vr::VRInput()->GetActionHandle("/actions/main/out/Shock", &m_rHand[Left].m_actionWireframe);

	vr::VRInput()->GetActionHandle("/actions/main/in/Wireframe", &m_rHand[Right].m_actionWireframe);
	vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
	vr::VRInput()->GetActionHandle("/actions/main/out/Shock", &m_rHand[Right].m_actionWireframe);
	
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

void HandleVRInput()
{
	// Process SteamVR events
	vr::VREvent_t event;
	while (ivrSystem->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event); 
	}

	vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Left].m_actionShock, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Right].m_actionShock, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);
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
