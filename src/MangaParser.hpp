#pragma once
#include <string>
#include <list>
#include <SFML/Network.hpp>
#include "IniFile.hpp"
#include "Utils.hpp"
#include "Image.hpp"

class Manga;

class MangaParser {
	
	std::string m_host, m_name;
	
public:
	
	MangaParser();
	~MangaParser();
	
	void load(const std::string& file);
};