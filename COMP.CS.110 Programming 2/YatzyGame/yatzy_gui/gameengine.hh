/*
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi ( Necessary due to gitlab folder naming. )
 * E-Mail: mikhail.silaev@tuni.fi
 */

#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include <string>
#include <vector>

// Obvious constants
const int INITIAL_NUMBER_OF_ROLLS = 3;
const int NUMBER_OF_DICES = 5;

// Data of each player
struct Player
{
    unsigned int id_;
    unsigned int rolls_left_;
    std::vector<int> latest_point_values_;
    std::vector<int> best_point_values_;
};

class GameEngine
{
public:
    // Constructor
    GameEngine();

    // Destructor
    ~GameEngine();

    // Adds a new player
    void add_player(const Player player);

    // Prints guide text, telling which player is in turn and how many trials
    // they have left.
    void update_guide() ;

    // Rolls all dices, i.e. draws a new series of face numbers for the player
    // currently in turn. Moreover, reports the winner, if after the draw, all
    // players have used all their turns.
    void roll();

    // Reports the status of the player currently in turn
    void report_player_status() ;

    // Gives turn for the next player having turns left, i.e. for the next
    // element in the players_ vector. After the last one, turn is given for
    // the second one (since the first one is NOBODY).
    void give_turn();

    // Reports a winner based on the current situation and sets the game_over_
    // attribute as true.
    void report_winner();

    // Tells if the game is over, i.e. if all players have used all their
    // turns.
    bool is_game_over() const;

    // Returns current point on the dices after the last roll
    //Used in mainwindow in the function draw_daices
    std::vector<int> report_roll_results() const;

    // flags shwoing which dices are blocked
    std::vector<bool> locked_dices = {false, false, false, false, false};

    // messaged generated by corresponding functions
    std::vector<std::string> message_from_roll;
    std::string message_from_update_guide;
    std::string message_from_report_status;
    std::string message_from_give_turn;
    std::string message_from_report_winner;

private:

    // Updates best and latest points of the player in turn:
    // latest_point_values_ will always be new_points,
    // best_point_values_ will be new_points, if the last_mentioned is better.
    std::string update_points(const std::vector<int>& new_points);

    // Returns true if all turns of all players have been used,
    // otherwise returns false.
    bool all_turns_used() const;

    // Vector of all players
    std::vector<Player> players_;

    // Tells the player currently in turn (index of players_ vector)
    unsigned int game_turn_;

    // Tells if the game is over
    bool game_over_;

    // vector storing current roll results
    std::vector<int> new_points_vec_ = {0,0,0,0,0};

};

#endif // GAMEENGINE_HH
