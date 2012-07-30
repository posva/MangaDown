#include "Manga.hpp"
#include <SFML/Network.hpp>

std::string Manga::MangaHost;
parse Manga::parseCover, Manga::parseChapters, Manga::parseChapterList, Manga::parseChapterListElement, Manga::parseChapterListName, Manga::parseChapterListUri, Manga::parsePageImg, Manga::parsePageNext, Manga::parseChapterPages, Manga::parseMangaName;
URLKind Manga::mangaPath, Manga::chapterPath;

void Manga::downloadInformation()
{
	sf::Http web(MangaHost);
	
	sf::Http::Request request(m_uri);
	
	sf::Http::Response response = web.sendRequest(request);
	
	sf::Http::Response::Status status = response.getStatus();
	
	if (status == sf::Http::Response::Ok)
	{
		//std::cout<<"Link is valid\n";
		
		std::string body(response.getBody());
		
		//Erase all of the \n
		size_t pos;
		do {
			pos = body.find('\n');
			if (pos != body.npos)
				body.erase(pos, 1);
		} while (pos != body.npos);
		
		
		//Start parsing
		m_name = getParse(body, parseMangaName);
		std::string last_chapter(getParse(body, parseChapters)), num_chapters;
		m_webName = last_chapter.substr(0, last_chapter.find('/'));
		num_chapters = last_chapter.substr(last_chapter.find('/')+1);
		sscanf(num_chapters.c_str(), "%u", &m_num_chapters);
		
		if (!directoryExists("mangas"))
			createDirectory("mangas");
		
		if (!directoryExists("mangas/" + m_webName))
			createDirectory("mangas/" + m_webName);
		
		if (m_cover != NULL)
			delete m_cover;
		m_cover = new Image(getParse(body, parseCover), "mangas/" + m_webName + "/");
		
		//m_cover->download();
		m_cover->thDownload();
		
		m_loaded = true;
		
		
		//Fill list of chapters
		sf::Clock clock;
		size_t first(0);
		std::string chapter_web_list(getParse(body, parseChapterList));
		unsigned int chapter_i(1);
		while(true)
		{
			first = chapter_web_list.find(parseChapterListElement.begin[0], first);
			if (first == chapter_web_list.npos)
				break;
			else
				first += parseChapterListElement.begin[0].size();
			///@todo add relative url etc
			std::string chapter_url;
			switch (Manga::chapterPath) {
				case URL_uri:
					chapter_url = getParse(chapter_web_list.substr(first), parseChapterListUri);
					break;
					
				case URL_absolute:
					chapter_url = getUri(getParse(chapter_web_list.substr(first), parseChapterListUri));
					break;
					
				case URL_relative:
					chapter_url = getWorkingDirectory(m_uri) + getParse(chapter_web_list.substr(first), parseChapterListUri);
					break;
					
				default:
					break;
			}
			m_chapters.push_back(Chapter(chapter_url, this, chapter_i));
			
			//std::cout<<"Added chapter: "<<m_chapters.back()<<"\n";
			++chapter_i;
			
		}
		
		//std::cout<<"Chapter list created in: "<<clock.getElapsedTime().asSeconds()<<" sec\n";
		
	}
	else
		std::cout<<"Error downloading manga info: "<<status<<"\n";
}

Chapter& Manga::getChapter(unsigned int n)
{
	unsigned int i(1);
	std::list<Chapter>::iterator it(m_chapters.begin());
	while (i < n)
	{
		++i;
		++it;
		if (it == m_chapters.end())
		{
			std::cerr<<"Chapter "<<n<<" doesn't exist for "<<m_name<<"\n";
			return m_chapters.front();
		}
	}
	
	return (*it);
}

void Manga::download(unsigned int chapter)
{
	unsigned int i(1);
	std::list<Chapter>::iterator it(m_chapters.begin());
	while (i < chapter)
	{
		++i;
		++it;
		if (it == m_chapters.end())
		{
			std::cerr<<"Chapter "<<chapter<<" doesn't exist for "<<m_name<<"\n";
			return;
		}
	}
	
	
	char chapter_dir[300];
	sprintf(chapter_dir, "%s %u", m_webName.c_str(), chapter);
	
	if (!directoryExists("mangas/" + m_webName + "/" + chapter_dir))
		createDirectory("mangas/" + m_webName + "/" + chapter_dir);
	it->setDir("mangas/" + m_webName + "/" + chapter_dir + "/");
	
	it->setChapter(chapter);
	it->thDownload();
	
}

void Manga::showInfo() const
{
	std::cout<<m_name<<" ("<<m_webName<<") at "<<MangaHost<<m_uri<<". "<<m_num_chapters<<" chapters\n";
}

Manga::~Manga()
{
	if (m_cover != NULL)
		delete m_cover;
	m_chapters.clear();
}

Chapter::~Chapter()
{
	for (std::vector<Image*>::iterator it(m_images.begin()); it != m_images.end(); ++it)
		delete *it;
	m_images.clear();
}

void Chapter::download()
{
	
	sf::Http web(Manga::MangaHost);
	
	sf::Http::Request request(m_uri);
	
	sf::Http::Response response = web.sendRequest(request);
	
	sf::Http::Response::Status status = response.getStatus();
	
	if (status == sf::Http::Response::Ok)
	{
		std::string body(response.getBody());
		
		//Erase all of the \n
		size_t pos;
		do {
			pos = body.find('\n');
			if (pos != body.npos)
				body.erase(pos, 1);
		} while (pos != body.npos);
		
		sscanf(getParse(body, Manga::parseChapterPages).c_str(), "%u", &m_pages);
		
		//Clean images
		for (std::vector<Image*>::iterator it(m_images.begin()); it != m_images.end(); ++it)
			delete *it;
		
		m_images.resize(m_pages);
		for (unsigned int i(0); i<m_images.size(); ++i)
			m_images[i] = new Image;
		
		//we ad the fisrt image
		m_images[0]->setUrl(getParse(body, Manga::parsePageImg));
		m_images[0]->setDir(m_dir);
		char filename[300];
		sprintf(filename, "%s %u-1", m_manga->m_webName.c_str(), m_num_chapter);
		m_images[0]->setFileName(filename);
		m_images[0]->thDownload();
		std::cout<<"Image 1/"<<m_images.size()<<" found.\n";
		
		std::string next_page(getParse(body, Manga::parsePageNext));
		
		switch (Manga::chapterPath) {
			case URL_uri:
				break;
			case URL_absolute:
				next_page = getUri(next_page);
				break;
			case URL_relative:
				next_page = getWorkingDirectory(m_uri) + next_page;
				break;
			default:
				break;
		}
		
		for (unsigned int i(1); i<m_images.size(); ++i)
		{
			///@todo modidy for relative path and etc
			request.setUri(next_page);
			response = web.sendRequest(request);
			status = response.getStatus();
			
			if (status == sf::Http::Response::Ok)
			{
				std::string body(response.getBody());
				
				//Erase all of the \n
				size_t pos;
				do {
					pos = body.find('\n');
					if (pos != body.npos)
						body.erase(pos, 1);
				} while (pos != body.npos);
				
				m_images[i]->setUrl(getParse(body, Manga::parsePageImg));
				m_images[i]->setDir(m_dir);
				sprintf(filename, "%s %u-%u", m_manga->m_webName.c_str(), m_num_chapter, i+1);
				m_images[i]->setFileName(filename);
				m_images[i]->thDownload();
				next_page = getParse(body, Manga::parsePageNext);
				
				std::cout<<"Image "<<i+1<<"/"<<m_images.size()<<" found.\n";
			}
			else
				std::cout<<"Error getting page: "<<status<<"\n";
		}
		
		//std::cout<<"img:"<<getParse(body, Manga::parsePageImg)<<" next page: "<<getParse(body, Manga::parsePageNext)<<"\n";
		//Download imgs
		
		//for (int i=0; i<1; ++i) m_images[i]->thDownload();
		
	}
	else
		std::cout<<"Error checking chapter: "<<status<<"\n";
}

void Chapter::waitForDownloads()
{
	while (1)
	{
		unsigned int done(0);
		
		for (unsigned int i(0); i<m_images.size(); ++i)
			done += (m_images[i]->getStatus() == down_done || m_images[i]->getStatus() == down_fail);
		if (done == m_images.size()) break;
		
		sf::sleep(sf::seconds(0.5f));
	}
	
	for (unsigned int i(0); i<m_images.size(); ++i)
		if (m_images[i]->getStatus() == down_fail)
			std::cerr<<"Page "<<i+1<<" failed to download\n";
}