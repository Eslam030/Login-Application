#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <regex>
#include <algorithm>
#include <QFile>
#include <QMouseEvent>
#include <QKeyEvent>


#define Q(i) QString::fromStdString(i)
#define S(i) i.toStdString()


using namespace std ;

// Counter for the times that the user allowed to enter wrong credentials
int counter = 0;

QString red             = "QProgressBar{width : 10px ;}QProgressBar::chunk{background-color : #ff0033; width : 5px ;}"
      , yellow          = "QProgressBar::chunk{background-color : #fff900; width : 10px ;}"
      , green           = "QProgressBar::chunk{background-color : #38761d; width : 10px ;}"
      , font_style      = "QLabel{font-family : Times New Roman ;font : bold ;font-size : 16px ;}"
      , lev_1           = "QLineEdit{color : #fde3d0; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}"
      , lev_2           = "QLineEdit{color : #fbc8a1; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}"
      , lev_3           = "QLineEdit{color : #f9ac71; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}"
      , lev_4           = "QLineEdit{color : #f79142; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}"
      , lev_5           = "QLineEdit{color : #f57513; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}"
      , lineRed         = "QLineEdit{color : #ff0000; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}"
      , lineGreen       = "QLineEdit{color : green  ; border : 1px solid black ;border-top-left-radius : 5px ;border-bottom-right-radius : 5px ;}";

// Class Data
class Data {
private:
    string user_name ;
    string email ;
    vector<string> password ;
    string information ;
    string Mobile ;
public:
    void set_user_name (string USER){
        user_name = USER ;
    }
    void set_email (string EMAIL){
        email = EMAIL ;
    }
    void set_password (vector<string> PASSWORD){
        password = PASSWORD ;
    }
    void set_info (string INFO){
        information = INFO ;
    }
    void set_Mobile (string MOBILE){
        Mobile = MOBILE ;
    }
    string get_user_name (){
        return user_name ;
    }
    string get_email (){
        return email ;
    }
    vector<string> get_password (){
        return password ;
    }
    string get_info (){
        return information ;
    }
    string get_Mobile (){
        return Mobile ;
    }
};




/*-----------------------------------------------------FUNCTION USED IN THE SYSTEM-----------------------------------------------------------------*/





// Cipher the password
void cipher (string & password){
    for (int i = 0 ; i < password.length() ; i++){
        password[i] += 1 ;
    }
}


// Decipher the password
void decipher (string & password){
    for (int i = 0 ; i < password.length() ; i++){
        password[i] -= 1 ;
    }
}

void analyis_pass_line (string line , vector<string>&pass){
    string get_pass ;
    for (auto i : line){
        if (i == -110){
            get_pass.erase(get_pass.length() - 1 , 1) ;
            get_pass.erase(0 , 1);
            pass.push_back(get_pass);
            get_pass.clear();
            get_pass += i ;
        }else {
            get_pass += i ;
        }
    }
}




// Get the informations from the line
void analyis_line (string line , vector<Data>&data){
    string new_line ;
    Data in ;
    vector<string> for_class ;
    vector<string>pass ;
    int arrange = 0 ;
    for (int i = 0 ; i < line.length() ; i++){
        if (line[i] == -62){
            if (arrange > 0 && arrange != 2){
                new_line.erase(new_line.begin());
            }
            if (new_line.length() > 1){
                if (arrange == 0){
                    for_class.push_back(new_line);
                }
                if (arrange == 1){
                    for_class.push_back(new_line);
                }
                if (arrange == 2){
                    analyis_pass_line(new_line , pass);
                }
                if (arrange == 3){
                    for_class.push_back(new_line);
                }
                arrange ++;
            }
            new_line = "" ;
        }else {
            new_line += line[i] ;
        }
    }
    if (new_line.length() > 1){
        if (arrange == 4){
            new_line.erase(new_line.begin());
            for_class.push_back(new_line);
        }
    }
    for (int i = 0 ; i < for_class.size() ; i++){
        if (i == 0) // 0 to user_name
            in.set_user_name(for_class[i]) ;

        if (i == 1) // 1 to Email
            in.set_email(for_class[i]) ;
            in.set_password(pass);

        if (i == 2) // 2 to Mobile number
            in.set_Mobile(for_class[i]) ;

        if (i == 3) // 3 for the information
            in.set_info(for_class[i]) ;

    }
    data.push_back(in) ;
}




// Load data from the file
void load_Data (vector<Data>&data){
    vector<string>lines ;
    QFile file ("Main info.txt") ;
    file.open(QIODevice::ReadWrite) ;
    QString data_in = file.readAll();
    string str_data_in = S(data_in) , line;
    for (auto i : str_data_in){
        if (i == -30){
            line.erase (0 , 4) ;
            lines.push_back(line);
            line = "" ;
        }else {
            line.push_back(i);
        }
    }
    lines.erase(lines.begin()) ; // To remove the first line which contain User_Names
    for (auto i : lines){
        analyis_line(i , data) ;
    }
    file.close();
}




// To update the info
string Update_info (string file , string user , string new_info){
    string line , new_file ;
    bool start = false ;
    for (auto i : file){
        if (i == -30){
            if (!start){
                new_file += line + "€\r" + "\n";
                line = "" ;
                start = true ;
            }
            else {
                line.erase (0 , 4) ;
                bool exist = false ;
                string inUser , new_line;
                int count = 0;
                for (auto j : line){
                    if (j == -62)
                        count ++ ;
                    if (!exist){
                        if (j == -62){
                            if (inUser == user){
                                exist = true ;
                            }
                        }else {
                            inUser += j ;
                        }
                    }
                    if (exist){
                        if (count < 4 ){
                            new_line += j ;
                        }
                    }else {
                        new_line += j ;
                    }
                }
                if (exist){
                    new_line += "£"+new_info ;
                }
                new_file += new_line + "€\r" + "\n" ;
                line = "" ;
            }
        }else {
            line += i ;
        }
    }
    return new_file;
}




//Check User existence in the file
bool isUserExist (string file , string user){
    string line ;
    bool start = false , exist = false ;
    for (auto i : file){
        if (i == -30){
            if (!start){
                line = "" ;
                start = true ;
            }
            else {
                line.erase(0 , 4) ;
                string inUser , new_line;
                for (auto j : line){
                    if (!exist){
                        if (j == -62){
                            if (inUser == user){
                                exist = true ;
                            }
                            break;
                        }else {
                            inUser += j ;
                        }
                    }
                }
                line = "" ;
            }
        }
        else {
            line += i ;
        }
    }
    return exist;
}




bool check_pass_exist(string file , string user , string password){
    string line , new_file ;
    bool start = false ;
    for (auto i : file){
        if (i == -30){
            if (!start){
                line = "" ;
                start = true ;
            }
            else {
                bool exist = false ;
                string pass_line , inUser;
                int count = 0;
                for (auto j : line){
                    if (j == -93)
                        count ++ ;
                    if (!exist){
                        if (j == -62){
                            inUser.erase(0 ,4);
                            if (inUser == user){
                                exist = true ;
                            }
                        }
                        else {
                            inUser += j ;
                        }
                    }
                    if (exist){
                         if (count == 2){
                             pass_line += j ;
                         }

                    }
                }
                if (pass_line.length() > 0){
                    pass_line += -93 ;
                    vector<string>pass;
                    analyis_pass_line(pass_line , pass);
                    for (auto i : pass){
                        if (password == i) {
                            return true ;
                        }
                    }
                }
                line = "";
            }
        }
        else {
            line += i ;
        }
    }
    return false ;
}




// Change password
string change_pass (string file , string user , string pass){
    string line , new_file ;
    cipher(pass);
    bool start = false ;
    for (auto i : file){
        if (i == -30){
            if (!start){
                new_file += line + "€\r" + "\n"  ;
                line = "" ;
                start = true ;
            }
            else {
                line.erase(0 , 4);
                bool exist = false , append = true , start_again = false;
                string inUser , new_line;
                int count = 0;
                for (auto j : line){
                    if (j == -93)
                        count ++ ;
                    if (!exist){
                        if (j == -62){
                            if (inUser == user){
                                exist = true ;
                            }
                        }
                        else {
                            inUser += j ;
                        }
                    }
                    if (exist){
                        if (count < 2){
                            new_line += j ;
                        }else if (count == 2){
                            new_line += j ;
                        }else if (count >= 3){
                            if (append){
                                new_line.erase(new_line.length() - 1 , 1) ;
                                new_line += pass ;
                                new_line += "ƒ£" ;
                                append = false ;
                            }
                            if (start_again)
                                new_line += j ;
                            else
                                start_again = true ;
                        }
                    }else {
                        new_line += j ;
                    }
                }
                new_file += new_line + "€\r" + "\n" ;
                line = "" ;
            }
        }else {
            line += i ;
        }
    }
    return new_file;
}




// Check password strength
int password_strength (string pass){
    bool upper = false , lower = false , num = false , special = false ;
    int counter = 0 ;
    if (pass.length() >= 9){
        counter ++ ;
    }
    for (int i = 0 ; i < pass.length() ; i++){
        if (pass[i] >= 65 && pass[i] <= 90){
            if (!upper)
                counter ++ ;
            upper = true ;

        }
        if (pass[i] >= 97 && pass[i] <= 122){
            if (!lower)
                counter ++ ;
            lower = true ;

        }
        if (pass[i] >= 48 && pass[i] <= 57){
            if (!num)
                counter ++ ;
            num = true ;

        }
        if ((pass[i] >=33 && pass[i] <=47) || (pass[i] >=58 && pass[i] <= 64) ||
            (pass[i] >= 91 && pass[i] <=96) || (pass[i] >=123 && pass[i] <=126)){
            if (!special)
                counter ++ ;
            special = true ;

        }
    }
    if (pass.length() < 9 && counter >= 3){
        counter = 2 ;
    }
    return counter ;
}




// Check user name existence
bool user_name_existence(string user_name , vector<Data>data){
    for (auto i : data){
        if (i.get_user_name() == user_name){
            return true ;
        }
    }
    return false ;
}




// Check user_name Validation
bool user_validation (string user){
    regex check ("[a-zA-Z-_0-9 ]+") ;
    return regex_match(user , check) ;
}




// Check email existence
bool email_existence(string email , vector<Data>data){
    for (auto i : data){
        if (i.get_email() == email){
            return true ;
        }
    }
    return false ;
}










// Check e-mail Validation
bool email_validation (string email){
    string Domain , local_part ;
    if (email.length() > 1){
        bool after = false , dot = false , space = false , local = false , domain = false;
        for (int i = 0 ; i < email.length() ; i++){
            if (after){
                Domain += email[i] ;
            }else {
                if (email[i] == '.' and email[i + 1] == '.'){
                    dot = true ;
                    break ;
                }
                if (email[i] == ' '){
                    space = true ;
                    break ;
                }
                local_part += email[i] ;
            }if (email[i] == '@'){
                after = true ;
            }
        }
        local_part.erase(local_part.length() - 1 ,  1) ;
        if (not dot && local_part.length() <= 64 && local_part[0] != '.' && local_part[local_part.length() - 1] != '.' and not space){
            local = true ;
        }
        vector<string>Domains = {"gmail.com" , "outlook.com" , "yahoo.com"} ;
        for (auto i : Domains){
            if (i == Domain){
                domain = true ;
                break ;
            }
        }
        if (local && domain){
            return true ;
        }else {
            return false ;
        }
    }else {
        return false ;
    }
}




// Check Mobile existence
bool mobile_existence(string mobile , vector<Data>data){
    for (auto i : data){
        if (i.get_Mobile() == mobile){
            return true ;
        }
    }
    return false ;
}




// Check mobile number validation
bool mobile_validation (string mobile){
    regex check  ("01[0-2-5][0-9]{8}") ;
    return regex_match(mobile , check) ;
}








/*-----------------------------------------------------END FUNCTION USED IN THE SYSTEM-------------------------------------------------------------*/




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->L_to_main->hide();
    ui->R_to_main->hide();
    ui->ChangePass->hide();
    ui->Logout->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_16->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->label_20->hide();
    ui->label_21->hide();
    ui->label_22->hide();
    ui->label_23->hide();
    ui->label_24->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    ui->label_27->hide();
    ui->label_28->hide();
    ui->label_29->hide();
    ui->label_30->hide();
    ui->label_31->hide();
    ui->label_32->hide();
    ui->label_33->hide();
    ui->label_34->hide();
    ui->label_35->hide();
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    ui->lineEdit_5->hide();
    ui->lineEdit_6->hide();
    ui->lineEdit_7->hide();
    ui->lineEdit_8->hide();
    ui->lineEdit_9->hide();
    ui->lineEdit_10->hide();
    ui->lineEdit_11->hide();
    ui->textEdit->hide();
    ui->Show_pass->hide();
    ui->Show_pass_2->hide();
    ui->Show_pass_3->hide();
    ui->show_pass_4->hide();
    ui->ChangePass_2->hide();
    ui->menubar->hide();
    ui->statusbar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}




/*-------------------------------------------------------------REGISTER FUNCTION-------------------------------------------------------------------*/






//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                                          To go to the Register window
                                                           ******************************                                                          */


void MainWindow::on_Register_clicked()
{
    if (ui -> Show_pass_2 -> isChecked()){
        ui -> Show_pass_2 -> click() ;
    }
    if (ui -> show_pass_4->isChecked()){
        ui -> show_pass_4 -> click() ;
    }
    ui->Login->hide();
    ui->Register->hide();
    ui->label->show();
    ui->label_3->show();
    ui->label_4->show();
    ui->lineEdit->show();
    ui->label_15->show();
    ui->label_16->show();
    ui->label_17->show();
    ui->label_18->show();
    ui->label_18->clear();
    ui->lineEdit_4->show();
    ui->Show_pass_2->show();
    ui->label_22->show();
    ui->label_23->show();
    ui->label_19->show();
    ui->lineEdit_9->show();
    ui->show_pass_4->show();
    ui->lineEdit_10->show();
    ui->lineEdit_11->show();
    ui->R_to_main->show();
    ui->label_20->show();
    ui->label_21->show();
    ui->label_24->show();
    ui->label_24->clear();
    ui->label_25->show();
    ui->label_25->clear();
    ui->label_26->show();
    ui->label_26->clear();
    ui->label_27->show();
    ui->label_28->show();
    ui->label_29->show();
    ui->label_30->show();
    ui->label_31->show();
    ui->label_32->show();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                                To go to the Main window after register the new user
                                                 ******************************************************                                            */


void MainWindow::on_R_to_main_clicked()
{
    string user_vaild = S(ui->label_18->text())
          ,re_pass    = S(ui->label_24->text())
          ,e_mail     = S(ui->label_25->text())
          ,mobile     = S(ui->label_26->text())
          ,pass ;
    if (ui->Show_pass_2->isChecked()){
        pass = ui->lineEdit_7->S(text());
    }else {
        pass = ui->lineEdit_4->S(text());
    }
    int pass_strength = password_strength(pass) ;
    if (user_vaild == "Valid" && re_pass == "matched" && e_mail == "Valid" && mobile == "Valid" && pass_strength >= 3){
        string new_line , user , email , mobile;
        user = ui->lineEdit->S(text());
        email  = ui->lineEdit_10->S(text());
        mobile = ui->lineEdit_11->S(text());
        cipher(pass);
        ui->Login->show();
        ui->Register->show();
        ui->label->hide();
        ui->lineEdit->hide();
        ui->lineEdit->clear();
        ui->label_15->hide();
        ui->label_16->hide();
        ui->label_17->hide();
        ui->lineEdit_4->hide();
        ui->lineEdit_4->clear();
        ui->lineEdit_7->hide();
         ui->lineEdit_7->clear();
        ui->Show_pass_2->hide();
        ui->label_22->hide();
        ui->label_23->hide();
        ui->label_19->hide();
        ui->lineEdit_9->hide();
        ui->lineEdit_9->clear();
        ui->lineEdit_6->hide();
        ui->lineEdit_6->clear();
        ui->show_pass_4->hide();
        ui->lineEdit_10->hide();
        ui->lineEdit_10->clear();
        ui->lineEdit_11->hide();
        ui->lineEdit_11->clear();
        ui->R_to_main->hide();
        ui->label_20->hide();
        ui->label_21->hide();
        ui->label_24->hide();
        ui->label_24->clear();
        ui->label_25->hide();
        ui->label_25->clear();
        ui->label_26->hide();
        ui->label_26->clear();
        ui->label_27->hide();
        ui->label_28->hide();
        ui->label_29->hide();
        ui->label_30->hide();
        ui->label_31->hide();
        ui->label_32->hide();
        ui->label_18->hide();
        ui->label_18->clear();
        ui->label_3->hide();
        ui->label_4->hide();
        new_line += user + "£" + email + "£" + pass + "ƒ£" + mobile + "£Hey\nYou can type your information there\nEnjoy ^_^€\r" + "\n";
        QFile file ("Main info.txt");
        file.open(QIODevice :: ReadWrite);
        file.readAll();
        QTextStream out (&file);
        out << Q(new_line) ;
        file.close();
    }
    else {
        if (user_vaild != "Valid"){
            if (user_vaild == "Already exist"){
                QMessageBox::warning(this , "Register" , "user name already exist") ;
            }else {
                QMessageBox::warning(this , "Register" , "user name isn't valid") ;
            }
        }
        else if (pass_strength < 3){
            QMessageBox::warning(this , "Register" , "password is week") ;
        }
        else if (re_pass != "matched"){
            QMessageBox::warning(this , "Register" , "both passwords arn't the same") ;
        }
        else if (e_mail != "Valid"){
            if (user_vaild == "Already exist"){
                QMessageBox::warning(this , "Register" , "email already exist") ;
            }else {
                QMessageBox::warning(this , "Register" , "email isn't valid") ;
            }
        }
        else if (mobile != "Valid"){
            if (user_vaild == "Already exist"){
                QMessageBox::warning(this , "Register" , "mobile already exist") ;
            }else {
                QMessageBox::warning(this , "Register" , "mobile isn't valid") ;
            }
        }
    }
}




/*-----------------------------------------------------------REGISTRATION FUNCTION-----------------------------------------------------------------*/




/*------------------------------------------------------------LOGIN FUNCTUION----------------------------------------------------------------------*/






//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                                          To go to the Login window
                                                           ****************************                                                            */


void MainWindow::on_Login_clicked()
{
    if (ui -> Show_pass -> isChecked()){
        ui -> Show_pass -> click() ;
    }
    int x , y , changey = 95 , changex = 10;
    ui->Login->hide();
    ui->Register->hide();

    ui->label_4->show();
    x = ui->label_4->x();
    y = ui->label_4->y();
    ui->label_4->move(x+changex, y +changey) ;

    ui->label_33->show();
    x = ui->label_33->x();
    y = ui->label_33->y();
    ui->label_33->move(x+changex, y +changey) ;

    ui->L_to_main->show();
    x = ui->L_to_main->x();
    y = ui->L_to_main->y();
    ui->L_to_main->move(x+changex, y +changey) ;

    ui->Show_pass->show();
    x = ui->Show_pass->x();
    y = ui->Show_pass->y();
    ui->Show_pass->move(x+changex, y +changey) ;

    ui->label->show();
    x = ui->label->x();
    y = ui->label->y();
    ui->label->move(x+changex, y +changey) ;

    ui->label_2->show();
    x = ui->label_2->x();
    y = ui->label_2->y();
    ui->label_2->move(x+changex, y +changey) ;

    ui->lineEdit->show();
    x = ui->lineEdit->x();
    y = ui->lineEdit->y();
    ui->lineEdit->move(x+changex, y +changey) ;

    ui->lineEdit_2->show();
    x = ui->lineEdit_2->x();
    y = ui->lineEdit_2->y();
    ui->lineEdit_2->move(x+changex, y +changey) ;

    x = ui->lineEdit_3->x();
    y = ui->lineEdit_3->y();
    ui->lineEdit_3->move(x+changex, y +changey) ;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                                          To go to the logged in window
                                                           *******************************                                                         */


void MainWindow::on_L_to_main_clicked()
{
    vector<Data>data ;
    load_Data(data) ;
    counter ++ ;
    QString QUser_name , pass  , info , QE_mail , QMobile , QHello;
    string Mobile = "Your Mobile is : " , E_mail = "You E-mail is : " , Hello = "Hello " + S(QUser_name);
    string Right_pass;
    bool found = false ;
    QUser_name = ui->lineEdit->text();
    Hello += S(QUser_name);
    for (auto i : data){
        if (i.get_user_name() == S(QUser_name)){
            found = true ;
            vector<string>passwords ;
            passwords = i.get_password();
            Right_pass = passwords[passwords.size()-1];
            E_mail += i.get_email();
            Mobile += i .get_Mobile();
            info   = Q(i.get_info());
        }
    }
    QMobile = Q(Mobile) ;
    QE_mail = Q(E_mail) ;
    QHello =  Q(Hello);
    if (ui->Show_pass->isChecked()){
        pass = ui->lineEdit_3->text();
    }else {
        pass = ui->lineEdit_2->text();
    }
    if (counter == 3){
        counter = 0 ;
        QMessageBox::information(this , "Login" , "accsses Denid") ;
        int x , y , changey = -95 , changex = -10;
        ui->L_to_main->hide();
        x = ui->L_to_main->x();
        y = ui->L_to_main->y();
        ui->L_to_main->move(x+changex, y +changey) ;

        ui->Show_pass->hide();
        x = ui->Show_pass->x();
        y = ui->Show_pass->y();
        ui->Show_pass->move(x+changex, y +changey) ;

        ui->label->hide();
        x = ui->label->x();
        y = ui->label->y();
        ui->label->move(x+changex, y +changey) ;

        ui->label_2->hide();
        x = ui->label_2->x();
        y = ui->label_2->y();
        ui->label_2->move(x+changex, y +changey) ;

        ui->label_4->hide();
        x = ui->label_4->x();
        y = ui->label_4->y();
        ui->label_4->move(x+changex, y +changey) ;

        ui->label_33->hide();
        x = ui->label_33->x();
        y = ui->label_33->y();
        ui->label_33->move(x+changex, y +changey) ;

        ui->lineEdit->hide();
        ui->lineEdit->clear();
        x = ui->lineEdit->x();
        y = ui->lineEdit->y();
        ui->lineEdit->move(x+changex, y +changey) ;

        ui->lineEdit_2->hide();
        ui->lineEdit_2->clear();
        x = ui->lineEdit_2->x();
        y = ui->lineEdit_2->y();
        ui->lineEdit_2->move(x+changex, y +changey) ;

        ui->lineEdit_3->hide();
        ui->lineEdit_3->clear();
        x = ui->lineEdit_3->x();
        y = ui->lineEdit_3->y();

        ui->lineEdit_3->move(x+changex, y +changey) ;
        ui->label->hide();
        ui->label_2->hide();
        ui->lineEdit->hide();
        ui->lineEdit->clear();
        ui->lineEdit_2->hide();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->hide();
        ui->lineEdit_3->clear();
        ui->L_to_main->hide();
        ui->Show_pass->hide();
        ui->Login->show();
        ui->Register->show();
    }else {
        decipher(Right_pass) ;
        if (found && S(pass) == Right_pass){
            counter = 0 ;
            int x , y , changey = -95 , changex = -10;
            ui->L_to_main->hide();
            x = ui->L_to_main->x();
            y = ui->L_to_main->y();
            ui->L_to_main->move(x+changex, y +changey) ;

            ui->Show_pass->hide();
            x = ui->Show_pass->x();
            y = ui->Show_pass->y();
            ui->Show_pass->move(x+changex, y +changey) ;

            ui->label->hide();
            x = ui->label->x();
            y = ui->label->y();
            ui->label->move(x+changex, y +changey) ;

            ui->label_2->hide();
            x = ui->label_2->x();
            y = ui->label_2->y();
            ui->label_2->move(x+changex, y +changey) ;

            ui->label_4->hide();
            x = ui->label_4->x();
            y = ui->label_4->y();
            ui->label_4->move(x+changex, y +changey) ;

            ui->label_33->hide();
            x = ui->label_33->x();
            y = ui->label_33->y();
            ui->label_33->move(x+changex, y +changey) ;

            ui->lineEdit->hide();
            ui->lineEdit->clear();
            x = ui->lineEdit->x();
            y = ui->lineEdit->y();
            ui->lineEdit->move(x+changex, y +changey) ;

            ui->lineEdit_2->hide();
            ui->lineEdit_2->clear();
            x = ui->lineEdit_2->x();
            y = ui->lineEdit_2->y();
            ui->lineEdit_2->move(x+changex, y +changey) ;

            ui->lineEdit_3->hide();
            ui->lineEdit_3->clear();
            x = ui->lineEdit_3->x();
            y = ui->lineEdit_3->y();
            ui->lineEdit_3->move(x+changex, y +changey) ;

            ui->ChangePass->show() ;
            ui->Logout->show();
            ui->label_6->show();
            ui->label_6->setText(QMobile);
            ui->label_6->setStyleSheet(font_style);
            ui->label_7->show();
            ui->label_7->setText(QE_mail);
            ui->label_7->setStyleSheet(font_style);
            ui->label_8->show();
            ui->label_8->setStyleSheet(font_style);
            ui->label_12->setText(QHello) ;
            ui->label_12->show();
            ui->label_12->setStyleSheet(font_style);
            ui->textEdit->show();
            ui->textEdit->setText(info) ;

        }else {
            string message = "Dosen't Exist email you still have " + to_string(3 - counter) + " Tries and you will be returned to main menu" ;
            QString Qmessage = Q(message) ;
            QMessageBox::warning(this , "Login" , Qmessage) ;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                                      To Logout and go to MainWindow
                                                       ********************************                                                            */


void MainWindow::on_Logout_clicked()
{
    string user_name = ui->label_12->S(text());
    user_name.erase(0 , 6) ;
    QFile file ("Main info.txt") ;
    file.open(QIODevice::ReadWrite);
    QString Qin = file.readAll() , Qdata = ui->textEdit->toPlainText();
    string in = S(Qin) , data = S(Qdata);
    string file_out = Update_info(in , user_name , data) ;
    QString Qfile_out = Q(file_out);
    file.resize(0) ;
    QTextStream out (&file) ;
    out << Qfile_out ;
    ui->Login->show();
    ui->Register->show();
    ui->ChangePass->hide() ;
    ui->Logout->hide();
    ui->label_6->hide();
    ui->label_6->clear();
    ui->label_7->hide();
    ui->label_7->clear();
    ui->label_8->hide();
    ui->textEdit->hide();
    ui->textEdit->clear();
    ui->label_12->hide() ;
    ui->label_12->clear();
    file.close();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                           To go to the change password window from the logged in window
                                            ***********************************************************                                            */


void MainWindow::on_ChangePass_clicked()
{
    if (ui -> Show_pass -> isChecked()){
        ui -> Show_pass -> click() ;
    }
    if (ui -> Show_pass_2 -> isChecked()){
        ui -> Show_pass_2 -> click() ;
    }
    if (ui -> Show_pass_3 -> isChecked()){
        ui -> Show_pass_3 -> click() ;
    }
    int x , y , changey = 50 , changex = 5;
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->Logout->hide();
    ui->textEdit->hide();
    ui->label_12->hide();
    ui->ChangePass->hide() ;

    ui->label_9->show();
    x = ui->label_9->x();
    y = ui->label_9->y();
    ui->label_9->move(x + changex, y + changey);

    ui->label_33->show();
    x = ui->label_33->x();
    y = ui->label_33->y();
    ui->label_33->move(x + changex, y + changey);

    ui->label_34->show();
    x = ui->label_34->x();
    y = ui->label_34->y();
    ui->label_34->move(x + changex, y + changey);

    ui->label_3->show();
    x = ui->label_3->x();
    y = ui->label_3->y();
    ui->label_3->move(x + changex, y + changey);

    ui->label_35->show();
    ui->label_35->setText("");
    x = ui->label_35->x();
    y = ui->label_35->y();
    ui->label_35->move(x + changex, y + changey);

    ui->label_10->show();
    x = ui->label_10->x();
    y = ui->label_10->y();
    ui->label_10->move(x + changex, y + changey);

    ui->label_11->show();
    x = ui->label_11->x();
    y = ui->label_11->y();
    ui->label_11->move(x + changex, y + changey);

    ui->lineEdit_2->show();
    x = ui->lineEdit_2->x();
    y = ui->lineEdit_2->y();
    ui->lineEdit_2->move(x + changex, y + changey);

    x = ui->lineEdit_3->x();
    y = ui->lineEdit_3->y();
    ui->lineEdit_3->move(x + changex, y + changey);

    ui->lineEdit_4->show();
    x = ui->lineEdit_4->x();
    y = ui->lineEdit_4->y();
    ui->lineEdit_4->move(x + changex, y + changey);

    x = ui->lineEdit_7->x();
    y = ui->lineEdit_7->y();
    ui->lineEdit_7->move(x + changex, y + changey);

    ui->lineEdit_8->show();
    x = ui->lineEdit_8->x();
    y = ui->lineEdit_8->y();
    ui->lineEdit_8->move(x + changex, y + changey);

    x = ui->lineEdit_5->x();
    y = ui->lineEdit_5->y();
    ui->lineEdit_5->move(x + changex, y + changey);

    ui->Show_pass->show();
    x = ui->Show_pass->x();
    y = ui->Show_pass->y();
    ui->Show_pass->move(x + changex, y + changey);

    ui->Show_pass_2->show();
    x = ui->Show_pass_2->x();
    y = ui->Show_pass_2->y();
    ui->Show_pass_2->move(x + changex, y + changey);

    ui->Show_pass_3->show();
    x = ui->Show_pass_3->x();
    y = ui->Show_pass_3->y();
    ui->Show_pass_3->move(x + changex, y + changey);

    ui->ChangePass_2->show();
    x = ui->ChangePass_2->x();
    y = ui->ChangePass_2->y();
    ui->ChangePass_2->move(x + changex, y + changey);

    ui->label_13->show();
    ui->label_13->clear();
    x = ui->label_13->x();
    y = ui->label_13->y();
    ui->label_13->move(x + changex, y + changey);

    ui->label_14->show();
    ui->label_14->clear();
    x = ui->label_14->x();
    y = ui->label_14->y();
    ui->label_14->move(x + changex, y + changey);

}


//-------------------------------------------------------------------------------------------------------------------------------------------------
/*                                      To Return to the logged in window after changing the password
                                       ***************************************************************                                             */


void MainWindow::on_ChangePass_2_clicked()
{
    bool valid = false ;
    string old_pass_check = ui->label_13->S(text())
         , re_pass_check = ui->label_14->S(text()) , user_name , pass;
    if (ui->Show_pass_2->isChecked()){
        pass = ui->lineEdit_7->S(text());
    }else {
        pass = ui->lineEdit_4->S(text());
    }
    int new_pass_check = password_strength(pass) ;
    if (old_pass_check == "Corrret old password" && new_pass_check >= 3 && re_pass_check == "matched"
            && S(ui->label_35->text()) == "Valid"){
        valid = true ;
        user_name = ui->label_12->S(text());
        user_name.erase(0 , 6) ;
        int x , y , changey = -50 , changex = -5;

        ui->label_9->hide();
        x = ui->label_9->x();
        y = ui->label_9->y();
        ui->label_9->move(x + changex, y + changey);

        ui->label_10->hide();
        x = ui->label_10->x();
        y = ui->label_10->y();
        ui->label_10->move(x + changex, y + changey);

        ui->label_11->hide();
        x = ui->label_11->x();
        y = ui->label_11->y();
        ui->label_11->move(x + changex, y + changey);

        ui->lineEdit_2->hide();
        ui->lineEdit_2->clear();
        x = ui->lineEdit_2->x();
        y = ui->lineEdit_2->y();
        ui->lineEdit_2->move(x + changex, y + changey);

        ui->lineEdit_3->hide();
        ui->lineEdit_3->clear();
        x = ui->lineEdit_3->x();
        y = ui->lineEdit_3->y();
        ui->lineEdit_3->move(x + changex, y + changey);

        ui->lineEdit_4->hide();
        ui->lineEdit_4->clear();
        x = ui->lineEdit_4->x();
        y = ui->lineEdit_4->y();
        ui->lineEdit_4->move(x + changex, y + changey);

        ui->lineEdit_5->hide();
        ui->lineEdit_5->clear();
        x = ui->lineEdit_5->x();
        y = ui->lineEdit_5->y();
        ui->lineEdit_5->move(x + changex, y + changey);

        ui->lineEdit_7->hide();
        ui->lineEdit_7->clear();
        x = ui->lineEdit_7->x();
        y = ui->lineEdit_7->y();
        ui->lineEdit_7->move(x + changex, y + changey);

        ui->lineEdit_8->hide();
        ui->lineEdit_8->clear();
        x = ui->lineEdit_8->x();
        y = ui->lineEdit_8->y();
        ui->lineEdit_8->move(x + changex, y + changey);


        ui->Show_pass->hide();
        x = ui->Show_pass->x();
        y = ui->Show_pass->y();
        ui->Show_pass->move(x + changex, y + changey);

        ui->Show_pass_2->hide();
        x = ui->Show_pass_2->x();
        y = ui->Show_pass_2->y();
        ui->Show_pass_2->move(x + changex, y + changey);

        ui->Show_pass_3->hide();
        x = ui->Show_pass_3->x();
        y = ui->Show_pass_3->y();
        ui->Show_pass_3->move(x + changex, y + changey);

        ui->ChangePass_2->hide();
        x = ui->ChangePass_2->x();
        y = ui->ChangePass_2->y();
        ui->ChangePass_2->move(x + changex, y + changey);

        ui->label_13->hide();
        ui->label_13->clear();
        x = ui->label_13->x();
        y = ui->label_13->y();
        ui->label_13->move(x + changex, y + changey);

        ui->label_14->hide();
        ui->label_14->clear();
        x = ui->label_14->x();
        y = ui->label_14->y();
        ui->label_14->move(x + changex, y + changey);

        ui->label_33->hide();
        x = ui->label_33->x();
        y = ui->label_33->y();
        ui->label_33->move(x + changex, y + changey);

        ui->label_34->hide();
        x = ui->label_34->x();
        y = ui->label_34->y();
        ui->label_34->move(x + changex, y + changey);

        ui->label_3->hide();
        x = ui->label_3->x();
        y = ui->label_3->y();
        ui->label_3->move(x + changex, y + changey);

        ui->ChangePass->show() ;
        ui->Logout->show();
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->textEdit->show();

        ui->label_35->hide();
        ui->label_35->setText("");
        x = ui->label_35->x();
        y = ui->label_35->y();
        ui->label_35->move(x + changex, y + changey);

        QFile file ("Main info.txt") ;

        file.open(QIODevice::ReadWrite);
        QString Qin = file.readAll() ;
        string in = S(Qin) ;
        string file_out = change_pass(in , user_name , pass) ;
        QString Qfile_out = Q(file_out);
        file.resize(0);
        QTextStream out (&file) ;
        out << Qfile_out ;
        file.close();
    }else {
        if (old_pass_check != "Corrret old password"){
            QMessageBox::warning(this , "Change password" , "Old pass isn't exist");
        }else if (new_pass_check < 3) {
            QMessageBox::warning(this , "Change password" , "New password is very week");
        }else {
            if (S(ui->label_35->text()) != "Already exist")
                QMessageBox::warning(this , "Change password" , "the new password is already exist");
            else
                QMessageBox::warning(this , "Change password" , "both passwords are't the same");
        }
    }
}




/*----------------------------------------------------------END LOGIN FUNCTION---------------------------------------------------------------------*/








/*------------------------------------------------------------RADIO BUTTONS------------------------------------------------------------------------*/




//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Show_pass for showing the Old Password in change password
                                           And showing the password in login
                                      ************************************************                                                             */


void MainWindow::on_Show_pass_toggled(bool checked)
{
    QString info ;
    if (checked){
        info = ui->lineEdit_2 ->text() ;
        ui->lineEdit_3->setText(info) ;
        ui->lineEdit_2->hide();
        ui->lineEdit_3->show();
    }
    else{
        info = ui->lineEdit_3 ->text() ;
        ui->lineEdit_2->setText(info) ;
        ui->lineEdit_3->hide();
        ui->lineEdit_2->show();
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Show_pass for showing the New Password in change password
                                           And showing the password in register
                                      ************************************************                                                             */


void MainWindow::on_Show_pass_2_toggled(bool checked)
{
    QString info ;
    if (checked){
        info = ui->lineEdit_4 ->text() ;
        ui->lineEdit_7->setText(info) ;
        ui->lineEdit_4->hide();
        ui->lineEdit_7->show();
    }
    else{
        info = ui->lineEdit_7 ->text() ;
        ui->lineEdit_4->setText(info) ;
        ui->lineEdit_7->hide();
        ui->lineEdit_4->show();
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Show_pass for showing the Re_Password in change password
                                      ************************************************                                                             */


void MainWindow::on_Show_pass_3_toggled(bool checked)
{
    QString info ;
    if (checked){

        info = ui->lineEdit_8 ->text() ;
        ui->lineEdit_5->setText(info) ;
        ui->lineEdit_8->hide();
        ui->lineEdit_5->show();
    }
    else{
        info = ui->lineEdit_5 ->text() ;
        ui->lineEdit_8->setText(info) ;
        ui->lineEdit_5->hide();
        ui->lineEdit_8->show();
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Show_pass for showing the Re_Password in register
                                      ************************************************                                                             */


void MainWindow::on_show_pass_4_toggled(bool checked)
{
    QString info ;
    if (checked){
        info = ui->lineEdit_9 ->text() ;
        ui->lineEdit_6->setText(info) ;
        ui->lineEdit_9->hide();
        ui->lineEdit_6->show();
    }
    else{
        info = ui->lineEdit_6 ->text() ;
        ui->lineEdit_9->setText(info) ;
        ui->lineEdit_6->hide();
        ui->lineEdit_9->show();
    }
}




/*--------------------------------------------------------------END RADIO BUTTONS------------------------------------------------------------------*/








/*----------------------------------------------------------------LINE EDITS-----------------------------------------------------------------------*/




//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Line Edit for Taking the user name for the user in regitser and login
                                      ************************************************                                                             */


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (!ui->L_to_main->isHidden()){
        if (S(arg1).empty())
            ui->label_4->show();
        else{
            ui->label_4->hide() ;
        }
    }
    if (!ui->R_to_main->isHidden()){
        if (S(arg1).length() > 0){
            ui->label_18->show();
            ui->label_4->hide();
        }else {
            ui->label_18->setText("Invalid");
            ui->label_18->setStyleSheet("QLabel {color : red ;}") ;
            ui->label_18->hide();
            ui->label_4->show();
        }
        vector<Data>data;
        load_Data(data);
        bool exist , valid ;
        if (S(arg1).length() == 0){
            ui->label_4->show();
//            ui->lineEdit->setStyleSheet(lineBorderBlack) ;
        }else {
            ui->label_4->hide();
            exist = user_name_existence(S(arg1) , data) ;
            valid = user_validation(S(arg1)) ;
            if (!exist && valid){
                ui->label_18->show();
                ui->label_18->setText("Valid");
//                ui->lineEdit->setStyleSheet(lineBorderGreen) ;
                ui->label_18->setStyleSheet("QLabel {color : green ;}") ;
            }else {
//                    ui->lineEdit->setStyleSheet(lineBorderRed) ;
                if (exist){
                    ui->label_18->show();
                    ui->label_18->setText("Already exist");
                    ui->label_18->setStyleSheet("QLabel {color : red ;}") ;
                }
                if (!valid){
                    ui->label_18->show();
                    ui->label_18->setText("Invalid");
                    ui->label_18->setStyleSheet("QLabel {color : red ;}") ;
                }
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                 Line Edit 2 for Taking the hidden password from the user in login
                                     And Taking the hidden old password in change password
                                    *******************************************************                                                        */


void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (!ui->L_to_main->isHidden()){
        if (S(arg1).length() > 0)
            ui->label_33->hide();
        else
            ui->label_33->show();
    }
    if (!ui->ChangePass_2->isHidden()){
        if (S(arg1).length() > 0){
            ui->label_13->show();
            ui->label_33->hide();
        }else {
            ui->label_13->hide();
            ui->label_33->show();
        }
        string user_name = ui->label_12->S(text()) , pass = S(arg1);
        cipher(pass);
        user_name.erase(0 , 6) ;
        vector<Data>data ;
        load_Data(data);
        for (auto i : data){
            vector<string>passwords ;
            passwords = i.get_password();
            if (i.get_user_name() == user_name && passwords[passwords.size()-1] == pass){
                ui->label_13->setText("Corrret old password");
                ui->label_13->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
                break;
            }else {
                ui->label_13->setText("inCorrret old password");
                ui->label_13->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                 Line Edit 3 for Taking the shown password from the user in login
                                     And Taking the shown old password in change password
                                    *******************************************************                                                        */


void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (!ui->L_to_main->isHidden()){
        if (S(arg1).length() > 0)
            ui->label_33->hide();
        else
            ui->label_33->show();

    }
    if (!ui->ChangePass_2->isHidden()){
        if (S(arg1).length() > 0){
            ui->label_13->show();
            ui->label_33->hide();
        }else {
            ui->label_13->hide();
            ui->label_33->show();
        }
        string user_name = ui->label_12->S(text()) , pass = S(arg1);
        cipher(pass);
        user_name.erase(0 , 6) ;
        vector<Data>data ;
        load_Data(data);
        for (auto i : data){
            vector<string>passwords ;
            passwords = i.get_password();
            if (i.get_user_name() == user_name && passwords[passwords.size()-1] == pass){
                ui->label_13->setText("Corrret old password");
                ui->label_13->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
                break;
            }else {
                ui->label_13->setText("inCorrret old password");
                ui->label_13->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Line Edit 4 for Taking the hidden New_password from the userin change password
                                            And the hidden password from the user in register
                                          *****************************************************                                                    */


void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    if (!ui->ChangePass_2->isHidden() || !ui->R_to_main->isHidden()){
        string user_name = ui->label_12->S(text()) , pass = S(arg1);
        if (pass.length() > 0){
            ui->label_35->show();
        }else {
            ui->label_35->hide();
            ui->label_14->hide();
            ui->lineEdit_5->setStyleSheet(lineRed) ;
            ui->lineEdit_8->setStyleSheet(lineRed) ;
            ui->lineEdit_6->setStyleSheet(lineRed) ;
            ui->lineEdit_9->setStyleSheet(lineRed) ;
        }
        cipher(pass);
        user_name.erase(0 , 6) ;
        vector<Data>data ;
        load_Data(data);
        string password = S(arg1) ;
        bool exist = false ;
        for (auto i : data){
                if (i.get_user_name() == user_name){
                vector<string>passwords;
                passwords = i.get_password() ;
                for (auto j : passwords){
                    if (j == pass){
                        exist = true ;
                        break ;
                    }
                }
            }
        }
        int power_of_password ;
        power_of_password = password_strength(pass) ;
        if (power_of_password >= 3){
            if (!exist){
                ui->label_35->setText("Valid") ;
                ui->label_35->setStyleSheet("QLabel {color : green ;}") ;
            }else {
                ui->label_35->setText("Alredy exist") ;
                ui->label_35->setStyleSheet("QLabel {color : red ;}") ;
            }
        }else{
            ui->label_35->setText("Not Valid") ;
            ui->label_35->setStyleSheet("QLabel {color : red ;}") ;
        }

        if (pass.length() > 0){
            if (power_of_password == 1){
                ui->lineEdit_4->setStyleSheet(lev_1);
                ui->label_3->hide();
            }
            else if (power_of_password == 2){
                ui->lineEdit_4->setStyleSheet(lev_2);
            }
            else if (power_of_password == 3){
                ui->lineEdit_4->setStyleSheet(lev_3);
            }
            else if (power_of_password == 4){
                ui->lineEdit_4->setStyleSheet(lev_4);
            }
            else if (power_of_password == 5){
                ui->lineEdit_4->setStyleSheet(lev_5);
            }
        }else {
            ui->label_3->show();
        }
    }
    if (!ui->ChangePass_2->isHidden()){
        if (S(arg1).length() > 0 && ui->label_35->text() == "Valid"){
            ui->label_14->show();
        }else {
            ui->label_14->hide();
        }
        string check = ui->label_14->S(text());
        if ((check == "matched" || check == "Don't match")){
            QString Qmain_pass;
            if (ui->Show_pass_3->isChecked()){
                Qmain_pass = ui->lineEdit_5->text();
            }else {
                Qmain_pass = ui->lineEdit_8->text();
            }
            if (arg1 == Qmain_pass){
                ui->label_14->setText("matched");
                ui->label_14->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
                ui->lineEdit_5->setStyleSheet(lineGreen) ;
                ui->lineEdit_8->setStyleSheet(lineGreen) ;
            }else {
                ui->label_14->setText("Don't match");
                ui->label_14->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
                ui->lineEdit_5->setStyleSheet(lineRed) ;
                ui->lineEdit_8->setStyleSheet(lineRed) ;
            }
        }
        if (S(ui->label_35->text()) == "Valid")
            ui->label_14->show();
        else
            ui->label_14->hide();
    }

    if (!ui->R_to_main->isHidden()){
        QString check_pass ;
        if (ui->show_pass_4->isChecked())
            check_pass = ui->lineEdit_6->text() ;
        else
            check_pass = ui->lineEdit_9->text() ;
        if (S(arg1).length() > 0 && S(check_pass).length() > 0){
            ui->label_24->show();
            string check_2 = ui->label_24->S(text());
            if (check_2 == "matched" || check_2 == "Don't match"){
                QString Qmain_pass;
                if (ui->show_pass_4->isChecked()){
                    Qmain_pass = ui->lineEdit_6->text();
                }else {
                    Qmain_pass = ui->lineEdit_9->text();
                }
                if (arg1 == Qmain_pass){
                    ui->label_24->setText("matched");
                    ui->label_24->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
                    ui->lineEdit_6->setStyleSheet(lineGreen) ;
                    ui->lineEdit_9->setStyleSheet(lineGreen) ;
                }else {
                    ui->label_24->setText("Don't match");
                    ui->label_24->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
                    ui->lineEdit_6->setStyleSheet(lineRed) ;
                    ui->lineEdit_9->setStyleSheet(lineRed) ;
                }
            }
        }else {
            ui->label_24->hide();
        }
        if (!ui->label_35->isHidden() && ui->label_35->text() == "Valid" && ui->label_24->text() == "matched")
            ui->label_24->show();
        else
            ui->label_24->hide();
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                    Line Edit 5 for Taking the shown Re_password from the user in change password
                                            ************************************************                                                       */


void MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{
    if (S(arg1).length() > 0){
        ui->label_14->show();
        ui->label_34->hide();
    }else {
        ui->label_14->hide();
        ui->label_34->show();
    }
    QString check_pass ;
    if (ui->Show_pass_2->isChecked())
        check_pass = ui->lineEdit_7->text();
    else
        check_pass = ui->lineEdit_4->text();
    if(S(check_pass).empty()){
        ui->label_14->hide();
        ui->lineEdit_5->setStyleSheet(lineRed);
        ui->lineEdit_8->setStyleSheet(lineRed);
    }else {
        QString Qmain_pass;
        if (ui->Show_pass_2->isChecked()){
            Qmain_pass = ui->lineEdit_7->text();
        }else {
            Qmain_pass = ui->lineEdit_4->text();
        }
        if (arg1 == Qmain_pass){
            ui->label_14->setText("matched");
            ui->label_14->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_5->setStyleSheet(lineGreen);
            ui->lineEdit_8->setStyleSheet(lineGreen);
        }else {
            ui->label_14->setText("Don't match");
            ui->label_14->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_5->setStyleSheet(lineRed);
            ui->lineEdit_8->setStyleSheet(lineRed);
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                  Line Edit 6 for Taking the shown Re_password from the user in register
                                          ************************************************                                                         */


void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
    if (S(arg1).length() > 0){
        ui->label_24->show();
        ui->label_30->hide() ;
    }else {
        ui->label_24->hide();
        ui->label_30->show() ;
    }
    QString Qmain_pass;
    if (ui->Show_pass_2->isChecked()){
        Qmain_pass = ui->lineEdit_7->text();
    }else {
        Qmain_pass = ui->lineEdit_4->text();
    }
    QString check_pass ;
    if (ui->Show_pass_2->isChecked())
        check_pass = ui->lineEdit_7->text();
    else
        check_pass = ui->lineEdit_4->text();
    if (!S(check_pass).empty()){
        if (arg1 == Qmain_pass){
            ui->label_24->setText("matched");
            ui->label_24->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_6->setStyleSheet(lineGreen) ;
            ui->lineEdit_9->setStyleSheet(lineGreen) ;
        }else {
            ui->label_24->setText("Don't match");
            ui->label_24->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_6->setStyleSheet(lineRed) ;
            ui->lineEdit_9->setStyleSheet(lineRed) ;
        }
    }else {
        ui->label_24->hide();
        ui->lineEdit_6->setStyleSheet(lineRed) ;
        ui->lineEdit_9->setStyleSheet(lineRed) ;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Line Edit 7 for Taking the shown New_password from the userin change password
                                            And the shown password from the user in register
                                          *****************************************************                                                    */


void MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{
    if (!ui->ChangePass_2->isHidden() || !ui->R_to_main->isHidden()){
        string user_name = ui->label_12->S(text()) , pass = S(arg1);
        if (pass.length() > 0){
            ui->label_35->show();
        }else {
            ui->label_35->hide();
            ui->label_14->hide();
            ui->lineEdit_5->setStyleSheet(lineRed) ;
            ui->lineEdit_8->setStyleSheet(lineRed) ;
            ui->lineEdit_6->setStyleSheet(lineRed) ;
            ui->lineEdit_9->setStyleSheet(lineRed) ;
        }
        cipher(pass);
        user_name.erase(0 , 6) ;
        vector<Data>data ;
        load_Data(data);
        string password = S(arg1) ;
        bool exist = false ;
        for (auto i : data){
                if (i.get_user_name() == user_name){
                vector<string>passwords;
                passwords = i.get_password() ;
                for (auto j : passwords){
                    if (j == pass){
                        exist = true ;
                        break ;
                    }
                }
            }
        }
        int power_of_password ;
        power_of_password = password_strength(pass) ;
        if (power_of_password >= 3){
            if (!exist){
                ui->label_35->setText("Valid") ;
                ui->label_35->setStyleSheet("QLabel {color : green ;}") ;
            }else {
                ui->label_35->setText("Alredy exist") ;
                ui->label_35->setStyleSheet("QLabel {color : red ;}") ;
            }
        }else
            ui->label_35->hide();
        if (pass.length() > 0){
            if (power_of_password == 1){
                ui->lineEdit_7->setStyleSheet(lev_1);
                ui->label_3->hide();
            }
            else if (power_of_password == 2){
                ui->lineEdit_7->setStyleSheet(lev_2);
            }
            else if (power_of_password == 3){
                ui->lineEdit_7->setStyleSheet(lev_3);
            }
            else if (power_of_password == 4){
                ui->lineEdit_7->setStyleSheet(lev_4);
            }
            else if (power_of_password == 5){
                ui->lineEdit_7->setStyleSheet(lev_5);
            }
        }else {
            ui->label_3->show();
        }
    }
    if (!ui->ChangePass_2>isHidden()){
        if (S(arg1).length() > 0){
            ui->label_14->show();
        }else {
            ui->label_14->hide();
        }
        string check = ui->label_14->S(text());
        if ((check == "matched" || check == "Don't match")){
            QString Qmain_pass;
            if (ui->Show_pass_3->isChecked()){
                Qmain_pass = ui->lineEdit_5->text();
            }else {
                Qmain_pass = ui->lineEdit_8->text();
            }
            if (arg1 == Qmain_pass){
                ui->label_14->setText("matched");
                ui->label_14->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
                ui->lineEdit_5->setStyleSheet(lineGreen) ;
                ui->lineEdit_8->setStyleSheet(lineGreen) ;
            }else {
                ui->label_14->setText("Don't match");
                ui->label_14->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
                ui->lineEdit_5->setStyleSheet(lineRed) ;
                ui->lineEdit_8->setStyleSheet(lineRed) ;
            }
            if (S(ui->label_35->text()) == "Valid")
                ui->label_14->show();
            else
                ui->label_14->hide();
        }

    }
    if (!ui->R_to_main->isHidden()){
        QString check_pass ;
        if (ui->show_pass_4->isChecked())
            check_pass = ui->lineEdit_6->text() ;
        else
            check_pass = ui->lineEdit_9->text() ;
        if (S(arg1).length() > 0 && S(check_pass).length() > 0){
            ui->label_24->show();
            string check_2 = ui->label_24->S(text());
            if (check_2 == "matched" || check_2 == "Don't match"){
                QString Qmain_pass;
                if (ui->show_pass_4->isChecked()){
                    Qmain_pass = ui->lineEdit_6->text();
                }else {
                    Qmain_pass = ui->lineEdit_9->text();
                }
                if (arg1 == Qmain_pass){
                    ui->label_24->setText("matched");
                    ui->label_24->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
                    ui->lineEdit_6->setStyleSheet(lineGreen) ;
                    ui->lineEdit_9->setStyleSheet(lineGreen) ;
                }else {
                    ui->label_24->setText("Don't match");
                    ui->label_24->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
                    ui->lineEdit_6->setStyleSheet(lineRed) ;
                    ui->lineEdit_9->setStyleSheet(lineRed) ;
                }
            }
        }else {
            ui->label_24->hide();
        }
        if (S(ui->label_35->text()) == "Valid")
            ui->label_24->show();
        else
            ui->label_24->hide();
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                  Line Edit 8 for Taking the hidden Re-Password form the user in change password
                                             **************************************************                                                    */


void MainWindow::on_lineEdit_8_textChanged(const QString &arg1)
{
    if (S(arg1).length() > 0){
        ui->label_14->show();
        ui->label_34->hide();
    }else {
        ui->label_14->hide();
        ui->label_34->show();
    }
    QString check_pass ;
    if (ui->Show_pass_2->isChecked())
        check_pass = ui->lineEdit_7->text();
    else
        check_pass = ui->lineEdit_4->text();
    if(S(check_pass).empty()){
        ui->label_14->hide();
        ui->lineEdit_5->setStyleSheet(lineRed);
        ui->lineEdit_8->setStyleSheet(lineRed);
    }else {
        QString Qmain_pass;
        if (ui->Show_pass_2->isChecked()){
            Qmain_pass = ui->lineEdit_7->text();
        }else {
            Qmain_pass = ui->lineEdit_4->text();
        }
        if (arg1 == Qmain_pass){
            ui->label_14->setText("matched");
            ui->label_14->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_5->setStyleSheet(lineGreen);
            ui->lineEdit_8->setStyleSheet(lineGreen);
        }else {
            ui->label_14->setText("Don't match");
            ui->label_14->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_5->setStyleSheet(lineRed);
            ui->lineEdit_8->setStyleSheet(lineRed);
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                  Line Edit 9 for Taking the hidden Re-Password from the user in regitser
                                            ************************************************                                                       */


void MainWindow::on_lineEdit_9_textChanged(const QString &arg1)
{

    if (S(arg1).length() > 0){
        ui->label_24->show();
        ui->label_30->hide() ;
    }else {
        ui->label_24->hide();
        ui->label_30->show() ;
    }
    QString Qmain_pass;
    if (ui->Show_pass_2->isChecked()){
        Qmain_pass = ui->lineEdit_7->text();
    }else {
        Qmain_pass = ui->lineEdit_4->text();
    }
    QString check_pass ;
    if (ui->Show_pass_2->isChecked())
        check_pass = ui->lineEdit_7->text();
    else
        check_pass = ui->lineEdit_4->text();
    if (!S(check_pass).empty()){
        if (arg1 == Qmain_pass){
            ui->label_24->setText("matched");
            ui->label_24->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_6->setStyleSheet(lineGreen) ;
            ui->lineEdit_9->setStyleSheet(lineGreen) ;
        }else {
            ui->label_24->setText("Don't match");
            ui->label_24->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
            ui->lineEdit_6->setStyleSheet(lineRed) ;
            ui->lineEdit_9->setStyleSheet(lineRed) ;
        }
    }else {
        ui->label_24->hide();
        ui->lineEdit_6->setStyleSheet(lineRed) ;
        ui->lineEdit_9->setStyleSheet(lineRed) ;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                   Line Edit 10 for Taking the e_mail from the user in regitser
                                        ************************************************                                                           */


void MainWindow::on_lineEdit_10_textChanged(const QString &arg1)
{
    if (S(arg1).length() > 0){
        ui->label_25->show();
        ui->label_32->hide();
    }else {
        ui->label_25->hide();
        ui->label_32->show();
    }
    vector<Data>data;
    load_Data(data);
    bool exist , valid ;
    exist = email_existence(S(arg1) , data) ;
    valid = email_validation(S(arg1)) ;
    if (!exist && valid){
        ui->label_25->setText("Valid");
        ui->label_25->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
    }else {
        if (exist){
            ui->label_25->setText("Already exist");
            ui->label_25->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;;}") ;
        }
        if (!valid){
            ui->label_25->setText("Invalid");
            ui->label_25->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
/*                                 Line Edit 11 for Taking the mobile for the user in regitser
                                       ************************************************                                                            */


void MainWindow::on_lineEdit_11_textChanged(const QString &arg1)
{
    if (S(arg1).length() > 0){
        ui->label_26->show();
        ui->label_31->hide();
    }else {
        ui->label_26->hide();
        ui->label_31->show();
    }
    vector<Data>data;
    load_Data(data);
    bool exist , valid ;
    exist = mobile_existence(S(arg1) , data) ;
    valid = mobile_validation(S(arg1)) ;
    if (!exist && valid){
        ui->label_26->setText("Valid");
        ui->label_26->setStyleSheet("QLabel {color : green ;font-family:Times New Roman ; font-size : 12px ;}") ;
    }else {
        if (exist){
            ui->label_26->setText("Already exist");
            ui->label_26->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
        }
        if (!valid){
            ui->label_26->setText("Invalid");
            ui->label_26->setStyleSheet("QLabel {color : red ;font-family:Times New Roman ; font-size : 12px ;}") ;
        }
    }
}




/*-------------------------------------------------------------END LINE EDITS----------------------------------------------------------------------*/



/*--------------------------------------------------------------MOUSE EVENTS-----------------------------------------------------------------------*/




void MainWindow::mousePressEvent(QMouseEvent * ev){
    int x = ev->x() ;
    int y = ev->y() ;
    if ((x >= ui->lineEdit->x() && x <= ui->lineEdit->x() + ui->lineEdit->width() )&& (y >= ui->lineEdit->y() && y <= ui->lineEdit->y() + ui->lineEdit->height())){
        ui->label_4->hide();
        if (! ui->lineEdit->isHidden())
            ui->lineEdit->setFocus();
    }else {
        if (! ui->lineEdit->isHidden())
            ui->lineEdit->clearFocus();
        if (ui->lineEdit->S(text()).length()==0){
            if (! ui->lineEdit->isHidden())
                ui->label_4->show();
        }
    }
    if ((x >= ui->lineEdit_4->x() && x <= ui->lineEdit_4->x() + ui->lineEdit_4->width() )&& (y >= ui->lineEdit_4->y() && y <= ui->lineEdit_4->y() + ui->lineEdit_4->height())){
        ui->label_3->hide();
        if (ui->Show_pass_2->isChecked()){
            if (! ui->lineEdit_7->isHidden())
                ui->lineEdit_7->setFocus();
        }else {
            if (!ui->lineEdit_4->isHidden())
                ui->lineEdit_4->setFocus() ;
        }
    }else {
        if (! ui->lineEdit_4->isHidden())
            ui->lineEdit_4->clearFocus();
        if (! ui->lineEdit_7->isHidden())
            ui->lineEdit_7->clearFocus();
        if (ui->Show_pass_2->isChecked()){
            if (ui->lineEdit_7->S(text()).length()==0){
                if (! ui->lineEdit_7->isHidden())
                    ui->label_3->show();
            }
        }else {
            if (ui->lineEdit_4->S(text()).length()==0){
                if (! ui->lineEdit_4->isHidden())
                    ui->label_3->show();
            }
        }
    }
    if ((x >= ui->lineEdit_6->x() && x <= ui->lineEdit_6->x() + ui->lineEdit_6->width() )&& (y >= ui->lineEdit_6->y() && y <= ui->lineEdit_6->y() + ui->lineEdit_6->height())){
        ui->label_30->hide();
        if (ui->show_pass_4->isChecked()){
            if (! ui->lineEdit_6->isHidden())
                ui->lineEdit_6->setFocus();
        }else{
            if (! ui->lineEdit_9->isHidden())
                ui->lineEdit_9->setFocus();
        }
    }else {
        if (! ui->lineEdit_6->isHidden())
            ui->lineEdit_6->clearFocus();
        if (! ui->lineEdit_9->isHidden())
            ui->lineEdit_9->clearFocus();
        if (ui->show_pass_4->isChecked()){
            if (ui->lineEdit_6->S(text()).length()==0){
                if (! ui->lineEdit_6->isHidden())
                    ui->label_30->show();
            }
        }else {
            if (ui->lineEdit_9->S(text()).length()==0){
                if (! ui->lineEdit_9->isHidden())
                    ui->label_30->show();
            }
        }
    }
    if ((x >= ui->lineEdit_10->x() && x <= ui->lineEdit_10->x() + ui->lineEdit_10->width() )&& (y >= ui->lineEdit_10->y() && y <= ui->lineEdit_10->y() + ui->lineEdit_10->height())){
        ui->label_32->hide();
        if (! ui->lineEdit_10->isHidden())
            ui->lineEdit_10->setFocus();
    }else {
        if (! ui->lineEdit_10->isHidden())
            ui->lineEdit_10->clearFocus();
        if (ui->lineEdit_10->S(text()).length()==0){
            if (! ui->lineEdit_10->isHidden())
                ui->label_32->show();
        }
    }
    if ((x >= ui->lineEdit_11->x() && x <= ui->lineEdit_11->x() + ui->lineEdit_11->width() )&& (y >= ui->lineEdit_11->y() && y <= ui->lineEdit_11->y() + ui->lineEdit_11->height())){
        ui->label_31->hide();
        if (! ui->lineEdit_11->isHidden())
            ui->lineEdit_11->setFocus();
    }else {
        if (! ui->lineEdit_11->isHidden())
            ui->lineEdit_11->clearFocus();
        if (ui->lineEdit_11->S(text()).length()==0){
            if (! ui->lineEdit_11->isHidden())
                ui->label_31->show();
        }
    }
    if ((x >= ui->lineEdit_2->x() && x <= ui->lineEdit_2->x() + ui->lineEdit_2->width() )&& (y >= ui->lineEdit_2->y() && y <= ui->lineEdit_2->y() + ui->lineEdit_2->height())){
        ui->label_33->hide();
        if (ui->Show_pass->isChecked()){
            if (! ui->lineEdit_3->isHidden())
                ui->lineEdit_3->setFocus();
        }else {
            if (!ui->lineEdit_2->isHidden())
                ui->lineEdit_2->setFocus() ;
        }
    }else {
        if (! ui->lineEdit_2->isHidden())
            ui->lineEdit_2->clearFocus();
        if (! ui->lineEdit_3->isHidden())
            ui->lineEdit_3->clearFocus();
        if (ui->Show_pass->isChecked()){
            if (ui->lineEdit_3->S(text()).length()==0){
                if (! ui->lineEdit_3->isHidden())
                    ui->label_33->show();
            }
        }else {
            if (ui->lineEdit_2->S(text()).length()==0){
                if (! ui->lineEdit_2->isHidden())
                    ui->label_33->show();
            }
        }
    }
    if ((x >= ui->lineEdit_5->x() && x <= ui->lineEdit_5->x() + ui->lineEdit_5->width() )&& (y >= ui->lineEdit_5->y() && y <= ui->lineEdit_5->y() + ui->lineEdit_5->height())){
        if (!ui->ChangePass_2->isHidden()){
            ui->label_34->hide();
            if (ui->Show_pass_3->isChecked()){
                if (! ui->lineEdit_5->isHidden())
                    ui->lineEdit_5->setFocus();
            }else {
                if (!ui->lineEdit_8->isHidden())
                    ui->lineEdit_8->setFocus() ;
            }
        }else {
            if (! ui->lineEdit_5->isHidden())
                ui->lineEdit_5->clearFocus();
            if (! ui->lineEdit_8->isHidden())
                ui->lineEdit_8->clearFocus();
            if (ui->Show_pass_3->isChecked()){
                if (ui->lineEdit_5->S(text()).length()==0){
                    if (! ui->lineEdit_5->isHidden())
                        ui->label_34->show();
                }
            }else {
                if (ui->lineEdit_8->S(text()).length()==0){
                    if (! ui->lineEdit_8->isHidden())
                        ui->label_34->show();
                }
            }
        }
    }
}




/*---------------------------------------------------------------BUTTONS---------------------------------------------------------------------------*/




/*---------------------------------------------------------------Back Button-----------------------------------------------------------------------*/




void MainWindow::on_Back_button_clicked()
{
    if (!ui->R_to_main->isHidden()){
        ui->Login->show();
        ui->Register->show();
        ui->label->hide();
        ui->lineEdit->hide();
        ui->lineEdit->clear();
        ui->label_15->hide();
        ui->label_16->hide();
        ui->label_17->hide();
        ui->lineEdit_4->hide();
        ui->lineEdit_4->clear();
        ui->lineEdit_7->hide();
         ui->lineEdit_7->clear();
        ui->Show_pass_2->hide();
        ui->label_22->hide();
        ui->label_23->hide();
        ui->label_19->hide();
        ui->lineEdit_9->hide();
        ui->lineEdit_9->clear();
        ui->lineEdit_6->hide();
        ui->lineEdit_6->clear();
        ui->show_pass_4->hide();
        ui->lineEdit_10->hide();
        ui->lineEdit_10->clear();
        ui->lineEdit_11->hide();
        ui->lineEdit_11->clear();
        ui->R_to_main->hide();
        ui->label_20->hide();
        ui->label_21->hide();
        ui->label_24->hide();
        ui->label_24->clear();
        ui->label_25->hide();
        ui->label_25->clear();
        ui->label_26->hide();
        ui->label_26->clear();
        ui->label_27->hide();
        ui->label_28->hide();
        ui->label_29->hide();
        ui->label_30->hide();
        ui->label_31->hide();
        ui->label_32->hide();
        ui->label_18->hide();
        ui->label_18->clear();
        ui->label_3->hide();
        ui->label_4->hide();
    }
    if (!ui->L_to_main->isHidden()){
        ui->Login->show();
        ui->Register->show();
        int x , y , changey = -95 , changex = -10;
        ui->L_to_main->hide();
        x = ui->L_to_main->x();
        y = ui->L_to_main->y();
        ui->L_to_main->move(x+changex, y +changey) ;

        ui->Show_pass->hide();
        x = ui->Show_pass->x();
        y = ui->Show_pass->y();
        ui->Show_pass->move(x+changex, y +changey) ;

        ui->label->hide();
        x = ui->label->x();
        y = ui->label->y();
        ui->label->move(x+changex, y +changey) ;

        ui->label_2->hide();
        x = ui->label_2->x();
        y = ui->label_2->y();
        ui->label_2->move(x+changex, y +changey) ;

        ui->lineEdit->hide();
        ui->lineEdit->clear();
        x = ui->lineEdit->x();
        y = ui->lineEdit->y();
        ui->lineEdit->move(x+changex, y +changey) ;

        ui->lineEdit_2->hide();
        ui->lineEdit_2->clear();
        x = ui->lineEdit_2->x();
        y = ui->lineEdit_2->y();
        ui->lineEdit_2->move(x+changex, y +changey) ;

        ui->lineEdit_3->hide();
        ui->lineEdit_3->clear();
        x = ui->lineEdit_3->x();
        y = ui->lineEdit_3->y();
        ui->lineEdit_3->move(x+changex, y +changey) ;

        ui->label_4->hide();
        x = ui->label_4->x();
        y = ui->label_4->y();
        ui->label_4->move(x+changex, y +changey) ;

        ui->label_33->hide();
        x = ui->label_33->x();
        y = ui->label_33->y();
        ui->label_33->move(x+changex, y +changey) ;
    }
    if(!ui->ChangePass_2->isHidden()){
        int x , y , changey = -50 , changex = -5;

        ui->label_9->hide();
        x = ui->label_9->x();
        y = ui->label_9->y();
        ui->label_9->move(x + changex, y + changey);

        ui->label_10->hide();
        x = ui->label_10->x();
        y = ui->label_10->y();
        ui->label_10->move(x + changex, y + changey);

        ui->label_11->hide();
        x = ui->label_11->x();
        y = ui->label_11->y();
        ui->label_11->move(x + changex, y + changey);

        ui->lineEdit_2->hide();
        ui->lineEdit_2->clear();
        x = ui->lineEdit_2->x();
        y = ui->lineEdit_2->y();
        ui->lineEdit_2->move(x + changex, y + changey);

        ui->lineEdit_3->hide();
        ui->lineEdit_3->clear();
        x = ui->lineEdit_3->x();
        y = ui->lineEdit_3->y();
        ui->lineEdit_3->move(x + changex, y + changey);

        ui->lineEdit_4->hide();
        ui->lineEdit_4->clear();
        x = ui->lineEdit_4->x();
        y = ui->lineEdit_4->y();
        ui->lineEdit_4->move(x + changex, y + changey);

        ui->lineEdit_5->hide();
        ui->lineEdit_5->clear();
        x = ui->lineEdit_5->x();
        y = ui->lineEdit_5->y();
        ui->lineEdit_5->move(x + changex, y + changey);

        ui->lineEdit_7->hide();
        ui->lineEdit_7->clear();
        x = ui->lineEdit_7->x();
        y = ui->lineEdit_7->y();
        ui->lineEdit_7->move(x + changex, y + changey);

        ui->lineEdit_8->hide();
        ui->lineEdit_8->clear();
        x = ui->lineEdit_8->x();
        y = ui->lineEdit_8->y();
        ui->lineEdit_8->move(x + changex, y + changey);


        ui->Show_pass->hide();
        x = ui->Show_pass->x();
        y = ui->Show_pass->y();
        ui->Show_pass->move(x + changex, y + changey);

        ui->Show_pass_2->hide();
        x = ui->Show_pass_2->x();
        y = ui->Show_pass_2->y();
        ui->Show_pass_2->move(x + changex, y + changey);

        ui->Show_pass_3->hide();
        x = ui->Show_pass_3->x();
        y = ui->Show_pass_3->y();
        ui->Show_pass_3->move(x + changex, y + changey);

        ui->ChangePass_2->hide();
        x = ui->ChangePass_2->x();
        y = ui->ChangePass_2->y();
        ui->ChangePass_2->move(x + changex, y + changey);

        ui->label_13->hide();
        ui->label_13->clear();
        x = ui->label_13->x();
        y = ui->label_13->y();
        ui->label_13->move(x + changex, y + changey);

        ui->label_14->hide();
        ui->label_14->clear();
        x = ui->label_14->x();
        y = ui->label_14->y();
        ui->label_14->move(x + changex, y + changey);

        ui->label_33->hide();
        x = ui->label_33->x();
        y = ui->label_33->y();
        ui->label_33->move(x + changex, y + changey);

        ui->label_34->hide();
        x = ui->label_34->x();
        y = ui->label_34->y();
        ui->label_34->move(x + changex, y + changey);

        ui->label_3->hide();
        x = ui->label_3->x();
        y = ui->label_3->y();
        ui->label_3->move(x + changex, y + changey);

        ui->ChangePass->show() ;
        ui->Logout->show();
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->textEdit->show();

        ui->label_35->hide();
        ui->label_35->setText("");
        x = ui->label_35->x();
        y = ui->label_35->y();
        ui->label_35->move(x + changex, y + changey);
    }
}



/*---------------------------------------------------------------Exit button-----------------------------------------------------------------------*/




void MainWindow::on_Exit_clicked(){
    if (!ui->label_12->S(text()).empty() && (!ui->Logout->isHidden() || !ui->ChangePass_2->isHidden())){
        string user_name = ui->label_12->S(text());
        user_name.erase(0 , 6) ;
        QFile file ("Main info.txt") ;
        file.open(QIODevice::ReadWrite);
        QString Qin = file.readAll() , Qdata = ui->textEdit->toPlainText();
        string in = S(Qin) , data = S(Qdata);
        string file_out = Update_info(in , user_name , data) ;
        QString Qfile_out = Q(file_out);
        file.resize(0) ;
        QTextStream out (&file) ;
        out << Qfile_out ;
        file.close();
    }
    close();
}



/*---------------------------------------------------------------Key events-----------------------------------------------------------------------*/



void MainWindow::keyPressEvent(QKeyEvent * ev){
    if (ev->key() == 16777220 || ev->key() == 16777221){
//        EnterPressed = true ;
        if (!ui->R_to_main->isHidden()){
            string user_vaild = ui->label_18->S(text())
                  ,re_pass    = ui->label_24->S(text())
                  ,e_mail     = ui->label_25->S(text())
                  ,mobile     = ui->label_26->S(text())
                  ,pass ;
            if (ui->Show_pass_2->isChecked()){
                pass = ui->lineEdit_7->S(text());
            }else {
                pass = ui->lineEdit_4->S(text());
            }
            int pass_strength = password_strength(pass) ;
            if (user_vaild == "Valid" && re_pass == "matched" && e_mail == "Valid" && mobile == "Valid" && pass_strength >= 3){
                string new_line , user , email , mobile;
                user = ui->lineEdit->S(text());
                email  = ui->lineEdit_10->S(text());
                mobile = ui->lineEdit_11->S(text());
                cipher(pass);
                ui->Login->show();
                ui->Register->show();
                ui->label->hide();
                ui->lineEdit->hide();
                ui->lineEdit->clear();
                ui->label_15->hide();
                ui->label_16->hide();
                ui->label_17->hide();
                ui->lineEdit_4->hide();
                ui->lineEdit_4->clear();
                ui->lineEdit_7->hide();
                 ui->lineEdit_7->clear();
                ui->Show_pass_2->hide();
                ui->label_22->hide();
                ui->label_23->hide();
                ui->label_19->hide();
                ui->lineEdit_9->hide();
                ui->lineEdit_9->clear();
                ui->lineEdit_6->hide();
                ui->lineEdit_6->clear();
                ui->show_pass_4->hide();
                ui->lineEdit_10->hide();
                ui->lineEdit_10->clear();
                ui->lineEdit_11->hide();
                ui->lineEdit_11->clear();
                ui->R_to_main->hide();
                ui->label_20->hide();
                ui->label_21->hide();
                ui->label_24->hide();
                ui->label_24->clear();
                ui->label_25->hide();
                ui->label_25->clear();
                ui->label_26->hide();
                ui->label_26->clear();
                ui->label_27->hide();
                ui->label_28->hide();
                ui->label_29->hide();
                ui->label_30->hide();
                ui->label_31->hide();
                ui->label_32->hide();
                ui->label_18->hide();
                ui->label_18->clear();
                ui->label_3->hide();
                ui->label_4->hide();
                new_line += user + "£" + email + "£" + pass + "ƒ£" + mobile + "£Hey\nYou can type your information there\nEnjoy ^_^€\r" + "\n";
                QFile file ("Main info.txt");
                file.open(QIODevice :: ReadWrite);
                file.readAll();
                QTextStream out (&file);
                out << Q(new_line) ;
                file.close();
            }
            else {
                if (user_vaild != "Valid"){
                    if (user_vaild == "Already exist"){
                        QMessageBox::warning(this , "Register" , "user name already exist") ;
                    }else {
                        QMessageBox::warning(this , "Register" , "user name isn't valid") ;
                    }
                }
                else if (pass_strength < 3){
                    QMessageBox::warning(this , "Register" , "password is week") ;
                }
                else if (re_pass != "matched"){
                    QMessageBox::warning(this , "Register" , "both passwords arn't the same") ;
                }
                else if (e_mail != "Valid"){
                    if (user_vaild == "Already exist"){
                        QMessageBox::warning(this , "Register" , "email already exist") ;
                    }else {
                        QMessageBox::warning(this , "Register" , "email isn't valid") ;
                    }
                }
                else if (mobile != "Valid"){
                    if (user_vaild == "Already exist"){
                        QMessageBox::warning(this , "Register" , "mobile already exist") ;
                    }else {
                        QMessageBox::warning(this , "Register" , "mobile isn't valid") ;
                    }
                }
            }
        }
        if (!ui->L_to_main->isHidden()){
            vector<Data>data ;
            load_Data(data) ;
            counter ++ ;
            QString QUser_name , pass  , info , QE_mail , QMobile , QHello;
            string Mobile = "Your Mobile is : " , E_mail = "You E-mail is : " , Hello = "Hello " + S(QUser_name);
            string Right_pass;
            bool found = false ;
            QUser_name = ui->lineEdit->text();
            Hello += S(QUser_name);
            for (auto i : data){
                if (i.get_user_name() == S(QUser_name)){
                    found = true ;
                    vector<string>passwords ;
                    passwords = i.get_password();
                    Right_pass = passwords[passwords.size()-1];
                    E_mail += i.get_email();
                    Mobile += i .get_Mobile();
                    info   = Q(i.get_info());
                }
            }
            QMobile = Q(Mobile) ;
            QE_mail = Q(E_mail) ;
            QHello =  Q(Hello);
            if (ui->Show_pass->isChecked()){
                pass = ui->lineEdit_3->text();
            }else {
                pass = ui->lineEdit_2->text();
            }
            if (counter == 3){
                counter = 0 ;
                QMessageBox::information(this , "Login" , "accsses Denid") ;
                int x , y , changey = -95 , changex = -10;
                ui->L_to_main->hide();
                x = ui->L_to_main->x();
                y = ui->L_to_main->y();
                ui->L_to_main->move(x+changex, y +changey) ;

                ui->Show_pass->hide();
                x = ui->Show_pass->x();
                y = ui->Show_pass->y();
                ui->Show_pass->move(x+changex, y +changey) ;

                ui->label->hide();
                x = ui->label->x();
                y = ui->label->y();
                ui->label->move(x+changex, y +changey) ;

                ui->label_2->hide();
                x = ui->label_2->x();
                y = ui->label_2->y();
                ui->label_2->move(x+changex, y +changey) ;

                ui->label_4->hide();
                x = ui->label_4->x();
                y = ui->label_4->y();
                ui->label_4->move(x+changex, y +changey) ;

                ui->label_33->hide();
                x = ui->label_33->x();
                y = ui->label_33->y();
                ui->label_33->move(x+changex, y +changey) ;

                ui->lineEdit->hide();
                ui->lineEdit->clear();
                x = ui->lineEdit->x();
                y = ui->lineEdit->y();
                ui->lineEdit->move(x+changex, y +changey) ;

                ui->lineEdit_2->hide();
                ui->lineEdit_2->clear();
                x = ui->lineEdit_2->x();
                y = ui->lineEdit_2->y();
                ui->lineEdit_2->move(x+changex, y +changey) ;

                ui->lineEdit_3->hide();
                ui->lineEdit_3->clear();
                x = ui->lineEdit_3->x();
                y = ui->lineEdit_3->y();

                ui->lineEdit_3->move(x+changex, y +changey) ;
                ui->label->hide();
                ui->label_2->hide();
                ui->lineEdit->hide();
                ui->lineEdit->clear();
                ui->lineEdit_2->hide();
                ui->lineEdit_2->clear();
                ui->lineEdit_3->hide();
                ui->lineEdit_3->clear();
                ui->L_to_main->hide();
                ui->Show_pass->hide();
                ui->Login->show();
                ui->Register->show();
            }else {
                decipher(Right_pass) ;
                if (found && S(pass) == Right_pass){
                    counter = 0 ;
                    int x , y , changey = -95 , changex = -10;
                    ui->L_to_main->hide();
                    x = ui->L_to_main->x();
                    y = ui->L_to_main->y();
                    ui->L_to_main->move(x+changex, y +changey) ;

                    ui->Show_pass->hide();
                    x = ui->Show_pass->x();
                    y = ui->Show_pass->y();
                    ui->Show_pass->move(x+changex, y +changey) ;

                    ui->label->hide();
                    x = ui->label->x();
                    y = ui->label->y();
                    ui->label->move(x+changex, y +changey) ;

                    ui->label_2->hide();
                    x = ui->label_2->x();
                    y = ui->label_2->y();
                    ui->label_2->move(x+changex, y +changey) ;

                    ui->label_4->hide();
                    x = ui->label_4->x();
                    y = ui->label_4->y();
                    ui->label_4->move(x+changex, y +changey) ;

                    ui->label_33->hide();
                    x = ui->label_33->x();
                    y = ui->label_33->y();
                    ui->label_33->move(x+changex, y +changey) ;

                    ui->lineEdit->hide();
                    ui->lineEdit->clear();
                    x = ui->lineEdit->x();
                    y = ui->lineEdit->y();
                    ui->lineEdit->move(x+changex, y +changey) ;

                    ui->lineEdit_2->hide();
                    ui->lineEdit_2->clear();
                    x = ui->lineEdit_2->x();
                    y = ui->lineEdit_2->y();
                    ui->lineEdit_2->move(x+changex, y +changey) ;

                    ui->lineEdit_3->hide();
                    ui->lineEdit_3->clear();
                    x = ui->lineEdit_3->x();
                    y = ui->lineEdit_3->y();
                    ui->lineEdit_3->move(x+changex, y +changey) ;

                    ui->ChangePass->show() ;
                    ui->Logout->show();
                    ui->label_6->show();
                    ui->label_6->setText(QMobile);
                    ui->label_6->setStyleSheet(font_style);
                    ui->label_7->show();
                    ui->label_7->setText(QE_mail);
                    ui->label_7->setStyleSheet(font_style);
                    ui->label_8->show();
                    ui->label_8->setStyleSheet(font_style);
                    ui->label_12->setText(QHello) ;
                    ui->label_12->show();
                    ui->label_12->setStyleSheet(font_style);
                    ui->textEdit->show();
                    ui->textEdit->setText(info) ;

                }else {
                    string message = "Dosen't Exist email you still have " + to_string(3 - counter) + " Tries and you will be returned to main menu" ;
                    QString Qmessage = Q(message) ;
                    QMessageBox::warning(this , "Login" , Qmessage) ;
                }
            }
        }
        if (!ui->ChangePass_2->isHidden()){
            bool valid = false ;
            string old_pass_check = ui->label_13->S(text())
                 , re_pass_check = ui->label_14->S(text()) , user_name , pass;
            if (ui->Show_pass_2->isChecked()){
                pass = ui->lineEdit_7->S(text());
            }else {
                pass = ui->lineEdit_4->S(text());
            }
            int new_pass_check = password_strength(pass) ;
            if (old_pass_check == "Corrret old password" && new_pass_check >= 3 && re_pass_check == "matched"
                    && S(ui->label_35->text()) == "Valid"){
                valid = true ;
                user_name = ui->label_12->S(text());
                user_name.erase(0 , 6) ;
                int x , y , changey = -50 , changex = -5;

                ui->label_9->hide();
                x = ui->label_9->x();
                y = ui->label_9->y();
                ui->label_9->move(x + changex, y + changey);

                ui->label_10->hide();
                x = ui->label_10->x();
                y = ui->label_10->y();
                ui->label_10->move(x + changex, y + changey);

                ui->label_11->hide();
                x = ui->label_11->x();
                y = ui->label_11->y();
                ui->label_11->move(x + changex, y + changey);

                ui->lineEdit_2->hide();
                ui->lineEdit_2->clear();
                x = ui->lineEdit_2->x();
                y = ui->lineEdit_2->y();
                ui->lineEdit_2->move(x + changex, y + changey);

                ui->lineEdit_3->hide();
                ui->lineEdit_3->clear();
                x = ui->lineEdit_3->x();
                y = ui->lineEdit_3->y();
                ui->lineEdit_3->move(x + changex, y + changey);

                ui->lineEdit_4->hide();
                ui->lineEdit_4->clear();
                x = ui->lineEdit_4->x();
                y = ui->lineEdit_4->y();
                ui->lineEdit_4->move(x + changex, y + changey);

                ui->lineEdit_5->hide();
                ui->lineEdit_5->clear();
                x = ui->lineEdit_5->x();
                y = ui->lineEdit_5->y();
                ui->lineEdit_5->move(x + changex, y + changey);

                ui->lineEdit_7->hide();
                ui->lineEdit_7->clear();
                x = ui->lineEdit_7->x();
                y = ui->lineEdit_7->y();
                ui->lineEdit_7->move(x + changex, y + changey);

                ui->lineEdit_8->hide();
                ui->lineEdit_8->clear();
                x = ui->lineEdit_8->x();
                y = ui->lineEdit_8->y();
                ui->lineEdit_8->move(x + changex, y + changey);


                ui->Show_pass->hide();
                x = ui->Show_pass->x();
                y = ui->Show_pass->y();
                ui->Show_pass->move(x + changex, y + changey);

                ui->Show_pass_2->hide();
                x = ui->Show_pass_2->x();
                y = ui->Show_pass_2->y();
                ui->Show_pass_2->move(x + changex, y + changey);

                ui->Show_pass_3->hide();
                x = ui->Show_pass_3->x();
                y = ui->Show_pass_3->y();
                ui->Show_pass_3->move(x + changex, y + changey);

                ui->ChangePass_2->hide();
                x = ui->ChangePass_2->x();
                y = ui->ChangePass_2->y();
                ui->ChangePass_2->move(x + changex, y + changey);

                ui->label_13->hide();
                ui->label_13->clear();
                x = ui->label_13->x();
                y = ui->label_13->y();
                ui->label_13->move(x + changex, y + changey);

                ui->label_14->hide();
                ui->label_14->clear();
                x = ui->label_14->x();
                y = ui->label_14->y();
                ui->label_14->move(x + changex, y + changey);

                ui->label_33->hide();
                x = ui->label_33->x();
                y = ui->label_33->y();
                ui->label_33->move(x + changex, y + changey);

                ui->label_34->hide();
                x = ui->label_34->x();
                y = ui->label_34->y();
                ui->label_34->move(x + changex, y + changey);

                ui->label_3->hide();
                x = ui->label_3->x();
                y = ui->label_3->y();
                ui->label_3->move(x + changex, y + changey);

                ui->ChangePass->show() ;
                ui->Logout->show();
                ui->label_6->show();
                ui->label_7->show();
                ui->label_8->show();
                ui->textEdit->show();

                ui->label_35->hide();
                ui->label_35->setText("");
                x = ui->label_35->x();
                y = ui->label_35->y();
                ui->label_35->move(x + changex, y + changey);

                QFile file ("Main info.txt") ;

                file.open(QIODevice::ReadWrite);
                QString Qin = file.readAll() ;
                string in = S(Qin) ;
                string file_out = change_pass(in , user_name , pass) ;
                QString Qfile_out = Q(file_out);
                file.resize(0);
                QTextStream out (&file) ;
                out << Qfile_out ;
                file.close();
            }else {
                if (old_pass_check != "Corrret old password"){
                    QMessageBox::warning(this , "Change password" , "Old pass isn't exist");
                }else if (new_pass_check < 3) {
                    QMessageBox::warning(this , "Change password" , "New password is very week");
                }else {
                    if (S(ui->label_35->text()) != "Already exist")
                        QMessageBox::warning(this , "Change password" , "the new password is already exist");
                    else
                        QMessageBox::warning(this , "Change password" , "both passwords are't the same");
                }
            }
        }
    }
}
