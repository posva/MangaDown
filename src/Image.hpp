#pragma once

#include <SFML/Network.hpp>
#include "Utils.hpp"


enum downStatus {
	down_undone,
	down_started,
	down_fail,
	down_done
	};

class Image {
	std::string m_url, m_dir, m_filename;
	downStatus m_status;
	sf::Thread m_thDown;
	void download();
	
public:
	Image(const std::string& url, const std::string& dir) : m_url(url), m_dir(dir), m_filename(""), m_status(down_undone), m_thDown(&Image::download, this) {}
	Image() : m_url(""), m_dir(""), m_filename(""), m_status(down_undone), m_thDown(&Image::download, this) {};
	~Image();
	
	inline void setUrl(const std::string& url) { m_url = url; }
	inline void setDir(const std::string& dir) { m_dir = dir; }
	
	inline std::string getUrl() const { return m_url; }
	inline void setFileName(const std::string& filename) { m_filename = filename; }
	
	inline void thDownload()
	{
		m_thDown.launch();
	}
	
	inline downStatus getStatus() const { return m_status; }
};