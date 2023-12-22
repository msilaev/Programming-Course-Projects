/* Source file for the class Theater.
 * Contains realizations of class member functions.
 *
 * File author
* Name: Mikhail Silaev
* Student number: 151861415
* UserID: xdmisi
* E-Mail: mikhail.silaev@tuni.fi
*/


#include "theaters.hh"
#include <iostream>
#include <algorithm>

Theaters::Theaters()
{}

//  The funciton print theaters
void Theaters::printTheaters()
{
    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        cout << it->first << endl;
    }
}


// The funciton print plays in a given city.
// Returns EXIT_SUCCESS or EXIT_FAILURE if errors occur.
int Theaters::printPlaysTown( string city )
{
    // If no towns found print error: unknown town
    if (!isTown(city))
    {
        cout << "Error: unknown town" << endl;
        return EXIT_FAILURE;
    }

    map<string, vector<InfoPlay1>> plays_town;

    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);

        if(plays_info_vec.at(0).town_name == city)
        {
            for (auto it1 = plays_info_vec.rbegin();
                 it1 != plays_info_vec.rend(); ++it1)
            {
                auto plays_one_theater = plays_town[it->first] ;
                auto it_find = find_if(plays_one_theater.begin(),
                        plays_one_theater.end(),
                        [it1](InfoPlay1 info) { return((*it1).name_play == info.name_play ); } ) ;

                if (it_find == plays_one_theater.end() )
                {
                    plays_town[it->first].push_back((*it1)) ;
                }
            }
        }
    }

    // filter plays with non-zero free places
    map<string, vector<InfoPlay1>> plays_town_filter;
    for (auto it = plays_town.begin(); it != plays_town.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);
        plays_info_vec.erase(remove_if(plays_info_vec.begin(),
        plays_info_vec.end(), [](InfoPlay1 info) {return (info.free_places == 0); } ),
        plays_info_vec.end());

        if (plays_info_vec.size() != 0)
        {
            plays_town_filter[it->first] = plays_info_vec;
        }
    }

    // check that there are free places
    if( plays_town_filter.size() == 0 )
    {
        cout << "No plays available" << endl;
        return EXIT_SUCCESS;
    }

    // print plays with aliases if any
    for(auto it = plays_town_filter.begin(); it != plays_town_filter.end(); ++it)
    {
       auto plays_one_theater = plays_town_filter[it->first];
       for (auto it1 = plays_one_theater.begin();
            it1 != plays_one_theater.end(); ++it1)
       {
           if ((*it1).alias =="" )
           {
               cout << it->first << " : " <<
                                 (*it1).name_play << " : " <<
                                   (*it1).free_places << endl;
           }
           else
           {
               cout << it->first << " : " <<
                               (*it1).name_play << " --- " <<
                                     (*it1).alias << " : " <<
                                   (*it1).free_places << endl;
           }
       }
    }
    return EXIT_SUCCESS;
}


// The funciton print plays in a given theater.
// Returns EXIT_SUCCESS or EXIT_FAILURE if errors occur.
int Theaters::printPlaysTheater(string theater)
{
    //check that theater exists
    if (!isTheater(theater))
    {
        cout << "Error: unknown theatre" << endl;
        return EXIT_FAILURE;
    }

    // form play name-alias map
    vector<InfoPlay1> plays_info_vec = theater_plays_[theater];
    map<string,string> plays_names_alias;
    for (auto it1 = plays_info_vec.begin(); it1 != plays_info_vec.end(); ++it1)
    {
        plays_names_alias[(*it1).name_play] = (*it1).alias ;
    }

    // print play name-alias
    for (auto it1 = plays_names_alias.begin();
         it1 != plays_names_alias.end(); ++ it1 )
    {
        if (it1->second != "")
        {
            cout << it1->first << " --- " << it1->second << endl ;
        }
        else
        {
            cout << it1->first << endl ;
        }
    }
    return EXIT_SUCCESS;
}


// The funciton prints theaters of the play.
// Returns EXIT_SUCCESS or EXIT_FAILURE of errors occur.
int Theaters::printPlayTheaters(string play)
{
    if (!isPlay(play) )
    {
        cout << "Error: unknown play" << endl;
        return EXIT_FAILURE;
    }

    // form a set of theater names
    set<string> s ;
    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);

        auto it_find = find_if( plays_info_vec.begin(), plays_info_vec.end(),
                             [&play](InfoPlay1 info)
        { return (info.name_play == play || info.alias == play ); } ) ;

        if (it_find != plays_info_vec.end())
        {
            s.insert(it->first) ;
        }
    }
    // print theater names
    for (auto it = s.begin(); it != s.end(); ++ it )
    {
        cout << (*it) << endl;
    }
    return EXIT_SUCCESS;
}

 //The funciton prints all plays
void Theaters::printPlays()
{
    map<string, string> play_alias ;
    // form the map of plays names -aliases
    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);

        for (auto it1 = plays_info_vec.begin();
             it1 != plays_info_vec.end(); ++ it1)
        {
            play_alias[(*it1).name_play] = (*it1).alias;
        }
     }          

    // print play names-aliases
    for (auto it = play_alias.begin(); it != play_alias.end(); ++it)
    {
        if(it->second == "")
        {
            cout << it->first << endl;
        }
        else
        {
            cout << it->first << " *** " << it->second << endl;
        }
    }
}

// The funciton prints players in a given play.
// Returns EXIT_SUCCESS or EXIT_FAILURE if errors occur.
int Theaters::printPlayersPlay(string play, string theater)
{
    if(!isTheater(theater) and theater != "")
    {
        cout << "Error: unknown theatre" << endl;
        return EXIT_FAILURE;
    }

    if (!isPlay(play))
    {
        cout << "Error: unknown play" << endl;
        return EXIT_FAILURE;
    }

    // form a player name -theater name map
    map<string, string> players ;
    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);

        for (auto it1 = plays_info_vec.begin();
             it1 != plays_info_vec.end(); ++it1)
        {
            if((*it1).name_play == play || (*it1).alias == play )
            {
                if(theater!="")
                {
                    if(theater==it->first)
                    {
                        players[(*it1).actor] = it->first  ;
                    }
                }
                else
                {
                    players[(*it1).actor] = it->first  ;
                }
            }
        }
    }    
    map <string, vector<string>> theater_player;

    // print players:theater
    for(auto it = players.begin(); it!=players.end(); ++it)
    {
        theater_player[it->second].push_back(it->first);
    }

    // print players:theater
    for(auto it = theater_player.begin(); it!=theater_player.end(); ++it)
    {
        for(auto it1 = (it->second).begin(); it1!=(it->second).end(); ++it1)
        {
            cout << it->first << " : " <<  *it1 << endl;
        }
    }
    return EXIT_SUCCESS;
}

// The funciton checks that the theater exist.
bool Theaters::isTheater(string theater)
{
    auto it = theater_plays_.find(theater);
    return(!(it == theater_plays_.end()) );
}

//  The funciton checks that the play exist.
bool Theaters::isPlay(string play)
{
    int play_count = 0;
    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);
        auto it_find_play = find_if( plays_info_vec.begin(),
                                     plays_info_vec.end(),
                             [&play](InfoPlay1 info)
        { return (info.name_play == play || info.alias == play ); } ) ;

        if (it_find_play != plays_info_vec.end())
        {
            ++play_count;
        }
    }
    return (play_count != 0);
}

//  The funciton checks that the town exist.
bool Theaters::isTown(string city)
{
    int town_count = 0;
    for (auto it = theater_plays_.begin(); it != theater_plays_.end(); ++it)
    {
        vector<InfoPlay1> plays_info_vec = (it->second);
        auto it_find_town = find_if( plays_info_vec.begin(),
                                     plays_info_vec.end(),
                             [&city](InfoPlay1 info)
        { return (info.town_name == city ); } ) ;

        if (it_find_town != plays_info_vec.end())
        {
            ++town_count;
        }
    }
    return (town_count != 0);
}

// add info to the class member map theater_plays_
void Theaters::addInfo(string play_name, string play_aliase,
                       string name_theater,
                       string player_name, string name_city, int num_seats)
{
    InfoPlay1 info_play1;
    info_play1.name_play = play_name;
    info_play1.actor = player_name;
    info_play1.free_places = num_seats;
    info_play1.alias = play_aliase;
    info_play1.town_name = name_city;

    theater_plays_[name_theater].push_back(info_play1);
}
