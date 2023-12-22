/*
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi ( Necessary due to gitlab folder naming. )
 * E-Mail: mikhail.silaev@tuni.fi
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include <QPushButton>
#include <QtWidgets>
#include <QTimer>
#include <string>
#include "gameengine.hh"

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Setting players according to the information (number of players)
    // entered by user
    void set_players(GameEngine& eng);

    string message_from_roll_;
    string message_from_update_guide_;

    // main timer
    QTimer* timer;

    // for animation dices
    QTimer* timer_dices;

signals:
    // This is the most used function printing game status messages
    // in the text browser field
    void showMessage(string &result);


private slots:

    void on_numOfPlayersButton_clicked();

    void on_rollButton_clicked();

    void on_transitButton_clicked();

    void on_quitButton_clicked();

    void on_StartOverButton_clicked();

    void MyTimerSlot();

    void on_pauseButton_clicked();

    void on_resumeButton_clicked();

    void rotate_dice( );

private:
    Ui::MainWindow *ui;
    string selection_ = "";
    GameEngine *engine_;

    // flags showing is dices are locked
    std::vector<bool> locked_dices_ = {true, false, false, false, false};

    std::vector<int> locked_dices_int_ = {0,0,0,0,0};

    // vector of pointers to dice Push Button objects
    std::vector<QPushButton*> pictures = {nullptr, nullptr, nullptr, nullptr, nullptr};
    // fetch current content of the information text browser
    string current_message_;

     // Consts for dice icons geometry
    const int MARGIN = 80;
    const int SHIFT_X = 20;
    const int SHIFT_Y = 80;
    const int DICE_SIZE = 50;

    const int ROTATION_STEP = 20;
    const int ROTATION_DURATION =200;

    string numOfPlayersStr_;

    int time_;

    int time1_;

    int i_;
    vector<QPixmap> image_;

    // vector to storre points to use them in draw_dices()
    //funcion
    std::vector<int> points = {2,2,3,4,5,6};

    // flags showing if the corresponding buttons are disabled (true)
    bool Roll_blocked = false;
    bool Quit_blocked = false;
    bool Transit_blocked = false;
    bool Dice_blocked = false;

    // function collecting informaiton (numbers) about the locked dices
    void handle_dice_clicks();

    // unlock all dice buttons and erase info about blocked ones
    void recet_locked_dices();

    // block pressing on dice buttons
    void block_dices_buttons();

    // activate all dice buttons
    void activate_dices_buttons();

    // return vector of flags corresponding to locked dices
    std::vector<bool> get_locked_dices();

    // reset times
    void reset_timer();

    // return vector consisting of flags corresponding to locked
    // and unlocked data
    void retun_button_status();


    // draws dices icons with buttons, takes
    //references to vector with points and to vector with pointers to
    //the push button objects
    void draw_dices(std::vector<int>& points,
                    std::vector<QPushButton*>& pictures);



};
#endif // MAINWINDOW_HH
