#include "MangaParser.hpp"
#include "Manga.hpp"

MangaParser::MangaParser()
{
	
}

MangaParser::~MangaParser()
{
}

void MangaParser::load(const std::string &file)
{
	IniFile ini(file);
	
	m_name = ini.getValue("Host", "name");
	m_host = ini.getValue("Host", "url");
	
	//we load parsers
	Manga::parseMangaName.load(ini, "Title");
	Manga::parseChapters.load(ini, "Chapters");
	Manga::parseCover.load(ini, "Cover");
	
	Manga::parseChapterList.load(ini, "ChapterList");
	Manga::parseChapterListName.load(ini, "ChapterListName");
	Manga::parseChapterListUri.load(ini, "ChapterListUri");
	Manga::parseChapterListElement.load(ini, "ChapterListElement");
	Manga::parsePageImg.load(ini, "PageImg");
	Manga::parsePageNext.load(ini, "PageNext");
	Manga::parseChapterPages.load(ini, "PageNumber");
	
	unsigned int read_int;
	sscanf(ini.getValue("ChapterListUri", "type").c_str(), "%u", &read_int);
	Manga::chapterPath = static_cast<URLKind>(read_int);
	sscanf(ini.getValue("PageNext", "type").c_str(), "%u", &read_int);
	Manga::mangaPath = static_cast<URLKind>(read_int);

	ini.free();
	
	//std::cout<<m_name<<": "<<m_host<<"\nTitle between "<<m_title.begin[0]<<" and "<<m_title.end<<"\n";
	//std::cout<<"chapters between "<<m_chapters.begin[0]<<" and "<<m_chapters.end<<"\n";
	
	Manga::MangaHost = m_host;
}
