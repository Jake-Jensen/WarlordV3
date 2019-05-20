#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include "TPBScraper.cpp"

using std::vector;  // For vectors. Obviously.
using std::cout;    // For printing information to the console.
using std::cin;     // For getting user input during torrent selection.
using std::string;  // For strings. Duh.


// Pretty large main by my standards. Still, it should do what it needs to do.
int main(int argc, char *argv[])
{
    TPBScraper Scraper; // Delcare the class

#pragma region Format the input string
    // Check if the required amount of arguments has been given, or exceeded. If not, open the help dialog.
    if (argc < 4) {
        cout << "No arguments specified. Printing the help page.\n";
        Scraper.HelpPageAccess();
        exit(-1);
    }

    // Loop through all the args, and format the search string if multiword
    int ArgLoopCount = 0;
    
    while (ArgLoopCount < argc) {
        if (ArgLoopCount > 2) {
            IsNotFormatted = true;
            FormattedSearchString.append(argv[ArgLoopCount]);
            FormattedSearchString.append("%20");
        }
        ArgLoopCount++;
    }

    if (IsNotFormatted == false) {
        FormattedSearchString = argv[3];
    }

    // Subtracting 2 due to -- prefix. Also subtracting 5 because otherwise it just straight fails. Don't ask me.
    if (FormattedSearchString.length() != 0) {
        string TempString01 = FormattedSearchString.substr(2, FormattedSearchString.length() - 5);
        FormattedSearchString = TempString01;
        cout << "Formatted string to search: " << FormattedSearchString << endl;
    }

#pragma endregion

    cout << "Checking mode...\n";
    string Search;
    bool Isbrowsing = false;

    // Yes, I know this is a lot of IF statements, and a switch case may be better here, but this allows faster
    // debugging and writing. 
    if (strcmp(FormattedSearchString.c_str(), "HDMovies") == 0) {
        Search = "https://www.thepiratebay.org/browse/207/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "Video") == 0) {
        Search = "https://www.thepiratebay.org/browse/200/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "Audio") == 0) {
        Search = "https://www.thepiratebay.org/browse/100/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "Games") == 0) {
        Search = "https://www.thepiratebay.org/browse/400/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "PCGames") == 0) {
        Search = "https://www.thepiratebay.org/browse/401/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "Porn") == 0) {
        Search = "https://www.thepiratebay.org/browse/500/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "HDPorn") == 0) {
        Search = "https://www.thepiratebay.org/browse/505/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "Apps") == 0) {
        Search = "https://www.thepiratebay.org/browse/300/0/7/0";
        Isbrowsing = true;
    }
    if (strcmp(FormattedSearchString.c_str(), "WinApps") == 0) {
        Search = "https://www.thepiratebay.org/browse/301/0/7/0";
        Isbrowsing = true;
    }

    // Above, we check if --Browse was passed. If it was, just create the URL hard and pass it. 
    // Below, we need to make the string based on what the user entered, and format it for them. 

    if (Isbrowsing == false) {
        // Build the search string
        Search = "https://www.thepiratebay.org/search/";
        Search.append(FormattedSearchString);
        Search.append("/0/99/0");
    }

    if (strcmp(argv[2], "--Search") == 0) {
        cout << "Downloading the source.\n";
        Scraper.ScrapeTPB(Search);
    }

    if (strcmp(argv[2], "--Browse") == 0) {
        cout << "Downloading the source.\n";
        Scraper.ScrapeTPB(Search);
    }

    if (strcmp(argv[2], "--Browse") != 0 || strcmp(argv[2], "--Search") != 0) {
        // Error checking
        cout << "Neither --Browse nor --Search was specified. Printing help page and exiting.\n";
        Scraper.HelpPageAccess();
        exit(-1);
    }

}

