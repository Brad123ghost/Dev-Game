#pragma once
#ifndef __DEBUGHELPER_H_
#define __DEBUGHELPER_H_

class Camera;

class DebugHelper {
public:
	static void DrawCameraDebug(Camera* pCamera);
	static void DrawGridDebug(int& gSize, int& cSize, bool& showGrid);
};

#endif // __DEBUGHELPER_H_