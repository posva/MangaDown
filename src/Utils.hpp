#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include "IniFile.hpp"

extern bool testing;

bool directoryExists(const std::string& dir);
bool createDirectory(const std::string& dir);

int getFiles(const std::string &dir, std::vector<std::string> &files, const std::string& ext);

struct parse {
	std::vector<std::string> begin;
	std::string end;
	parse(const std::string& _begin, const std::string& _end) : begin(1), end(_end)
	{
		begin[0] = _begin;
	}
	parse() : begin(1), end("")
	{
		begin[0] = "";
	}
	void load(IniFile& ini, const std::string& name)
	{
		std::string n_str(ini.getValue(name, "n"));
		unsigned int n;
		sscanf(n_str.c_str(), "%u", &n);
		
		end = ini.getValue(name, "end");
		
		begin.resize(n);
		
		for (unsigned int i(1); i <= n; ++i)
		{
			char text[100];
			sprintf(text, "begin%u", i);
			begin[i-1] = ini.getValue(name, text);
		}
	}
};

inline std::string getUri(const std::string& url)
{
	return url.substr(1 + url.find('/', url.find('.')));
}

inline std::string getHost(const std::string& url)
{
	return url.substr(0, url.find('/', url.find('.')));
}

inline std::string getFile(const std::string& url)
{
	size_t first(0), end;
	
	while (url.find('/', first+1) != url.npos) {
		first = url.find('/', first+1);
	}
	
	end = url.find('?', url.find('.', first + 1));
	
	if (end == url.npos)
		return url.substr(first+1);
	else
		return url.substr(first+1, end - first - 1);
}

inline std::string getExtension(const std::string& url)
{
	size_t first(0);
	
	while (url.find('.', first+1) != url.npos) {
		first = url.find('.', first+1);
	}
	return url.substr(first);
}

inline std::string getHostName(const std::string& url)
{
	size_t end, first1, first2;
	first1 = url.find("://");
	first2 = url.find("www.");
	
	if (first2 != url.npos)
	{
		first2 += 4;
		end = url.find('.', url.find('.') + 1);
		return url.substr(first2, end-first2);
	}
	else if (first1 != url.npos)
	{
		first1 += 3;
		end = url.find('.');
		return url.substr(first1, end-first1);
	}
	else
	{
		end = url.find('.');
		return url.substr(0, end);
	}
		
}

inline std::string getParse(const std::string& str, const parse& par)
{
	if (par.begin.size() == 1)
	{
		size_t first(str.find(par.begin[0]) + par.begin[0].size());
		return str.substr(first, str.find(par.end, first) - first);
	}
	else
	{
		size_t first(0); unsigned int i(0);
		
		while (i < par.begin.size()) {
			first = str.find(par.begin[i], first) + par.begin[i].size();
			++i;
		}
		
		return str.substr(first, str.find(par.end, first) - first);
	}
	
	return "";
	
}

inline std::string getWorkingDirectory(const std::string& url)
{
	size_t first(0);
	
	while (url.find('/', first+1) != url.npos) {
		first = url.find('/', first+1);
	}
	return url.substr(0, first+1);
}

inline void eraseN(std::string& str)
{
	size_t pos;
	do {
		pos = str.find('\n');
		if (pos != str.npos)
			str.erase(pos, 1);
	} while (pos != str.npos);
}