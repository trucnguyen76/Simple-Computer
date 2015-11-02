#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

enum {
    INP,    //INPUT
    OUT,    //OUTPUT
    ADD,    //ADD
    SUB,    //SUBSTRACT
    LDA,    //LOAD
    STA,    //STORE ACCUMULATOR
    JMP,    //JUMP
    TAC,    //TEST ACCUMULATOR
    SHF,    //SHIFT
    HLT     //HALF
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpMemory();
    void setUpIO();
    void updateACDisplay();
    void updatePCDisplay();
    void updateIRDisplay();
    bool step(int command);
    void run();

private:
    Ui::MainWindow *ui;
    QMap<int, QLineEdit*> memoryMap;
    QMap<int, QLineEdit*> inputMap;
    QMap<int, QLineEdit*> outputMap;
    int currentInputKey;
    int currentOutputKey;
    int accumulator;
    int iRegister;
    int pCounter;

private slots:
    void editMemory();
    void editInput();

    void on_runBtn_clicked();
    bool on_stepBtn_clicked();
    void on_clearCPUBut_clicked();
    void on_clearInBut_clicked();
    void on_clearOutBut_clicked();
    void on_clearMemBut_clicked();
    void on_clearAllBut_clicked();
    void on_resetInBtn_clicked();
};




#endif // MAINWINDOW_H
