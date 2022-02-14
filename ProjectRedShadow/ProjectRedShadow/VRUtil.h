#pragma once

#include "framework.h"
#include <glm.hpp>
#include <openvr.h>

class RED_EXPORT VRUtil
{
private:
	vr::IVRSystem* m_pHMD = nullptr;

public:
	glm::mat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose);
	glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	std::string GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL);
	bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
	void SetVRSystem(vr::IVRSystem* ivrSystem);
	vr::IVRSystem* GetVRSystem();
	bool HasVRSystem();
};