#pragma once
#ifndef __DEBUGHELPER_H_
#define __DEBUGHELPER_H_

class Camera;
class EntityManager;

class DebugHelper {
public:
	static void DrawCameraDebug(Camera* pCamera, EntityManager* pEntitiyManager);
	static void DrawGridDebug(int& gSize, int& cSize, bool& showGrid);
};

#endif // __DEBUGHELPER_H_