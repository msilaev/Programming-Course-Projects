/* Header file for the class Theater.
 * Contains declaration of data struction and class member functions.
 *
 * File author
* Name: Mikhail Silaev
* Student number: 151861415
* UserID: xdmisi
* E-Mail: mikhail.silaev@tuni.fi
*/

#ifndef THEATERS_H
#define THEATERS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <set>

using namespace std;

// This structure is info about play in a theatre
struct InfoPlay1 {
    string name_play;
    string town_name;
    string alias;
    string actor;
    int free_places;
};

using TheaterPlays = map <string, vector<InfoPlay1>>;

class Theaters
{
public:
    Theaters ();

    void printTheaters() ;
    void printPlays();

    int printPlaysTown(string city) ;
    int printPlaysTheater (string theater);

    int printPlayTheaters(string play);
    int printPlayersPlay( string play, string theater = "") ;

    bool isTheater(string theater);
    bool isPlay(string play);
    bool isTown(string city);

    void addInfo(string play_name, string play_aliase, string name_theater,
                 string player_name, string name_city, int num_seats);

private:
    TheaterPlays theater_plays_;
};

#endif // THEATERS_H
