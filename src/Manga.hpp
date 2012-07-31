#pragma once
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "Utils.hpp"
#include "Image.hpp"

class MangaParser;
class Manga;

enum URLKind {
	URL_uri,
	URL_relative,
	URL_absolute
	};

class Chapter {
	std::vector<Image*> m_images;
	std::string m_uri, m_dir;
	unsigned int m_pages, m_num_chapter, m_finished;
	const Manga* m_manga;
	
	sf::Thread m_thDownUrls;
	//Get the necessary information
	void download();
	
public:
	Chapter(const std::string& uri, const Manga* manga, unsigned int num) : m_uri(uri), m_dir(""), m_pages(0), m_num_chapter(num), m_finished(0), m_manga(manga), m_thDownUrls(&Chapter::download, this) {}
	Chapter(const Chapter& chapter) : m_uri(chapter.m_uri), m_dir(chapter.m_dir), m_pages(chapter.m_pages), m_num_chapter(chapter.m_num_chapter), m_finished(0), m_manga(chapter.m_manga), m_thDownUrls(&Chapter::download, this) {}
	~Chapter();
	
	inline void setDir(const std::string& dir) { m_dir = dir; }
	inline void setChapter(unsigned int chapter) { m_num_chapter = chapter; }
	
	inline void thDownload() { m_thDownUrls.launch(); }
	inline void launchWait() { m_thDownUrls.wait(); }
	
	void waitForDownloads();
	
	void showInfo() const;
};

class Manga {
	
	static std::string MangaHost;
	static parse parseChapters, parseCover, parseChapterList, parseChapterListElement, parseChapterListName, parseChapterListUri, parsePageImg, parsePageNext, parseChapterPages, parseMangaName;
	static URLKind mangaPath, chapterPath;
	
	std::string m_name, m_webName, m_uri;
	unsigned int m_num_chapters;
	friend class MangaParser;
	friend class Chapter;
	bool m_loaded;
	Image* m_cover;
	std::list<Chapter> m_chapters;
	
	sf::Thread m_thInfo;
	
public:
	Manga() : m_name(""), m_webName(""), m_uri(""), m_num_chapters(0), m_loaded(false), m_cover(NULL), m_thInfo(&Manga::downloadInformation, this) {}
	Manga(const std::string& uri, const std::string& name) : m_name(name), m_webName(""), m_uri(uri), m_num_chapters(0), m_loaded(false), m_cover(NULL), m_thInfo(&Manga::downloadInformation, this)
	{
		
	}
	Manga(const Manga& manga) : m_name(manga.m_name), m_webName(manga.m_webName), m_uri(manga.m_uri), m_num_chapters(manga.m_num_chapters), m_loaded(manga.m_loaded), m_cover(manga.m_cover), m_thInfo(&Manga::downloadInformation, this) 
	{
		
	}
	~Manga();
	void downloadInformation();
	
	void download(unsigned int chapter);
	
	void showInfo() const;
	void showChapters() const;
	inline unsigned int getChapters() const { return m_chapters.size(); }
	
	Chapter& getChapter(unsigned int n);
};