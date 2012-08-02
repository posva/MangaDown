Compile with make
Launch with ./bin/MangaDown
Remember to create your own parsers for the websites you use. Feel free to do pull requests to add your own parsers! It really appreciate that
You can also send me by email at posva13@gmail.com and I will add them to the repository and credit you here.

Usage example:

./bin/MangaDown http://www.mangareader.net/93/naruto.html 1 100
This Download the chapter 1 to 100 of Naruto from MangaReader
./bin/MangaDown http://www.mangareader.net/93/naruto.html 10
This download the chapter 10 of Naruto
./bin/MangaDown http://www.mangareader.net/93/naruto.html
This ask you for the chapter to download
./bin/MangaDown
This ask you the url of the manga and the chapter to download

Parsers:
-MangaReader (by me aka Posva)
-Manga Fox (by me aka Posva)


How to create your parser
Copy one that already exists and find words that will allow you to find the required information fro downloading the mangas.
The 'type' can be 1(uri), 2(relative) or 3(absolute).