#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ChangePass_clicked();

    void on_ChangePass_2_clicked();

    void on_Show_pass_toggled(bool checked);

    void on_Show_pass_2_toggled(bool checked);

    void on_Show_pass_3_toggled(bool checked);

    void on_L_to_main_clicked();

    void on_Login_clicked();

    void on_Logout_clicked();

    void on_R_to_main_clicked();

    void on_Register_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_lineEdit_6_textChanged(const QString &arg1);

    void on_lineEdit_7_textChanged(const QString &arg1);

    void on_lineEdit_8_textChanged(const QString &arg1);

    void on_lineEdit_9_textChanged(const QString &arg1);

    void on_lineEdit_10_textChanged(const QString &arg1);

    void on_lineEdit_11_textChanged(const QString &arg1);

    void on_show_pass_4_toggled(bool checked);

    void keyPressEvent(QKeyEvent * ev);

    void mousePressEvent(QMouseEvent * ev) ;

    void on_Back_button_clicked();

    void on_Exit_clicked();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
