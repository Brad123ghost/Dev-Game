// COMP710 GP Framework 2024

#include "iniparser.h"
#include "logmanager.h"

#include <fstream>
#include <iostream>
#include <assert.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

IniParser::IniParser()
{

}

IniParser::~IniParser()
{

}

bool IniParser::LoadIniFile(const std::string& filename)
{
	// Get file extension
	size_t position = filename.find_last_of(".");
	std::string fileExtension = filename.substr(position);

	// Check for .ini file type
	if (fileExtension.compare(".ini") != 0)
	{
		LogManager::GetInstance().Log("File is not of type .ini");
		assert(0);
	}

	// Open file
	std::ifstream inputFile(filename);
	if (!inputFile.is_open())
	{
		LogManager::GetInstance().Log("Failed to open file");
		assert(0);
	}
	std::string sectionName = "0";
	// Read lines
	while (std::getline(inputFile, m_currentLine))
	{
		// If line is not empty and not a comment
		if (m_currentLine.compare("") != 0 && m_currentLine.at(0) != ';')
		{
			// Check for section
			if (m_currentLine.at(0) == '[' && m_currentLine.back() == ']')
			{
				// Store the section name removing []
				sectionName = m_currentLine.substr(1, m_currentLine.length() -2);
			}
			else
			{
				// Get the key
				size_t keyPosition = m_currentLine.find("=");
				// Remove space before equal sign if it exists
				if (m_currentLine.at(keyPosition - 1) == ' ')
				{
					keyPosition -= 1;
				}
				std::string key = m_currentLine.substr(0, keyPosition);
				
				// Get the value
				size_t valuePosition = m_currentLine.find("=");
				// Remove space after equal sign if it exists
				if (m_currentLine.at(valuePosition + 1) == ' ')
				{
					valuePosition += 2;
				}
				std::string value = m_currentLine.substr(valuePosition);

				// Insery KV pair into map
				dataMap.insert({ sectionName + "|" + key, value });
			}
		}
	}

	inputFile.close();

	return true;
}

std::string IniParser::GetValueAsString(const std::string& iniSection, const std::string& key)
{
	auto pos = dataMap.find(iniSection + "|" + key);
	if (pos == dataMap.end())
	{
		return "No value";
	}
	return pos->second;
}

int IniParser::GetValueAsInt(const std::string& iniSection, const std::string& key)
{
	auto pos = dataMap.find(iniSection + "|" + key);
	if (pos == dataMap.end())
	{
		return 0;
	}
	
	return std::stoi(pos->second);
}

float IniParser::GetValueAsFloat(const std::string& iniSection, const std::string& key)
{
	auto pos = dataMap.find(iniSection + "|" + key);
	if (pos == dataMap.end())
	{
		return 0;
	}
	
	return std::stof(pos->second);
}

bool IniParser::GetValueAsBoolean(const std::string& iniSection, const std::string& key)
{
	auto pos = dataMap.find(iniSection + "|" + key);
	if (pos == dataMap.end())
	{
		return 0;
	}
	/*
	* This string to bool conversion method was taken from Stack Overflow.
	* Author: Georg Fritzsche
	* Link: https://stackoverflow.com/a/3613424
	* Date accessed: Monday 19 2024
	*/
	std::string temp = pos->second;
	std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
	std::istringstream is(temp);
	bool value;
	is >> std::boolalpha >> value;

	return value;
}

int IniParser::GetSectionCount(const std::string& iniSection)
{
	auto pos = dataMap.find(iniSection);
	if (pos == dataMap.end())
	{
		return 0;
	}
	
	int count = 0;
	for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
	{
		if (it->first.find(iniSection) != std::string::npos)
		{
			count++;
		}
	}
	return count;
}