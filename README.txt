Compile with make
Launch with ./bin/MangaDown
Remember to create your own parsers for the websites you use. Feel free to do pull requests to add your own parsers! It really appreciate that
You can also send me by email at posva13@gmail.com and I will add them to the repository and credit you here.

Usage example:
Note: In windows ./bin/MangaDown become bin\MangaDown.exe

Launching from console:
./bin/MangaDown http://www.mangareader.net/93/naruto.html 1 100
This Download the chapter 1 to 100 of Naruto from MangaReader
./bin/MangaDown http://www.mangareader.net/93/naruto.html 10
This download the chapter 10 of Naruto
./bin/MangaDown http://www.mangareader.net/93/naruto.html
This ask you for the chapter to download
./bin/MangaDown
This ask you the url of the manga and the chapter to download

Opening the executable by double clicking (if done under a UNIX OS you should place the parsers folder in your user folder, you can do it
placing yourself with command cd in the MangaDown folder and then using command cp -dr parsers/ ~/)
When the application ask you to enter chapter you can either enter a single chapter, ex: 31 or two chapters to download chapter between
the two values (the 2 chapters are also downloaded), ex: 31 35 download chapters 31, 32, 33, 34 and 35.

Parsers:
-MangaReader[ENG] (by me aka Posva)
-Manga Fox[ENG] (by me aka Posva)
-MangaHere[ENG] (by me aka Posva)

Binaries
You can download compiled binaries here: http://posva.github.com/MangaDown/

How to create your parser
Copy one that already exists and find words that will allow you to find the required information fro downloading the mangas.
The 'type' can be 1(uri), 2(relative) or 3(absolute).