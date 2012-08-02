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
	
	if (testing)
	{
		if (!ini.isSection("Host"))
			std::cerr<<"The parser "<<file<<" is missing the section Host\n";
		else
		{
			if (!ini.isKey("Host", "name"))
				std::cerr<<"The parser "<<file<<" is missing the key name in section Host\n";
			if (!ini.isKey("Host", "url"))
				std::cerr<<"The parser "<<file<<" is missing the url name in section Host\n";
		}
		
		if (!ini.isSection("Title"))
			std::cerr<<"The parser "<<file<<" is missing the section Title (parser section).\n";
		if (!ini.isSection("Chapters"))
			std::cerr<<"The parser "<<file<<" is missing the section Chapters (parser section).\n";
		if (!ini.isSection("Cover"))
			std::cerr<<"The parser "<<file<<" is missing the section Cover (parser section).\n";
		if (!ini.isSection("ChapterList"))
			std::cerr<<"The parser "<<file<<" is missing the section ChapterList (parser section).\n";
		else
		{
			if (!ini.isKey("ChapterList", "reversed"))
				std::cerr<<"The parser "<<file<<" is missing the key reversed in section ChapterList\n";
		}
		if (!ini.isSection("ChapterListName"))
			std::cerr<<"The parser "<<file<<" is missing the section ChapterListName (parser section).\n";
		if (!ini.isSection("ChapterListUri"))
			std::cerr<<"The parser "<<file<<" is missing the section ChapterListUri (parser section).\n";
		else
		{
			if (!ini.isKey("ChapterListUri", "type"))
				std::cerr<<"The parser "<<file<<" is missing the key type in section ChapterListUri\n";
		}
		if (!ini.isSection("ChapterListElement"))
			std::cerr<<"The parser "<<file<<" is missing the section ChapterListElement (parser section).\n";
		if (!ini.isSection("PageImg"))
			std::cerr<<"The parser "<<file<<" is missing the section PageImg (parser section).\n";
		if (!ini.isSection("PageNext"))
			std::cerr<<"The parser "<<file<<" is missing the section PageNext (parser section).\n";
		else
		{
			if (!ini.isKey("PageNext", "type"))
				std::cerr<<"The parser "<<file<<" is missing the key type in section PageNext\n";
		}
		if (!ini.isSection("PageNumber"))
			std::cerr<<"The parser "<<file<<" is missing the section PageNumber (parser section).\n";

	}
	
	unsigned int read_int;
	
	m_name = ini.getValue("Host", "name");
	m_host = ini.getValue("Host", "url");
	
	//we load parsers
	Manga::parseMangaName.load(ini, "Title");
	Manga::parseChapters.load(ini, "Chapters");
	Manga::parseCover.load(ini, "Cover");
	
	Manga::parseChapterList.load(ini, "ChapterList");
	sscanf(ini.getValue("ChapterList", "reversed").c_str(), "%u", &read_int);
	Manga::chapterListReversed = static_cast<bool>(read_int);
	Manga::parseChapterListName.load(ini, "ChapterListName");
	Manga::parseChapterListUri.load(ini, "ChapterListUri");
	Manga::parseChapterListElement.load(ini, "ChapterListElement");
	Manga::parsePageImg.load(ini, "PageImg");
	Manga::parsePageNext.load(ini, "PageNext");
	Manga::parseChapterPages.load(ini, "PageNumber");
	
	sscanf(ini.getValue("ChapterListUri", "type").c_str(), "%u", &read_int);
	Manga::chapterPath = static_cast<URLKind>(read_int);
	sscanf(ini.getValue("PageNext", "type").c_str(), "%u", &read_int);
	Manga::mangaPath = static_cast<URLKind>(read_int);

	ini.free();
	
	//std::cout<<m_name<<": "<<m_host<<"\nTitle between "<<m_title.begin[0]<<" and "<<m_title.end<<"\n";
	//std::cout<<"chapters between "<<m_chapters.begin[0]<<" and "<<m_chapters.end<<"\n";
	
	Manga::MangaHost = m_host;
}
