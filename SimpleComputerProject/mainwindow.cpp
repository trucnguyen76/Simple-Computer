#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpMemory();
    setUpIO();

    foreach(QLineEdit* memoryCell, memoryMap)
    {
        connect(memoryCell, SIGNAL(editingFinished()), this, SLOT(editMemory()));
    }

    foreach(QLineEdit* inputCell, inputMap)
    {
        connect(inputCell, SIGNAL(editingFinished()), this, SLOT(editInput()));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpMemory()
{
    //YSTART has to go down a lil bit so it wouldnt be at edge
    const int YSTART = 32;
    const int YEND   = YSTART + 20*20;
    int x = 50;
    int y;
    int key = 0;

    y = YSTART;

    for(x = 50; x <= 490; x += 110)
    {
        for(y = YSTART; y < YEND; y+= 20 )
        {
            memoryMap.insert(key,((QLineEdit*)(ui->memoryGroup->childAt(x,y))));
            memoryMap[key]->setValidator(new QIntValidator(000, 999, this));
            key++;
        }
    }
}

void MainWindow::setUpIO()
{
    //YSTART has to go down a lil bit so it wouldnt be at edge
    int x = 35;
    int y = 25;
    int key;

    for(key = 1; key <= 10; key++ )
    {
        inputMap.insert(key,((QLineEdit*)(ui->InputGroup->childAt(x,y))));
        inputMap[key]->setText("01");
        inputMap[key]->setValidator(new QIntValidator(-999, 999, this));
        y += 20;
    }

    y = 25;
    for(key = 1; key <= 10; key++ )
    {
        outputMap.insert(key,((QLineEdit*)(ui->OutputGroup->childAt(x,y))));
        outputMap[key]->setText("01");
        y += 20;

    }
}

void MainWindow::editMemory()
{
    QLineEdit* lineEdit = (QLineEdit*) sender();
    QString text = lineEdit->displayText();
    int number = text.toInt();

    //Prevent keep adding 0
    if(!text.startsWith('0'))
    {
        if(number < 10)
        {
            lineEdit->setText("00" + text);
        }
        else if(number < 100)
        {
            lineEdit->setText("0" + text);
        }
    }

}

void MainWindow::editInput()
{
    QLineEdit* lineEdit = (QLineEdit*) sender();
    QString text = lineEdit->displayText();
    int number = text.toInt();

    if(number < 0)
    {
        //Prevent keep adding 0
        if(text.at(1) != '0')
        {
            if(number > -10)
            {
                lineEdit->setText("-00" + text);
            }
            else if(number > -100)
            {
                lineEdit->setText("-0" + text);
            }
        }
    }
    else
    {
        //Prevent keep adding 0
        if(!text.startsWith('0'))
        {
            if(number < 10)
            {
                lineEdit->setText("00" + text);
            }
            else if(number < 100)
            {
                lineEdit->setText("0" + text);
            }
        }
    }
}

void MainWindow::decision(int command)
{
    int code;
    int memory;

    code = command / 100;
    memory = command % 100;

    switch(code)
    {
        case INP:
            memoryMap[memory]->setText(inputMap[currentInputKey]->displayText());
            currentInputKey++;
            pCounter++;
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case OUT:
            outputMap[currentOutputKey]->setText(memoryMap[memory]->displayText());
            currentOutputKey++;
            pCounter++;
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case ADD:
            accumulator += memoryMap[memory]->displayText().toInt();
            //Change display of AC on GUI
            updateACDisplay();
            pCounter++;
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case SUB:
            accumulator -= memoryMap[memory]->displayText().toInt();
            //Change display of AC on GUI
            updateACDisplay();
            pCounter++;
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case LDA:
            accumulator = memoryMap[memory]->displayText().toInt();
            pCounter++;
            //Change display of AC on GUI
            updateACDisplay();
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case STA:
            memoryMap[memory]->setText(QString::number(accumulator % 1000));
            pCounter++;
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case JMP:
            memoryMap[99]->setText(QString::number(pCounter));
            pCounter = memory;
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case TAC:
            if(accumulator < 0)
            {
                pCounter = memory;
            }
            else
            {
                pCounter++;
            }
            //Change display of PC on GUI
            updatePCDisplay();
            break;
        case SHF:
            int x, y;
            x = memory / 10;
            y = memory % 10;

            for(int i = 0; i < x; i++)
            {
                accumulator *= 10;
            }
            for(int i = 0; i < y; i++)
            {
                accumulator /= 10;
            }
            pCounter++;
            //Change display of PC on GUI
            updatePCDisplay();
            //Change display of AC on GUI
            updateACDisplay();
            break;
        case HLT:
            qDebug() << "HALT";
            break;
        default:
            qDebug() << "Not a valid command";

    }
    //Update Intstruction Register
    updateIRDisplay();
}

void MainWindow::updateACDisplay()
{
    ui->AC1->setText(QString::number (accumulator / 1000));
    ui->AC2->setText(QString::number((accumulator / 100)%10));
    ui->AC3->setText(QString::number((accumulator / 10)%10));
    ui->AC4->setText(QString::number (accumulator % 10));
}

void MainWindow::updatePCDisplay()
{
    ui->PC1->setText(QString::number (pCounter / 10));
    ui->PC2->setText(QString::number (pCounter % 10));
}

void MainWindow::updateIRDisplay()
{
    int IR = memoryMap[pCounter - 1]->displayText().toInt();

    ui->IR1->setText(QString::number (IR / 100));
    ui->IR2->setText(QString::number((IR / 10)%10));
    ui->IR3->setText(QString::number (IR % 10));
}




