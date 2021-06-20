#include "KevinBacon.h"
using namespace std;

/*
You can get .tsv files from IDMB website

TestCase:
I have added 2 names nm20 and nm21 in name.tsv (people)
tt20 and tt21 in title.tsv (movies)
where in principals.tsv and names.tsv i have set nm20 has participated 2 movies tt20 and tt21
while nm21 has participated tt21 movie
i.e.
	nm20 -> tt20, tt21
	nm21 -> tt21

tt20's movie score is supposed to be 90.. (comedy+horror (-0.1 percent), 2020 start year)
tt21's movie score is supposed to be 100.. (documentry (+0.0 percent), 2020 start year)

this is done for testing the BFS(breadth first search) like algorithm that this program has.

i have set line read limit to 70000 lines in LOAD_MAX_LIM.. for fast testing.

also many ppl will have 0 score given the discrepency in dataset..
therefore i have tested the algorithm with my own above additions.

*/






string NAME_TSV;            //set by cmd line arguments
string TEST_TITLE_TSV;      //        - also modified
string TEST_PRINCIPAL_TSV;  //          . name.tsv - added 2 names for testing                     
string RATINGS_TSV;         //          . testTitle.tsv, 
const int LOAD_MAX_LIM = 70000;      //--load less lines for fast results and testing // (no of lines read from each tsv file)



void loadPeople() {
	ifstream file1;
	file1.open(NAME_TSV);

	if (file1) {

		string temp;
		char buff[255];
		vector<string> row;

		getline(file1, temp);

		int n = 0;
		while (!file1.eof()) {

			file1.getline(buff, 255);

			stringstream ss(buff);

			while (getline(ss, temp, '\t')) {    //breaking lines into columns
				row.push_back(temp);             //adding individual column data to vector at specific row
			}

			Person* person = new Person(row[0], row[1]);   
			person->setMovies(row[5]);
			
			mapPerson.insert({ row[0], person });
			
			row.clear();

			if (n > LOAD_MAX_LIM) {
				break;
			}
			n++;

		}

		file1.close();

	}
	else {
		cout << "fail";
	}

	

}

void loadMovies() {
	ifstream file1;
	file1.open(TEST_TITLE_TSV);

	if (file1) {
		string temp;
		char buff[1000];
		vector<string> row;

		getline(file1, temp);

		int n = 0;
		while (!file1.eof()) {

			file1.getline(buff, 1000);

			stringstream ss(buff);

			while (getline(ss, temp, '\t')) {
				row.push_back(temp);
			}

			Movie* movie = new Movie(row[0], row[1], row[8], row[5]);
			mapMovie.insert({ row[0], movie });
			row.clear();

			if (n > LOAD_MAX_LIM) {
				break;
			}
			n++;

		}

		file1.close();
	}
	else {
		cout << "fail";
	}

	
}

void loadPrincipal() {
	ifstream file1;
	file1.open(TEST_PRINCIPAL_TSV);

	if (file1) {
		string temp;
		char buff[255];
		vector<string> row;

		getline(file1, temp);

		int n = 0;
		while (!file1.eof()) {

			file1.getline(buff, 255);

			stringstream ss(buff);

			while (getline(ss, temp, '\t')) {
				row.push_back(temp);
			}

			mapPrincipal.insert({ row[0], row[2] });

			row.clear();

			if (n > LOAD_MAX_LIM) {
				break;
			}
			n++;
		}
	}
	else {
		cout << "fail";
	}

	

}

void loadPeopleinMovie() {

	string prevMovie = "001";

	for (auto iter = mapPrincipal.begin(); iter != mapPrincipal.end(); iter++) {
		
		if (prevMovie != iter->first) {

			for(auto itr = mapPrincipal.find(iter->first); itr != mapPrincipal.end(); itr++) {
				
				if (itr->first != iter->first) {
					break;
				}

				auto movieitr = mapMovie.find(itr->first);
				if (movieitr != mapMovie.end()) {
					movieitr->second->peopleIDs.push_back(itr->second);
				}


			}

		}

		prevMovie = iter->first;

	}

}

void loadRatings() {
	ifstream file1;
	file1.open(RATINGS_TSV);

	if (file1) {
		string temp;
		char buff[500];
		vector<string> row;

		getline(file1, temp);


		int n = 0;
		while (!file1.eof()) {

			if (n > LOAD_MAX_LIM) {
				return;
			}
			n++;


			file1.getline(buff, 500);

			stringstream ss(buff);

			while (getline(ss, temp, '\t')) {
				row.push_back(temp);
			}

			auto itr = mapMovie.find(row[0]);

			if (itr != mapMovie.end()) {

				float percent = 0;

				vector<string> movietype = itr->second->movieGenere;

				for (vector<string>::iterator it = movietype.begin(); it != movietype.end(); ++it) {

					if (*it == "Comedy") {
						percent += 0.2;
					}
					if (*it == "Drama") {
						percent += 0.4;
					}
					if (*it == "Romance") {
						percent += -0.2;
					}
					if (*it == "Horror") {
						percent += -0.3;
					}
					
					itr->second->movieScore = ((1 + percent) * (stof(row[1]) * stof(row[2]))) * (100 - (2020 - stoi(itr->second->movieStartYear))) / 100;
				}
				
			}

			row.clear();

		}
	}
	else {
		cout << "fail";
	}

	
}

float getScore(vector<string> personID, int indirection) {

	float totalScore = 0;

	for (unsigned int i = 0; i < personID.size(); i++) {

		auto itr = mapPerson.find(personID[i]);

		if (itr != mapPerson.end()) {

			Person* person = new Person(personID[i], itr->second->getName());

			vector<string> movies = itr->second->getMovies();

			for (unsigned int j = 0; j < movies.size(); j++) {

				auto itrMovie = mapMovie.find(movies[j]);

				if (itrMovie != mapMovie.end()) {

					int _ = indirection - 1;

					if ( _ > 0) {
						totalScore += getScore(itrMovie->second->getPeopleid(), indirection - 1);
					}
					else {
						totalScore += itrMovie->second->movieScore;
					}
						 
				}

			}

		}
		else {
			return 0;
		}
	}

	return totalScore;

}

bool comparePeopleIDs(pair<string, float>& a, pair<string, float>& b) {       // compare function for pairs of 2 map values
	return a.first < b.first;
}

void sortMap(map<string, float>& M){    // sorting algorithm for map

	// Declare vector of pairs
	vector<pair<string, float> > A;

	// Copy key-value pair from Map
	// to vector of pairs
	for (auto& it : M) {
		A.push_back(it);
	}

	// Sort using comparator function
	sort(A.begin(), A.end(), comparePeopleIDs);

	M.clear();
	// add the sorted values
	for (auto& it : A) {
		M.insert({ it.first, it.second });
	}

}

void outputInFile(string outputFile,int indirection, bool addResonableScore) {
	fstream outdata;

	outdata.open(outputFile, fstream::in | fstream::out);
	outdata << "personID" <<"	" << "score" << "\n";
	outdata.seekp(0, ios::end);

	vector<string> peopleid;

	if (!outdata) {
		cout << "Some error occured while creating/opening the output file" << endl;
	}
	else {

		for (map<string, Person*>::iterator itr = mapPerson.begin(); itr != mapPerson.end(); ++itr) {

			peopleid.push_back(itr->second->getID());
			float score = getScore(peopleid, indirection);
			//cout << peopleid[0] << " id's person's score is " << score << " at " << indirection << " indirection" << "\n";
			mapPersonScore.insert({ itr->second->getID(), score });
			peopleid.clear();

		}

		sortMap(mapPersonScore);  //sort function

		for (map<string, float>::iterator itr = mapPersonScore.begin(); itr != mapPersonScore.end(); ++itr) {
			
			if (addResonableScore) {
				if (itr->second > 0) {
					outdata << itr->first << "	" << itr->second << "\n";     // data written in output.tsv
				}
				
			}
			else {
				outdata << itr->first << "	" << itr->second << "\n";     // data written in output.tsv
			}
		}

	}

	outdata.close();

}




void main(int argc, const char* argv[]){

	cout << "cmdline args* --" << endl;
	for (int i = 1; i < argc; i++)
	{
		cout <<"  "<< argv[i] << std::endl;
	}
	cout << "----***----" << endl << endl;


	NAME_TSV = argv[1];               // "name.tsv"                  --- Edited files to have 
	TEST_TITLE_TSV = argv[2];         // "title.tsv"                   - Manasi (nm20) in 2 movies tt20, tt21
	TEST_PRINCIPAL_TSV = argv[3];     // "principals.tsv"              - Atharva (nm21) in 1 movie tt21
	RATINGS_TSV = argv[4];            // "ratings.tsv"                 - movie score tt20 = 90 and tt21 = 100
	string outputFileName = argv[5];  // "output.txt" - testing      --- 
									  //                             --- check first 2 lines of all mentioned files
	vector<string> peopleid;
	peopleid.push_back(argv[6]);      // "nm20" - testing

	int indirection = stoi(argv[7]);  //indirection


	loadMovies();
	cout << "loaded Movies" << endl;

	loadPeople();
	cout << "loaded people" << endl;

	loadPrincipal();
	cout << "loaded principal" << endl;

	loadPeopleinMovie();
	cout << "loaded people in movie" << endl;

	loadRatings();
	cout << "loaded ratings" << endl;

	//print indivitual score 
	float score = getScore(peopleid, indirection);
	cout << endl << endl << "-------------------------------" << endl<<endl;
	cout << peopleid[0] << "	" << score << endl;
	cout << endl << "-------------------------------" << endl<<endl;


	bool addResonableScore = false; //add only scores > 0

	//create output file of all people for given indirection 
	//change cmd line args to set indirection
	outputInFile(outputFileName, indirection, addResonableScore);
	//refresh output.tsv file after running to see results

}

/*
Test case utput:

personID	score
nm20	1250
nm21	770

*/