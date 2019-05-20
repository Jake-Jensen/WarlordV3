# WarlordV3
A torrent scraper for The Pirate Bay

# Usage
Examples: 

./WarlordV3 --TPB --Search --Ubuntu
          
./WarlordV3 --TPB --Browse --HDMovies
          
If the search text is not formatted for html (%20 for spaces), any extra arguments will be interpreted as spaces.

Example:
          
./WarlordV3 --TPB --Search --Ubuntu 18.10 x64 >> ./WarlordV3 --TPB --Search --Ubuntu%2018.10%20x64

Valid arguments:

[SearchSite]: --TPB

[SearchMode]: --Browse, --Search

[SearchText]: --NotFormattedOneWord, --Formatted%20One%20Word, --Not formatted multi word

[SearchText]: --HDMovies, --Video, --Audio, --Games, --PCGames, --Porn, --HDPorn, --Apps, --WinApps

