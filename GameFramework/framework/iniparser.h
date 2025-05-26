// COMP710 GP Framework 2024
#ifndef __INIPARSER__H_
#define __INIPARSER__H_

#include <fstream>
#include <map>

class IniParser
{
public:
	IniParser();
	~IniParser();

	bool LoadIniFile(const std::string& filename);
	std::string GetValueAsString(const std::string& iniSection, const std::string& key);
	int GetValueAsInt(const std::string& iniSection, const std::string& key);
	float GetValueAsFloat(const std::string& iniSection, const std::string& key);
	bool GetValueAsBoolean(const std::string& iniSection, const std::string& key);
	int GetSectionCount(const std::string& iniSection);
protected:

private:
	IniParser(const IniParser& iniParser);
	IniParser& operator=(const IniParser& iniParser);

public:

protected:
	std::map<std::string, std::string> dataMap;

	std::string m_currentLine;
private:

};

#endif // __INIPARSER__H_