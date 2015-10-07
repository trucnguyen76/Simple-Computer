#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QLineEdit>
#include <QDebug>

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
    void decision(int command);

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

};




#endif // MAINWINDOW_H
