/*
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi ( Necessary due to gitlab folder naming. )
 * E-Mail: mikhail.silaev@tuni.fi
 */

#include "gameengine.hh"
#include "functions.hh"
#include <iostream>
#include <sstream>

GameEngine::GameEngine():
    game_turn_(0), game_over_(false)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::add_player(const Player player)
{
    players_.push_back(player);
}

void GameEngine::update_guide()
{
    if(players_.size() <= game_turn_)
    {

        string message;
        message = "Internal error: update_guide";
        message_from_update_guide = message;

        return ;
    }
    ostringstream outputstream{""};
    outputstream << "Player " << game_turn_ + 1 << " in turn, "
                 << players_.at(game_turn_).rolls_left_ << " trials left!";

    message_from_update_guide = outputstream.str();
    return ;
}

void GameEngine::roll()
{
    vector<int> old_points ;
    vector<string> messages = { "", "", "", "", "", "", ""};
   // messages:
   //0 "Internal error: roll"
   //1  "No more rolls left"
   // 2 points
   // 3 status
   // 4 turn endedn
   // 5 winner
    if(players_.size() <= game_turn_)
    {
        messages.at(0) = "Internal error: roll";
        message_from_roll = messages;
        return ;
    }

    if(players_.at(game_turn_).rolls_left_ == 0)
    {
        messages.at(1) = "No more rolls left";
        message_from_roll = messages;
        return ;
    }

    if(players_.at(game_turn_).rolls_left_ < INITIAL_NUMBER_OF_ROLLS+1 and
            players_.at(game_turn_).rolls_left_ >0)
    {
        messages.at(6) = "Lock any dices by clicking on icons";
    }

    // old_points values needed to hadle locked dices
    old_points = new_points_vec_;
    ostringstream outputstream{""};
    vector<int> new_points = new_points_vec_ ;

    unsigned int dice = 0;
    while ( dice < NUMBER_OF_DICES )
    {
        int point_value = roll_dice();        

        if (!locked_dices.at(dice))
        {
            new_points_vec_.at(dice)=point_value;
            new_points.at(dice) = point_value;
            ++dice;
        }
        else
        {
            new_points_vec_.at(dice)=old_points.at(dice);
            new_points.at(dice) = old_points.at(dice);
            ++dice;
        }
    }
    messages.at(2) = update_points(new_points);
    report_player_status();
    messages.at(3) = message_from_report_status;


    // Decreasing rolls left
    --players_.at(game_turn_).rolls_left_;

    // Checking if the player in turn has rolls left
    if ( players_.at(game_turn_).rolls_left_ == 0 )
    {
        outputstream << "Turn of " << players_.at(game_turn_).id_
                     << " has ended!";

        messages.at(4) = outputstream.str();
    }

    // Checking if any player has turns left
    if (all_turns_used())
    {
      report_winner();
      messages.at(5)= message_from_report_winner;
    }
    message_from_roll = messages;

    return ;
}

void GameEngine::give_turn()
{
    string message = "";
    // Searching for the next player among those, whose id_ is greater than
    // that of the current player.
    for ( unsigned int i = game_turn_ + 1; i < players_.size(); ++i )
    {
        if ( players_.at(i).rolls_left_ > 0 )
        {
            game_turn_ = i;
            message_from_give_turn = message;

            return;
        }
    }

    // A suitable next player couldn't be found in the previous search, so
    // searching for the next player among those, whose id_ is less than
    // or equal to that of the current player
    // (perhaps the current player is the only one having turns left)
    for(unsigned int i = 0; i <= game_turn_; ++i)
    {
        if(players_.at(i).rolls_left_ > 0)
        {
            game_turn_ = i;
            message_from_give_turn = message;

            return ;
        }
    }

    // No player has turns left
    report_winner();
    message = message_from_report_winner;
    message_from_give_turn = message;
    return;

}

void GameEngine::report_winner()
{
    string message;
    vector<vector<int>> all_point_values;
    for ( auto player : players_ )
    {
        all_point_values.push_back(player.best_point_values_);
    }
    string winner_text = decide_winner(all_point_values);

    message = winner_text;
    game_over_ = true;
    message_from_report_winner = message;
    return ;
}

bool GameEngine::is_game_over() const
{
    return game_over_;
}

void GameEngine::report_player_status()
{
    if ( players_.size() <= game_turn_ )
    {
        message_from_report_status = "Internal error: report_player_status";
        return ;
    }
    string textual_description = "";
    construe_result(players_.at(game_turn_).latest_point_values_,
                    textual_description);
    message_from_report_status =textual_description;

    return ;
}

std::vector<int> GameEngine::report_roll_results() const
{
    return new_points_vec_;
}

string GameEngine::update_points(const std::vector<int>& new_points)
{
    string message="";
    if ( players_.size() <= game_turn_ )
    {
        message = "Internal error: update_points";
        return message;
    }
    string dummy = "";
    int new_result = construe_result(new_points, dummy);
    int best_result_so_far
            = construe_result(players_.at(game_turn_).best_point_values_,
                              dummy);
    if ( new_result > best_result_so_far )
    {
        players_.at(game_turn_).best_point_values_ = new_points;
    }
    players_.at(game_turn_).latest_point_values_ = new_points;
    return message;
}

bool GameEngine::all_turns_used() const
{
    for ( auto player : players_ )
    {
        if ( player.rolls_left_ > 0 )
        {
            return false;
        }
    }
    return true;
}
