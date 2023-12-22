/*
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi ( Necessary due to gitlab folder naming. )
 * E-Mail: mikhail.silaev@tuni.fi
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), engine_(new GameEngine)
{
    ui->setupUi(this);

    ui->rollButton->setEnabled(false);
    ui->transitButton->setEnabled(false);
    ui->quitButton->setEnabled(false);

    // connect dice buttons to hahdle_dice_clicks function
    for (int i = 0; i<NUMBER_OF_DICES; ++i)
    {
        pictures.at(i) = new QPushButton("", this);
        connect(pictures.at(i), &QPushButton::clicked,
                this, &MainWindow::handle_dice_clicks);
    }

    // print message in the text browser
   connect(this, &MainWindow::showMessage, this,
           [this](const std::string& result)
     {
       ui->resultsBrowser->setText(QString::fromStdString(result));
     });

   // setting the maint timer and connecting to MyTimerSlot function
   // which increses time and shows it on the display
   timer = new QTimer(this);
   timer->setInterval(1000); // 1000 ms = 1 second
   time_=0;
   connect(timer, &QTimer::timeout, this, &MainWindow::MyTimerSlot);

   // setting the timer for dice animation
   timer_dices = new QTimer(this);
   timer_dices->setSingleShot(false);
   connect(timer_dices, &QTimer::timeout, this, &MainWindow::rotate_dice);

   // Connect QTimer's timeout signal to a lambda function
   connect(timer_dices, &QTimer::timeout, this, [=]() {
       time1_ += ROTATION_STEP; // Increment time1_ by the timer interval (20 milliseconds)
       if (time1_ > ROTATION_DURATION) {
           timer_dices->stop(); // Stop the timer when time1_ exceeds 100
       }
   });

   ui->lcdNumberSec->setStyleSheet("background-color: red; color: white;");
   ui->lcdNumberMin->setStyleSheet("background-color: yellow; color: black;");

   this->setStyleSheet("background-color: green");

  recet_locked_dices();
  block_dices_buttons();

  draw_dices(points, pictures);

}

MainWindow::~MainWindow()
{
    delete timer;
    timer = nullptr;

    delete timer_dices;
    timer_dices = nullptr;

    delete ui;
    delete engine_;
    engine_ = nullptr;

    for (int i = 0; i<NUMBER_OF_DICES; ++i)
    {
       delete pictures.at(i) ;
       pictures.at(i)= nullptr;
    }

}

void MainWindow::on_rollButton_clicked()
{
    engine_->roll();

    vector<string> messages = engine_->message_from_roll;

    // message numbers:
    //0 "Internal error: roll"
    //1  "No more rolls left"
    // 2 points
    // 3 status
    // 4 turn endedn
    // 5 winner
    // 6 lock dices

    if (messages.at(0) != "")
    {        
        emit showMessage(messages.at(0));
        ui->rollButton->setEnabled(false);
        ui->numOfPlayersButton->setEnabled(true);
    }

    else if (messages.at(3)!="" and messages.at(4)!="" )
    {
        message_from_roll_ =messages.at(3)+ "\n" + messages.at(4);
        emit showMessage(message_from_roll_);

        points = engine_->report_roll_results();

        draw_dices(points, pictures);
    }

    else if (messages.at(3)!="" )
    {
        if ( messages.at(6)=="")
        {
            engine_->update_guide();
            message_from_update_guide_ = engine_->message_from_update_guide;

            message_from_roll_ =messages.at(3)+ "\n\n" + message_from_update_guide_;

            emit showMessage(message_from_roll_);

            points = engine_->report_roll_results();

            draw_dices(points, pictures);
        }
        else
        {
            engine_->update_guide() ;
            message_from_update_guide_ = engine_->message_from_update_guide;
            message_from_roll_ = messages.at(3)+ "\n\n" +
                    message_from_update_guide_ + "\n\n" +
                    messages.at(6) ;

            emit showMessage(message_from_roll_);

            points = engine_->report_roll_results();

            draw_dices(points, pictures);

            for (int i = 0; i<NUMBER_OF_DICES; ++i)
            {
                pictures.at(i)->setEnabled(true);
            }
        }
    }

    else if (messages.at(1)!="")
    {
        emit showMessage(messages.at(1));
        ui->rollButton->setEnabled(false);

        block_dices_buttons();
        recet_locked_dices();
    }

    else if (messages.at(5)!="")
    {
        emit showMessage(messages.at(5));
    }
    else if (messages.at(4)!="")
    {
        emit showMessage(messages.at(4));
        ui->rollButton->setEnabled(false);
    }
    recet_locked_dices();
}

void MainWindow::on_transitButton_clicked()
{
    recet_locked_dices();
    block_dices_buttons();

    engine_->update_guide();
    message_from_update_guide_ = engine_->message_from_update_guide;

    engine_->give_turn();
    current_message_ = engine_->message_from_give_turn;

    if ( current_message_ != "")
    {
        emit showMessage(current_message_);
    }
    else
    {

    ui->transitButton->setEnabled(true);
    ui->rollButton->setEnabled(true);
    ui->quitButton->setEnabled(true);

    engine_->update_guide();
    message_from_update_guide_ = engine_->message_from_update_guide;
    emit showMessage(message_from_update_guide_);
    }
}

void MainWindow::on_quitButton_clicked()
{
    ui->rollButton->setEnabled(false);
    ui->transitButton->setEnabled(false);
    ui->quitButton->setEnabled(false);
    block_dices_buttons();

    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(false);

    this->setStyleSheet("background-color: blue");

    recet_locked_dices();
    block_dices_buttons();


    engine_->report_winner();

    current_message_ =engine_->message_from_report_winner + "\n" + "game time: "
                    + to_string(time_/60) + " min " +
            to_string(time_%60) + " sec";
    emit showMessage(current_message_);

    ui->numOfPlayersButton->setEnabled(false);
    ui->rollButton->setEnabled(false);
    ui->transitButton->setEnabled(false);
    ui->quitButton->setEnabled(false);
    timer->stop();

}

void MainWindow::on_numOfPlayersButton_clicked()
{
    ui->resumeButton->setEnabled(false);

    QString textFromLineEdit = ui->numPlayers->text();

    numOfPlayersStr_ = textFromLineEdit.toStdString();

    if (numOfPlayersStr_.size() == 1 and isdigit(numOfPlayersStr_.at(0))
            and stoi(numOfPlayersStr_)>0)
    {
        ui->numOfPlayersButton->setEnabled(false);
        ui->StartOverButton->setEnabled(true);
        ui->rollButton->setEnabled(true);
        ui->transitButton->setEnabled(true);
        ui->quitButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);


        this->set_players(*engine_);
        draw_dices(points, pictures);
        timer->start();
    }
    else
    {
        current_message_ = "Wrong or non-number entered, try again!";
        emit showMessage(current_message_);
    }
}

void MainWindow::set_players(GameEngine& eng)
{
    string player_amount_str = "";
    unsigned int player_amount = 0;

    player_amount_str = numOfPlayersStr_;

    if(player_amount_str.size() == 1 and isdigit(player_amount_str.at(0)))
    {
        player_amount = stoi(player_amount_str);
    }
    else
    {
        set_players(eng); // recursive call
    }
    for(unsigned int i = 0; i < player_amount; ++i)
    {
        Player player = {i + 1, INITIAL_NUMBER_OF_ROLLS, {}, {}};
        eng.add_player(player);
    }
    eng.update_guide();
    message_from_update_guide_ = eng.message_from_update_guide;
    emit showMessage(message_from_update_guide_);
}


void MainWindow::handle_dice_clicks()
{
    current_message_ =message_from_roll_;
    for (int i = 0; i<NUMBER_OF_DICES; ++i)
    {
        if (pictures.at(i) == sender())
         {
            locked_dices_.at(i) = true;
         }
    }

    for (int i = 0; i<NUMBER_OF_DICES; ++i)
    {
        if (locked_dices_.at(i))
         {
            current_message_ = current_message_ + "\n"+ "block " +
                    std::to_string(i+1) + "st dice ";
         }
    }
    engine_->locked_dices = get_locked_dices();
    emit showMessage(current_message_);
}

void MainWindow::recet_locked_dices()
{
    locked_dices_ = {false, false, false, false, false};
    engine_->locked_dices = get_locked_dices();
}

void MainWindow::block_dices_buttons()
{
    for (int i = 0; i<NUMBER_OF_DICES; ++i)
    {
        pictures.at(i)->setEnabled(false);
    }
}

void MainWindow::activate_dices_buttons()
{
    for (int i = 0; i<NUMBER_OF_DICES; ++i)
    {
        pictures.at(i)->setEnabled(true);
    }

}

std::vector<bool> MainWindow::get_locked_dices()
{
    return locked_dices_;
}

void MainWindow::reset_timer()
{
    time_=0;
    int sec= time_%60;
    int min =time_/60;
    ui->lcdNumberSec->display(sec);
    ui->lcdNumberMin->display(min);
}



void MainWindow::draw_dices(std::vector<int>& points, std::vector<QPushButton*>& pictures)
{
    for (int i =0; i<NUMBER_OF_DICES; ++i)
    {
         locked_dices_int_.at(i) = false;
        if(locked_dices_.at(i))
        {
            locked_dices_int_.at(i) = true;

        }
    }

    // Map of dice numbers and corresponding file names.
    // In this case it is trivial but can help is names are
    // different
    std::map<int, string> dices;
    dices[1] = "1";
    dices[2] = "2";
    dices[3] = "3";
    dices[4] = "4";
    dices[5] = "5";
    dices[6] = "6";

    std::vector<std::string>  dices_vec ;
    for (auto it_numbers = points.begin();
         it_numbers != points.end(); ++it_numbers)
    {
        dices_vec.push_back(dices[*it_numbers]);
    }

    // Define where the images can be found and
    //what kind of images they are
    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");

    image_.clear();

    for(unsigned int i = 0; i < NUMBER_OF_DICES; ++i)
    {

        pictures.at(i)->setGeometry(SHIFT_X + i * (DICE_SIZE + SHIFT_X),
                       SHIFT_Y, DICE_SIZE, DICE_SIZE);

         // Converting image (png) to a pixmap
         std::string filename = PREFIX + dices_vec.at(i) + SUFFIX;

         QPixmap image(QString::fromStdString(filename));


         // Scaling the pixmap
         image = image.scaled(DICE_SIZE, DICE_SIZE);
         image_.push_back(image);

         // Setting the pixmap for a button
         QPushButton* labelAtIndex1 = pictures.at(i);
         if (labelAtIndex1 != nullptr)
         {
             labelAtIndex1->setIcon(QIcon(image));
             labelAtIndex1->setIconSize(3*image.size()/4);
             labelAtIndex1->setText("");
         }

         // Rotate the image using QTimer for a delay of 20 milliseconds
         time1_ =0;
         timer_dices->start(ROTATION_STEP);

             }
         }

void MainWindow::rotate_dice()
{
    QTransform transform ;
    double angle = 360*time1_/200;
    QPixmap rotatedImage = image_.at(0);

    for (int i =0; i<NUMBER_OF_DICES; ++i)
    {
            QPushButton* labelAtIndex1 = pictures.at(i);

            if (labelAtIndex1 != nullptr)
            {
                if(!locked_dices_int_.at(i))
                {
                    rotatedImage = image_.at(i).transformed(QTransform().rotate(angle));
                }
                else
                {
                     rotatedImage = image_.at(i);
                }

                labelAtIndex1->setIcon(QIcon(rotatedImage));
                labelAtIndex1->setIconSize(3*rotatedImage.size()/4);
                labelAtIndex1->setText("");
        }
     }
}

void MainWindow::on_StartOverButton_clicked()
{
    this->setStyleSheet("background-color: green");

    reset_timer();
    timer->stop();
    recet_locked_dices();
    block_dices_buttons();
    delete engine_;
    engine_ = new GameEngine;

    ui->numOfPlayersButton->setEnabled(true);
    ui->rollButton->setEnabled(false);
    ui->transitButton->setEnabled(false);
    ui->quitButton->setEnabled(false);
    block_dices_buttons();


    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(true);

    ui->resultsBrowser->clear();

}

void MainWindow::MyTimerSlot()
{
    time_++;
    int sec= time_%60;
    int min =time_/60;
    ui->lcdNumberSec->display(sec);
    ui->lcdNumberMin->display(min);
}


void MainWindow::retun_button_status()
{
    Roll_blocked = false;
    if (!ui->rollButton->isEnabled())
    {
        Roll_blocked = true;
    }

    Transit_blocked = false;
    if (!ui->transitButton->isEnabled())
    {
        Transit_blocked = true;
    }

    Quit_blocked = false;
    if (!ui->quitButton->isEnabled())
    {
        Quit_blocked = true;
    }

    Dice_blocked = false;
    if (!pictures.at(0)->isEnabled())
    {
        Dice_blocked = true;
    }
}

void MainWindow::on_pauseButton_clicked()
{
    timer->stop();
    retun_button_status();

    ui->rollButton->setEnabled(false);
    ui->transitButton->setEnabled(false);
    ui->quitButton->setEnabled(false);
    block_dices_buttons();


    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(true);

    QString currentText = ui->resultsBrowser->toPlainText();
    current_message_ = currentText.toStdString();
    string message = current_message_ + "\n" + "game paused";
    emit showMessage(message);
}

void MainWindow::on_resumeButton_clicked()
{
    timer->start();
    ui->pauseButton->setEnabled(true);
    ui->resumeButton->setEnabled(false);

    if (!Roll_blocked) { ui->rollButton->setEnabled(true);}
    if (!Transit_blocked) { ui->transitButton->setEnabled(true); }
    if (! Quit_blocked) { ui->quitButton->setEnabled(true); }
    if (! Dice_blocked) { activate_dices_buttons(); }

    // show message that was before pause
    emit showMessage(current_message_);
}

