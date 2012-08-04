#include "MangaParser.hpp"
#include "Manga.hpp"

bool checkArguments(int argc, const char* argv[], int &arg_counter);

int main (int argc, const char * argv[])
{
	int arg_counter(1);
	std::string manga_url;
	unsigned int n(0), n2(0);
	bool interval(false);
	
	//Check if there are extra arguments starting with - such as -t to test
	checkArguments(argc, argv, arg_counter);
	if (argc > arg_counter)
	{
		manga_url = argv[arg_counter];
		++arg_counter;
		
		checkArguments(argc, argv, arg_counter);
		if (argc > arg_counter)
		{
			sscanf(argv[arg_counter], "%u", &n);
			++arg_counter;
			
			checkArguments(argc, argv, arg_counter);
			if (argc > arg_counter)
			{
				sscanf(argv[arg_counter], "%u", &n2);
				++arg_counter;
				interval = true;
			}
			else
				n2 = n;
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
	
	
	char read_buff[100];
	if (n == 0 || n2 == 0)
	{
		std::cout<<"Which chapter do you want to read? (ex: 1 ; ex: 1 30 (chapter 1 to 30)\n";
		std::cin.getline(read_buff, 100);
		n2 = 0;
		sscanf(read_buff, "%u %u", &n, &n2);
		interval =  (n2 != 0);
	}
	
	while (n > manga.getChapters() || n == 0 || n2 > manga.getChapters() || (n2 != 0 && n > n2))
	{
		std::cout<<"You must enter numbers in range 1.."<<manga.getChapters()<<"\n";
		std::cin.getline(read_buff, 100);
		n2 = 0;
		sscanf(read_buff, "%u %u", &n, &n2);
		interval =  (n2 != 0);
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

bool checkArguments(int argc, const char* argv[], int &arg_counter)
{
	if (argc > arg_counter && argv[arg_counter][0] == '-')
	{
		for (unsigned int i(1); i<strlen(argv[arg_counter]); ++i)
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
