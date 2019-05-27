/* File: Movie.h

Where the movie data are stored.
*/

#ifndef Movie_h
#define Movie_h

#include <iostream>
#include <string>
using namespace std;

class Movie
{
private:
    string title;
    int year;
    string director;
    string genre;
    string stars;
    int runtime;

public:
    Movie();
    Movie(string, int, string, string, string, int);

    void setTitle(string);
    void setYear(int);
    void setDirector(string);
    void setStars(string);
    void setGenre(string);
    void setRuntime(int);

    string getTitle();
    int getYear();
    string getDirector();
    string getStars();
    string getGenre();
    int getRuntime();
};

Movie::Movie()
{}

Movie::Movie(string title, int year, string director, string stars, string genre, int runtime)
{
    this->title=title;
    this->year=year;
    this->director=director;
    this->stars=stars;
    this->genre=genre;
    this->runtime=runtime;
}

void Movie::setTitle(string title)
{
    this->title=title;
}
void Movie::setYear(int year)
{
    this->year=year;
}

void Movie::setDirector(string director)
{
    this->director=director;
}

void Movie::setStars(string stars)
{
    this->stars=stars;
}

void Movie::setGenre(string genre)
{
    this->genre=genre;
}

void Movie::setRuntime(int runtime)
{
    this->runtime=runtime;
}

string Movie::getTitle()
{
    return title;
}

int Movie::getYear()
{
    return year;
}

string Movie::getDirector()
{
    return director;
}

string Movie::getStars()
{
    return stars;
}

string Movie::getGenre()
{
    return genre;
}

int Movie::getRuntime()
{
    return runtime;
}

#endif // Movie_h
