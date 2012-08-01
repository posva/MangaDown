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
		eraseN(body);
		
		if (testing)
		{
			if (!directoryExists("test"))
				createDirectory("test");
			
			FILE* f = fopen("test/body.html", "w");
			fwrite(body.c_str(), sizeof(char), body.size(), f);
			fclose(f);
			
			std::cout<<"Html page output to file: test/body.html\n";
			
		}
		
		//Start parsing
		m_name = getParse(body, parseMangaName);
		std::string last_chapter(getParse(body, parseChapters));
		sscanf(last_chapter.c_str(), "%u", &m_num_chapters);
		
		if (testing)
			std::cout<<"Name parsed: "<<m_name<<"\nChapter count parsed: "<<last_chapter<<"\n";
		if (!testing)
		{
			if (!directoryExists("mangas"))
				createDirectory("mangas");
			
			if (!directoryExists("mangas/" + m_name))
				createDirectory("mangas/" + m_name);
		}
		
		
		if (m_cover != NULL)
			delete m_cover;
		m_cover = new Image(getParse(body, parseCover), "mangas/" + m_name + "/");
		
		if (!testing)
			m_cover->thDownload();
		else
			std::cout<<"Cover parsed: "<<m_cover->getUrl()<<"\n";
		
		m_loaded = true;
		
		
		//Fill list of chapters
		sf::Clock clock;
		size_t first(0);
		std::string chapter_web_list(getParse(body, parseChapterList));
		//erase all the \n!!
		eraseN(chapter_web_list);
		
		if (testing)
		{
			FILE* f = fopen("test/chapter_list.html", "w");
			fwrite(chapter_web_list.c_str(), sizeof(char), chapter_web_list.size(), f);
			fclose(f);
			
			std::cout<<"Html page output to file: test/chapter_list.html\n";
			
		}
		
		unsigned int chapter_i(1);
		while(true)
		{
			first = chapter_web_list.find(parseChapterListElement.begin[0], first);
			if (first == chapter_web_list.npos)
				break;
			else
				first += parseChapterListElement.begin[0].size();
			
			std::string chapter_url;
			switch (Manga::chapterPath) {
				case URL_uri:
					chapter_url = getParse(chapter_web_list.substr(first), parseChapterListUri);
					break;
					
				case URL_absolute:
					//std::cout<<getParse(chapter_web_list.substr(first), parseChapterListUri)<<" tachan!\n";
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
		
		if (testing)
		{
			std::cout<<m_chapters.size()<<" chapters were parsed\n";
			
			for (std::list<Chapter>::iterator it(m_chapters.begin()); it != m_chapters.end(); ++it)
				std::cout<<"Chapter "<<it->m_num_chapter<<" parsed: "<<it->m_uri<<"\n";
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
	sprintf(chapter_dir, "%s %u", m_name.c_str(), chapter);
	
	if (!testing)
	{
		if (!directoryExists("mangas/" + m_name + "/" + chapter_dir))
			createDirectory("mangas/" + m_name + "/" + chapter_dir);
		it->setDir("mangas/" + m_name + "/" + chapter_dir + "/");
	}
	
	
	it->setChapter(chapter);
	it->thDownload();
	
}

void Manga::showInfo() const
{
	std::cout<<m_name<<" ("<<m_webName<<") at "<<MangaHost<<m_uri<<". "<<m_chapters.size()<<" chapters\n";
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
		eraseN(body);
		
		if (testing)
		{
			char html_file[300];
			sprintf(html_file, "test/chapter_%u_page1.html", m_num_chapter);
			FILE* f = fopen(html_file, "w");
			fwrite(body.c_str(), sizeof(char), body.size(), f);
			fclose(f);
			
			std::cout<<"Html page output to file: "<<html_file<<"\n";
			
		}
		
		sscanf(getParse(body, Manga::parseChapterPages).c_str(), "%u", &m_pages);
		
		if (testing)
			std::cout<<"number of pages for chapter "<<m_num_chapter<<" parsed: "<<m_pages<<"\n";
		
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
		sprintf(filename, "%s %u-1", m_manga->m_name.c_str(), m_num_chapter);
		m_images[0]->setFileName(filename);
		if (!testing)
		{
			m_images[0]->thDownload();
			std::cout<<"Image 1/"<<m_images.size()<<" found.\n";
		}
	
		
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
		
		if (testing)
			std::cout<<"Image 1 parsed: "<<m_images[0]->getUrl()<<"\nNext page parsed: "<<next_page<<"\n";
		
		for (unsigned int i(1); i<m_images.size(); ++i)
		{
			request.setUri(next_page);
			response = web.sendRequest(request);
			status = response.getStatus();
			
			if (status == sf::Http::Response::Ok)
			{
				std::string body(response.getBody());
				
				//Erase all of the \n
				eraseN(body);
				
				if (testing && i == 1)
				{
					char html_file[300];
					sprintf(html_file, "test/chapter_%u_other_page.html", m_num_chapter);
					FILE* f = fopen(html_file, "w");
					fwrite(body.c_str(), sizeof(char), body.size(), f);
					fclose(f);
					
					std::cout<<"Html page output to file: "<<html_file<<"\n";
					
				}
				
				m_images[i]->setUrl(getParse(body, Manga::parsePageImg));
				m_images[i]->setDir(m_dir);
				sprintf(filename, "%s %u-%u", m_manga->m_name.c_str(), m_num_chapter, i+1);
				m_images[i]->setFileName(filename);
				if (!testing)
				{
					m_images[i]->thDownload();
					std::cout<<"Image "<<i+1<<"/"<<m_images.size()<<" found.\n";
				}
				
				next_page = getParse(body, Manga::parsePageNext);
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
				
				if (testing)
					std::cout<<"Image "<<i<<" parsed: "<<m_images[i]->getUrl()<<"\nNext page parsed: "<<next_page<<"\n";
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
	if (testing)
		return;
	
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

void Chapter::showInfo() const
{
	std::cout<<m_manga->m_name<<" "<<m_num_chapter<<" ("<<m_images.size()<<" pages) at "<<m_uri<<"\n";
}

void Manga::showChapters() const
{
	for (std::list<Chapter>::const_iterator it(m_chapters.begin()); it != m_chapters.end(); ++it)
		it->showInfo();
}