// Jake Jensen, 2019
// INTERNAL USE ONLY! Use only as an include.

/* Usage: 
    TPBScraper <Class nickname>;
    <Class nickname>.ScrapeTPB(<String to search>)
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;
bool IsNotFormatted;
bool IsMultiWord;
string FormattedSearchString;


// Is this class neccessary? No. Is it useful? Not really. Why's it here? Because I said so.
class TPBScraper {

public:

    void HelpPageAccess(){
        HelpPage();
        }

    void ScrapeTPB(string SearchString) {
        SearchTPB(SearchString);
        }

private:
    void HelpPage()
    {
    cout << "WarlordV3 Usage:\n";
    cout << "WarlordV3 [SearchSite] [SearchMode] [SearchText]\n\n";

    // You may ask, why --TPB if it's the only option? It's to ensure people actually read this help information.
    // This also allows me to add more sites in the future, such as 1337X and Rarbg (Once I figure out a way
    // to bypass the cookie check for robots)

    cout << "Examples:\n./WarlordV3 --TPB --Search --Ubuntu\n";
    cout << "./WarlordV3 --TPB --Browse --HDMovies\n\n";

    cout << "If the search text is not formatted for html (%20 for spaces), any extra arguments will be interpreted as spaces.\n";
    cout << "Example:\n";
    cout << "./WarlordV3 --TPB --Search --Ubuntu 18.10 x64 >> ./WarlordV3 --TPB --Search --Ubuntu%2018.10%20x64\n\n";

    cout << "Valid arguments:\n";
    cout << "[SearchSite]: --TPB\n";
    cout << "[SearchMode]: --Browse, --Search\n";
    cout << "[SearchText]: --NotFormattedOneWord, --Formatted%20One%20Word, --Not formatted multi word\n";
    cout << "[SearchText]: --HDMovies, --Video, --Audio, --Games, --PCGames, --Porn, --HDPorn, --Apps, --WinApps\n";
    cout << "This ends the help page for WarlordV3\n";
    exit(0);
}

    void SearchTPB(string SearchString)
    {
    string Search = SearchString;
    bool Isbrowsing = false;

    char *SearchChar = new char[Search.length() + 1];
    strcpy(SearchChar, Search.c_str());

    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = SearchChar;
    char outfilename[FILENAME_MAX] = "PageResults.html";
    curl = curl_easy_init();                                                                                                                                                                                                                                                           
    if (curl)
    {   
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }  

    vector<string> WebsiteSource;
    std::ifstream infile("PageResults.html");
    for(std::string line; getline(infile, line);)
        {
            WebsiteSource.push_back(line);
        }
    // We're done with the file, so let's remove it.
    infile.close();
    if (remove("PageResults.html") != 0) {
        perror( "Error deleting file" );
    } else {
        cout << "Removed source file. Now working entirely in RAM.\n";
    }

    // Error check the header and the title
    // WebsiteSource's first index should always be the doctype header.
    if (strcmp(WebsiteSource[0].c_str(), "<!DOCTYPE html>") == 0) {
        cout << "Source file confirmed as type HTML.\n";
    } else {
        cout << "Source file header was not HTML. Exiting.\n";
        exit(-2);
    }

    // The landing page, search, and browse, all have different headers, so let's check all of them at once.
    if (WebsiteSource[5].find("The Pirate Bay") != string::npos || WebsiteSource[4].find("TPB") != string::npos) {
        cout << "Website confirmed as The Pirate Bay.\n";
    } else {
        cout << "Website does not appear to be The Pirate Bay. Exiting.\n";
        exit(-3);
    }

    // Check the size of the source, to make sure it isn't a 404, or cloudfare. 
    // 205 is the line length of a 0 result search page, at the time of programming.
    // A more valid and foolproof method would be to interate through the source code line by line, checking
    // for error codes or Cloudfare's error page. This should do for the time being though.
    if (WebsiteSource.size() < 206) {
        cout << "The source file was too small. Exiting.\n";
        exit(-4);
    } else {
        cout << "Source file larger than baseline. Continuing.\n";
    }

    // Start parsing the data
    int VectorLengthSource = WebsiteSource.size();
    int Counter = 0;
    vector<string> Torrents;
    vector<string> Magnets;
    vector<string> Seeds;
    vector<string> Leech;
    bool Flip_Flop = false;
    while (Counter < VectorLengthSource) {
        if (WebsiteSource[Counter].find("<a href=\"/torrent/") != string::npos) {
            Torrents.push_back(WebsiteSource[Counter]);
        }
        if (WebsiteSource[Counter].find("magnet:?xt") != string::npos) {
            Magnets.push_back(WebsiteSource[Counter]);
        }

        // Seeds and leeches
        if (Flip_Flop == false) {
            if (WebsiteSource[Counter].find("<td align=\"right\">") != string::npos) {
            Seeds.push_back(WebsiteSource[Counter]);
            }
        } else {
            if (WebsiteSource[Counter].find("<td align=\"right\">") != string::npos) {
            Leech.push_back(WebsiteSource[Counter]);
            }
        }
            if (Flip_Flop == false) {
                Flip_Flop = true;
            } else {
                Flip_Flop = false;
            }
        
        Counter++;
    }

    // Trim the names
    // 21 - Start offset
    Counter = 0;
    int VectorLengthTorrents = Torrents.size();
    string Trimmer;
    string Trimmer2;
    vector<string> TrimmedTorrents;
    while (Counter < VectorLengthTorrents) {
        Trimmer = Torrents[Counter];
        Trimmer2 = Trimmer.substr(Trimmer.find("<a href"), Trimmer.find("class=\"detLink\""));
        Trimmer = Trimmer2.substr(9, Trimmer2.length() - 35);
        Trimmer2 = Trimmer.substr(9, Trimmer.length());
        Trimmer = Trimmer2.substr(Trimmer2.find_first_of("/") + 1, Trimmer2.length());
        TrimmedTorrents.push_back(Trimmer);
        Counter++;
    }

    Counter = 0;
    int VectorLengthSeeds = Seeds.size();
    string SeedTrim1;
    string SeedTrim2;
    vector<string> TrimmedSeed;
    while (Counter < VectorLengthSeeds) {
        SeedTrim1 = Seeds[Counter].substr(Seeds[Counter].find_first_of(">") + 1, Seeds[Counter].length());
        SeedTrim2 = SeedTrim1.substr(0, SeedTrim1.length() - 5);
        TrimmedSeed.push_back(SeedTrim2);
        Counter++;
    }

    Counter = 0;
    int VectorLengthLeech = Leech.size();
    string LeechTrim1;
    string LeechTrim2;
    vector<string> TrimmedLeech;
    while (Counter < VectorLengthLeech) {
        LeechTrim1 = Leech[Counter].substr(Leech[Counter].find_first_of(">") + 1, Leech[Counter].length());
        LeechTrim2 = LeechTrim1.substr(0, LeechTrim1.length() - 5);
        TrimmedLeech.push_back(LeechTrim2);
        Counter++;
    }

    Counter = 0;
    int MagnetLength = Magnets.size();
    string MagTrim1;
    string MagTrim2;
    vector<string> TrimmedMagnets;
    bool Trusted;
    bool VIP;
    cout << "Truncating results to 13 entries.\n";
    while (Counter < MagnetLength) {
        Trusted = false;
        VIP = false;
        if (Magnets[Counter].find("Trusted") != string::npos) {
            Trusted = true;
        }
        if (Magnets[Counter].find("VIP") != string::npos) {
            VIP = true;
        }
        MagTrim1 = Magnets[Counter].substr(9, Magnets[Counter].find("title=") - 11);
        TrimmedMagnets.push_back(MagTrim1);
        
        if (Counter < 14) {
            cout << Counter << ": " << TrimmedTorrents[Counter] << " [" << TrimmedLeech[Counter] << "] " << "[" << TrimmedSeed[Counter] << "]" << endl;
        if (Trusted == true && VIP == false) {
            cout << "[TRUSTED]" << endl;
        }
        if (Trusted == false && VIP == true) {
            cout << "[VIP]" << endl;
        }
        if (Trusted == false && VIP == false) {
            cout << "[WARNING]" << endl;
        }
        }

        
        Counter++;
    }

    // This is a temporary measure. The Windows .NET version already has a GUI that autofills and
    // allocates the magnet to the default torrenting application. 

    cout << "Enter your selection:\n";
    int Selection;
    cin >> Selection;
    cin.clear();
    string OpenSelectedTorrent;
    OpenSelectedTorrent = TrimmedMagnets[Selection];
    cout << "Here's your magnet.\n\n" << OpenSelectedTorrent << endl;
    exit(0);
    }
};

// Eyyy you made it to the end of the source. 
// Want to check out an indie game? I made it. https://store.steampowered.com/app/989210/PsyShift/
// Do you want to contribute to this source? Fork it and make your own version. Copying and pasting is no fun.