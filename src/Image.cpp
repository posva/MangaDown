#include "Image.hpp"


Image::~Image()
{
	
}

void Image::download()
{
	if (m_status != down_undone) return;
	
	std::string file;
	
	if (m_filename == "")
		file = (m_dir + getFile(m_url));
	else
		file = m_dir + m_filename + getExtension(m_url);
	FILE* f = fopen(file.c_str(), "rb");
	
	if (f != NULL) //Cover was alreayd downloaded
	{
		m_status = down_done;
	}
	
	if (m_status == down_done) return;
	
	m_status = down_started;
	sf::Http web(getHost(m_url));
	sf::Http::Request request(getUri(m_url));
	std::cout<<"Downloading "<<m_url<<"\n";
	sf::Http::Response response = web.sendRequest(request);
	
	
	sf::Http::Response::Status status = response.getStatus();
	
	if (status == sf::Http::Response::Ok)
	{
		f = fopen(file.c_str(), "wb");
		fwrite(response.getBody().c_str(), sizeof(char), response.getBody().size(), f);
		fclose(f);
		m_status = down_done;
	}
	else
	{
		std::cerr<<"Download fail, error: "<<status<<"\n";
		m_status = down_fail;
	}
}