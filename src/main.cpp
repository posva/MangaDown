#include "MangaParser.hpp"
#include "Manga.hpp"

int main (int argc, const char * argv[])
{
	std::string manga_url;
	if (argc < 2)
	{
		std::cout<<"URL of manga to download: ";
		std::cin>>manga_url;
	}
	else
		manga_url = argv[1];
	
	//Check avaibles parsers
	std::vector<std::string> parsers;
	getFiles("parsers", parsers, ".ini");
	
	bool can(false);
	unsigned int parser_i;
	for (unsigned int i(0); i<parsers.size(); ++i)
		if (parsers[i] == getHostName(manga_url) + ".ini")
			can = true, parser_i = i;
	
	if (!can)
	{
		std::cout<<"The website you provided doesn't have a parser!\nYou should have a file named "<<getHostName(manga_url)<<".ini in parsers/\n";
		return 1;
	}
	
	MangaParser mn;
	mn.load("parsers/" + parsers[parser_i]);
	
	Manga manga(getUri(manga_url), "");
	
	std::cout<<"Downloading manga information...\n";
	manga.downloadInformation();
	manga.showInfo();
	
	unsigned int n, n2;
	bool interval(false);
	
	if (argc > 2)
	{
		if (argc > 3)
		{
			sscanf(argv[3], "%u", &n2);
			sscanf(argv[2], "%u", &n);
			interval = true;
		}
		else
		{
			sscanf(argv[2], "%u", &n);
			n2 = n;
		}
	}
	else
	{
		std::cout<<"Which chapter do you want to read?\n";
		std::cin>>n;
		n2 = n;
	}
	
	if (n > manga.getChapters() || n == 0 || n2 > manga.getChapters() || n2 == 0 || n > n2)
	{
		n = 0;
		interval = false;
	}
	
	
	while (n > manga.getChapters() || n == 0)
	{
		std::cout<<"You must enter a number in range 0.."<<manga.getChapters()<<"\n";
		std::cin>>n;
	}
	
	if (!interval)
	{
		std::cout<<"Finding images...\n";
		manga.download(n);
		manga.getChapter(n).launchWait();
		manga.getChapter(n).waitForDownloads();
	}
	else
	{
		for (unsigned int i(n); i <= n2; ++i)
		{
			std::cout<<"Finding images for chapter "<<i<<"...\n";
			manga.download(i);
			manga.getChapter(i).launchWait();
			manga.getChapter(i).waitForDownloads();
		}
	}
	
	std::cout<<"Done!\n";

	return EXIT_SUCCESS;
}
