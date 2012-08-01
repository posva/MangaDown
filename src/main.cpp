#include "MangaParser.hpp"
#include "Manga.hpp"

bool checkArguments(int argc, const char* argv[], unsigned int &arg_counter);

int main (int argc, const char * argv[])
{
	unsigned int arg_counter(1);
	std::string manga_url;
	unsigned int n(0), n2(0);
	bool interval(false);
	
	//Check if there are extra arguments starting with - such as -t to test
	if (argc > arg_counter)
	{
		checkArguments(argc, argv, arg_counter);
		
		if (argc > arg_counter)
		{
			manga_url = argv[arg_counter];
			++arg_counter;
		
			checkArguments(argc, argv, arg_counter);
			if (argc > arg_counter)
			{
				if (argc > arg_counter + 1)
				{
					sscanf(argv[arg_counter], "%u", &n);
					++arg_counter;
					checkArguments(argc, argv, arg_counter);
					sscanf(argv[arg_counter], "%u", &n2);
					++arg_counter;
					interval = true;
				}
				else
				{
					sscanf(argv[arg_counter], "%u", &n);
					++arg_counter;
					n2 = n;
				}
			}
		}
			
		
		//check again for parameters
		checkArguments(argc, argv, arg_counter);

	}
	else
	{
		std::cout<<"URL of manga to download: ";
		std::cin>>manga_url;
	}

	if (testing)
		std::cout<<"Testing mode ON. No files or folders will be downloaded/created in this mode.\n";
	
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
		std::cerr<<"The website you provided doesn't have a parser!\nYou should have a file named "<<getHostName(manga_url)<<".ini in parsers/\n";
		return 1;
	}
	
	MangaParser mn;
	mn.load("parsers/" + parsers[parser_i]);
	
	Manga manga(getUri(manga_url), "");
	
	std::cout<<"Downloading manga information...\n";
	manga.downloadInformation();
	if (!testing)
		manga.showInfo();
	
	if (n == 0 || n2 == 0)
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
		std::cout<<"You must enter a number in range 1.."<<manga.getChapters()<<"\n";
		std::cin>>n;
	}
	
	//manga.showChapters();
	
	if (!interval)
	{
		std::cout<<"Finding images...\n";
		manga.download(n);
		manga.getChapter(n).launchWait();
		if (testing)
			manga.getChapter(n).showInfo();
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

bool checkArguments(int argc, const char* argv[], unsigned int &arg_counter)
{
	if (argc > arg_counter && argv[arg_counter][0] == '-')
	{
		for (int i(1); i<strlen(argv[arg_counter]); ++i)
		{
			switch (argv[arg_counter][i])
			{
				case 't':
					testing = true;
					break;
					
				default:
					std::cerr<<"Unknown paremeter "<<argv[arg_counter][i]<<"\n";
					break;
			}
		}
		++arg_counter;
		return true;
	}
	
	return false;
}
