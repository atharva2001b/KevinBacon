
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
using namespace std;


class Movie;
class Person;
//class Principal; no need of principal class.. i made a multimap and directly added ppl to movies

map<string, Person*> mapPerson;
map<string, Movie*> mapMovie;
multimap<string, string> mapPrincipal;
map<string, string> mapRating;
map<string, float> mapPersonScore;


class Movie {
private:
	string movieName;  // not used
	
public:
	string movieID;    //in public for direct access
	string movieType;
	vector<string> movieGenere;
	float movieScore;
	string movieStartYear;
	vector <string> peopleIDs;

	Movie(string movieID, string movieType, string movieGenere, string movieStartYear = "2020", float movieScore = 0) {
		this->movieID = movieID;
		this->movieType = movieType;

		string genre;
		stringstream ss(movieGenere);
		while (getline(ss, genre, ',')) {
			this->movieGenere.push_back(genre);
		}

		this->movieScore = movieScore;
		this->movieStartYear = movieStartYear;
	}

	vector<string> getPeopleid() {
		return peopleIDs;
	}

	~Movie() {
	}


};


class Person {

	private:
		string name;
		string id;
		vector <string> movies;

	public:

		Person(string personID, string name) {
			this->id = personID;
			this->name = name;
		}

		string getID() {
			return id;
		};

		void setMovies(string str) {   //break comma seperated string and add to vector
			string movieID;
			stringstream ss(str);
			while (getline(ss, movieID, ',')) {
				movies.push_back(movieID);
			}
		}

		vector<string> getMovies() {
			return movies;
		}


		string getName() {
			return name;
		}

		~Person() {
		}
};
