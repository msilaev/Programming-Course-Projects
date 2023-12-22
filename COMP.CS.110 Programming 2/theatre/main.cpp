/* Theater
 *
 * Desc:
 * This program implements a simple database allowing to search the information
 * about the plays in different theaters located in different towns.
 * At first the program reads strings from ';' - separated file. Strings must have exactly five
 * fields each.  Program stops with errors in case of the wrong file name program or
 * missing fields. After the successfull file reading the program awaits for user commands
 * "plays", "theaters", "players_in_play [play name] [theater name]", "plays_in_town [town_name]",
 * "plays_in_theatre [theatre name]".
 * The program terminates if any of the file reading errors occur or when the
 * user gives a quitting command ('quit').
 *
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi
 * E-Mail: mikhail.silaev@tuni.fi
 */

#include "theaters.hh"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

// Fields in the input file
const int NUMBER_OF_FIELDS = 5;

// Command prompt
const string PROMPT = "the> ";

// Commands
const  vector<string> COMMANDS =
{
    "plays", "theatres", "plays_in_theatre", "plays_in_town",
    "players_in_play", "theatres_of_play"
};

// Error and other messages
const string EMPTY_FIELD = "Error: empty field in line ";
const string FILE_ERROR = "Error: input file cannot be opened";
const string WRONG_PARAMETERS = "Error: wrong number of parameters";
const string THEATRE_NOT_FOUND = "Error: unknown theatre";
const string PLAY_NOT_FOUND = "Error: unknown play";
const string PLAYER_NOT_FOUND = "Error: unknown player";
const string TOWN_NOT_FOUND = "Error: unknown town";
const string COMMAND_NOT_FOUND = "Error: unknown command";
const string NOT_AVAILABLE = "No plays available";


// Casual split function, if delim char is between "'s, ignores it.
vector<string> split(const string& str, char delim)
{
    vector<string> result = {""};
    bool inside_quotation = false;
    for(char current_char : str)
    {
        if(current_char == '"')
        {
            inside_quotation = not inside_quotation;
        }
        else if(current_char == delim and not inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    return result;
}

// check that string format is correct having 5 non-empty fields
bool isStringValid(const vector <string>& s_vec)
{    
    if (s_vec.size()!= NUMBER_OF_FIELDS)
    {        
        return false;
    }
    for (auto s = s_vec.begin(); s!= s_vec.end(); ++s)
    {        
        if (s->size() == 0)
        {
            return false;
        }
    }
    return true;
}

// Thie function assigns the data from input vector line_vec
//to the relevant variables: play_name, play_aliase, name_theater,
//player_name, name_city, num_seats
void splitInputString(string& play_name, string& play_aliase,
                      string& name_theater,
                      string& player_name, string& name_city,
                      int& num_seats,
                      vector <string>& line_vec )
{
    // input string format: <town>;<theatre>;<play>;<player>;
    //<number_of_free_seats>
    name_theater = line_vec.at(1);
    name_city = line_vec.at(0);
    player_name = line_vec.at(3);

    // get number of seats and convert "none" to 0
    if (line_vec.at(4) == "none")
    {
        num_seats = 0;
    }
    else
    {
        num_seats= stoi(line_vec.at(4));
    }
    // Check if play name contains alias and if no assign allase = ""
    vector <string> name_alias = split(line_vec.at(2), '/');

    if (name_alias.size()>1)
    {
        play_aliase = name_alias.at(1);
        play_name =name_alias.at(0);
    }   
    else
    {
        play_aliase = "";
        play_name =name_alias.at(0);
    }
}

// This function reads the command from user input, checks if
// the command exists and has correct parameters.
// Returns true if command is correct and false otherwise. Prints type of error:
// "unknown command" or "wrong #parameters".
// comm_num is the command number calculated from the inupt
// and used in the main program.
bool readCommandSuccessfully(vector <string>& comm, int& comm_num)
{
     string input;
     while (true)
     {
        comm.clear();
        cout<<PROMPT;
        getline(cin, input);
        // Check if the input is 'q' to quit
        if (input == "quit")
        {
            return false;
        }
        comm = split(input, ' ');
        auto it = find(COMMANDS.begin(), COMMANDS.end(), comm.at(0));
        if (it == COMMANDS.end())
        {
            cout << "Error: unknown command" << endl;
        }
        else
        {
            bool cond1 = (comm.at(0) == "plays" and comm.size() == 1);
            bool cond2 = (comm.at(0) == "theatres" and comm.size() == 1);
            bool cond3 = (comm.at(0) == "plays_in_town" and comm.size() == 2);
            bool cond4 = (comm.at(0) ==
                          "plays_in_theatre" and comm.size() == 2);
            bool cond5 = (comm.at(0) ==
                          "players_in_play" and (comm.size() == 2
                                                 or comm.size() == 3));
            bool cond6 = (comm.at(0) ==
                          "theatres_of_play" and (comm.size() == 2));

            if (cond1)
            {comm_num=1;}

            else if (cond2)
            {comm_num=2;}

            else if (cond3)
            {comm_num=3;}

            else if (cond4)
            {comm_num=4;}

            else if (cond5)
            {comm_num=5;}

            else if (cond6)
            {comm_num=6;}

            if (cond1 or cond2 or cond3 or cond4 or cond5 or cond6)
            {
                return true;
            }
            else
            {
                cout << "Error: wrong number of parameters" << endl;
            }
        }
     }
        return false;
}

// Main function
int main()
{    
    Theaters theaters;

    string play_name;
    string player_name;
    string play_aliase ="";

    string name_theater;
    string name_city;
    int num_seats;
    vector <string> line_vec;

    // reading from file
    string filename = "";
    cout << "Input file: ";
    getline(cin, filename);

    ifstream file_object(filename);
    if ( not file_object )
    {
        cout << "Error: input file cannot be opened" << endl;
        return EXIT_FAILURE;
    }

       int lineNumber = 0;
       string line;
       while ( getline(file_object, line) )
       {

           ++ lineNumber;
            line_vec = split(line, ';');

           if (! isStringValid(line_vec))
           {
               cout << "Error: empty field in line " << lineNumber << endl;
               return EXIT_FAILURE;
           }
           splitInputString( play_name,  play_aliase,  name_theater,
                             player_name, name_city, num_seats, line_vec );

           // add information to the map which is member of class
           // Theaters
           theaters.addInfo(play_name,  play_aliase,  name_theater,
                            player_name, name_city, num_seats);

        }
       file_object.close();

       vector<string> comm;
       int comm_num;
       while(readCommandSuccessfully(comm, comm_num))
       {
         if (comm_num==1)
         {
             theaters.printPlays();
         }
         else if (comm_num==2)
         {
             theaters.printTheaters();
         }
         else if (comm_num==3)
         {
             theaters.printPlaysTown( comm.at(1));
         }
         else if (comm_num==4)
         {
             theaters.printPlaysTheater ( comm.at(1));
         }
         else if (comm_num == 5)
         {
             if (comm.size() == 2)
             {
                 theaters.printPlayersPlay(comm.at(1)) ;
             }
             else if (comm.size() == 3)
             {
                 theaters.printPlayersPlay(comm.at(1), comm.at(2)) ;
             }
         }
         else if (comm_num == 6)
         {
             theaters.printPlayTheaters(comm.at(1)) ;
         }
       }
    return EXIT_SUCCESS;
}
