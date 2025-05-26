// COMP 710 GP Framework 2024
#ifndef __LOGMANAGER_H_
#define __LOGMANAGER_H_

#include "vector"
#include "string"

class LogManager
{
	// Member methods:
public:
	static LogManager& GetInstance();
	static void DestroyInstance();
	void Log(const char* pcMessage);
	
	void DebugDraw();

protected:

private:
	LogManager();
	~LogManager();
	LogManager(const LogManager& logMnager);
	LogManager& operator=(const LogManager& logManager);

	// Member Data:
public:

protected:
	static LogManager* sm_pInstance;

	std::vector<std::string> m_logHistory;
	bool m_bShowLogMessages;

private:
	bool m_bUserIsScrolling;
	bool m_bScrollToBottom;
};

#endif // __LOGMANAGER_H_