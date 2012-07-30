#include "IniFile.hpp"

//#include <iostream>

IniFile::IniFile()
{
}

IniFile::IniFile(const std::string& Path)
{
	load(Path);
}

IniFile::~IniFile()
{
	free();
}

bool IniFile::load()
{
	return load(myPath);
}

bool IniFile::load(const std::string& Path)
{
	std::ifstream File(Path.c_str());
	if(File)
	{
		free();
		Section *CurrentSection;
		std::string line, key, value;
		while(std::getline(File, line))
		{
			if(line[0] == '[')
			{
				CurrentSection = new Section(line.substr(1, line.find_first_of("]") - 1));
				insert(std::make_pair(CurrentSection->getName(), CurrentSection));
			} else if(line != "" && line != "\n") {
				key = line.substr(line.find_first_not_of(" "));
				key = line.substr(0, line.find_first_of(" ="));
				if(line.find_first_of("=") + 1>= line.length())
				{
					value = "";
				} else  {
					value = line.substr(line.find_first_of("=") + 1);
					if(value.find_first_not_of(" ") > value.length())
						value = "";
					else
						value = value.substr(value.find_first_not_of(" "));
				}
				//std::cout << key << " = " << value << std::endl;
				CurrentSection->insert(std::make_pair(key, value));
			}
		}
		File.close();
		myPath = Path;
		return true;
	} else { return false; }
}

bool IniFile::save()
{
	return save(myPath);
}

bool IniFile::save(const std::string& Path)
{
	std::ofstream File(Path.c_str());
	if(File)
	{
		for(IniFile::iterator it = (*this).begin();
			it != (*this).end(); it++)
		{
			File << "[" << it->first << "]" << std::endl;
			for(Section::iterator it2 = it->second->begin();
				it2 != it->second->end(); it2++)
				File << it2->first << " = " << it2->second << std::endl;
			File << std::endl;
		}
		File.close();
		return true;
	} else { return false; }
}

void IniFile::free()
{
	for(IniFile::iterator it = (*this).begin();
		it != (*this).end(); it++)
		delete it->second;
	(*this).clear();
}

void IniFile::addSection(const std::string& Name)
{
	if(!isSection(Name))
	{
		insert(std::make_pair(Name, new Section(Name)));
	}
}

void IniFile::addValue(const std::string& Name, const std::string& Key, const std::string& Value)
{
	addSection(Name);
	Section *S = (*this)[Name];
	if(!isKey(Name, Key))
	{
		S->insert(std::make_pair(Key, Value));
	} else {
		(*S)[Key] = Value;
	}
}

Ini::Section* IniFile::getSection(const std::string& Name)
{
	return (*this)[Name];
}

bool IniFile::isSection(const std::string& Name)
{
	return count(Name);
}

std::string IniFile::getValue(const std::string& Name, const std::string& Key)
{
	return (*(*this)[Name])[Key];
}

bool IniFile::isKey(const std::string& Name, const std::string& Key)
{
	return isSection(Name) && (*this)[Name]->count(Key);
}

