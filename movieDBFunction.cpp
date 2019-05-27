#include <iostream>
#include <fstream>
#include <winsock.h>
#include "Movie.h"
#include <map>
#include <sstream>
using namespace std;

map <string, Movie> readData( map <string, Movie> movieList);

map <string, Movie> readData(map <string, Movie> movieList)
{
    ifstream ifile;
    ifile.open("movies.txt",ios::in);

    string line;

    while(getline(ifile,line))
    {
        string title, yearS, director, stars, genre, runtimeS;
        int year, runtime;
        istringstream iss(line);

        getline(iss, title, '|');
        getline(iss, yearS, '|');
        getline(iss, director, '|');
        getline(iss, stars, '|');
        getline(iss, genre, '|');
        getline(iss, runtimeS, '|');
        year=stoi(yearS.c_str());
        runtime=stoi(runtimeS.c_str());

        Movie movie=Movie(title,year,director,stars,genre,runtime);
        movieList.insert(pair<string, Movie>(title,movie));
    }

    ifile.close();
    return movieList;
}

Movie searchTitle(string title, map <string, Movie> movieList)
{



}
