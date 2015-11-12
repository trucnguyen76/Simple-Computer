#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpMemory();
    setUpIO();
    setWindowTitle("Simple Computer");
    ui->saveLoadWidget->hide();


    foreach(QLineEdit* memoryCell, memoryMap)
    {
        connect(memoryCell, SIGNAL(editingFinished()), this, SLOT(editMemory()));
    }

    foreach(QLineEdit* inputCell, inputMap)
    {
        connect(inputCell, SIGNAL(editingFinished()), this, SLOT(editInput()));
    }

    connect(ui->actionSave_File, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionLoad_File, SIGNAL(triggered()), this, SLOT(loadFile()));


    currentInputKey = 0;
    currentOutputKey = 0;
    accumulator = 0;
    pCounter = 0;
    iRegister = 0;
    okClicked = false;
    cancelClicked = false;

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

    for(key = 0; key < 10; key++ )
    {
        inputMap.insert(key,((QLineEdit*)(ui->InputGroup->childAt(x,y))));
//        inputMap[key]->setText("01");
        inputMap[key]->setValidator(new QIntValidator(-999, 999, this));
        y += 20;
    }

    y = 25;
    for(key = 0; key < 10; key++ )
    {
        outputMap.insert(key,((QLineEdit*)(ui->OutputGroup->childAt(x,y))));
//        outputMap[key]->setText("01");
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

bool MainWindow::step(int command)
{
    int code;
    int memory;
    bool cont;
    QString str;

    code = command / 100;
    memory = command % 100;

    //Update Intstruction Register
    iRegister = memoryMap[pCounter]->displayText().toInt();

    switch(code)
    {
        case INP:
            if(inputMap[currentInputKey]->text().size() > 0)
            {
                memoryMap[memory]->setText(inputMap[currentInputKey]->displayText());
                cont = true;
            }
            else
            {
                QMessageBox::information(this, "Error", "Input Card Empty");
                cont = false;
            }
            currentInputKey++;
            pCounter++;

            break;
        case OUT:
            if(currentOutputKey <= 9)
            {
                outputMap[currentOutputKey]->setText(memoryMap[memory]->displayText());
                currentOutputKey++;
                pCounter++;
                cont = true;
            }
            else
            {
                cont = false;
            }

            break;
        case ADD:
            accumulator += memoryMap[memory]->displayText().toInt();
            updateACDisplay();
            pCounter++;
            cont = true;

            break;
        case SUB:
            accumulator -= memoryMap[memory]->displayText().toInt();
            updateACDisplay();
            pCounter++;
            cont = true;

            break;
        case LDA:
            accumulator = memoryMap[memory]->displayText().toInt();
            pCounter++;
            updateACDisplay();
            cont = true;

            break;
        case STA:
            if(accumulator >= 0)
            {
                str = QString::number(accumulator % 1000);
                if(str.size() == 1)
                {
                    str = "00" + str;
                }
                else if(str.size() == 2)
                {
                    str = "0" + str;
                }
            }
            else
            {
                str = QString::number((0-accumulator) % 1000);
                if(str.size() == 1)
                {
                    str = "-00" + str;
                }
                else if(str.size() == 2)
                {
                    str = "-0" + str;
                }
                else
                {
                    str = "-" + str;
                }
            }


            memoryMap[memory]->setText(str);
            pCounter++;

            cont = true;
            break;
        case JMP:
            str = QString::number(pCounter + 1 );
            if(str.size() == 1)
            {
                str = "00" + str;
            }
            else if(str.size() == 2)
            {
                str = "0" + str;
            }
            memoryMap[99]->setText(str);
            pCounter = memory;

            cont = true;
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
            cont = true;

            break;
        case SHF:
            int x, y;
            x = memory / 10;
            y = memory % 10;

            for(int i = 0; i < x; i++)
            {
                accumulator *= 10;
            }
            if(accumulator > 9999)
            {
                accumulator = accumulator % 10000;
            }
            for(int i = 0; i < y; i++)
            {
                accumulator /= 10;
            }
            pCounter++;

            updateACDisplay();
            cont = true;
            break;
        case HLT:
            QMessageBox::information(this, "Info", "Program terminate normally");
            cont = false;
            pCounter = 0;
            break;
        default:
//            qDebug() << "Not a valid command";
            cont = false;
            break;

    }

    updateIRDisplay();
    updatePCDisplay();

    return cont;
}

void MainWindow::updateACDisplay()
{
    if(accumulator >= 0)
    {
        ui->AC1->setText(QString::number (accumulator / 1000));
        ui->AC2->setText(QString::number((accumulator / 100)%10));
        ui->AC3->setText(QString::number((accumulator / 10)%10));
        ui->AC4->setText(QString::number (accumulator % 10));
    }
    else
    {
        ui->AC1->setText("-");
        ui->AC2->setText(QString::number((accumulator / -100)%10));
        ui->AC3->setText(QString::number((accumulator / -10)%10));
        ui->AC4->setText(QString::number ((0-accumulator) % -10));
    }

}

void MainWindow::updatePCDisplay()
{
    ui->PC1->setText(QString::number (pCounter / 10));
//    ui->PC2->setText(QString::number((pCounter / 10)%10));
    ui->PC2->setText(QString::number (pCounter % 10));
}

void MainWindow::updateIRDisplay()
{
    ui->IR1->setText(QString::number (iRegister/ 100));
    ui->IR2->setText(QString::number((iRegister / 10)%10));
    ui->IR3->setText(QString::number (iRegister % 10));
}


void MainWindow::on_runBtn_clicked()
{
    while(on_stepBtn_clicked())
    {}
}

bool MainWindow::on_stepBtn_clicked()
{
    bool cont;
    //  ui->PC1->displayText().toInt() * 100 +
    pCounter =  ui->PC1->displayText().toInt() * 10 +
                ui->PC2->displayText().toInt();
//qDebug() << "pCounter" << pCounter;
//qDebug() << "Start Step";
    iRegister = memoryMap[pCounter]->displayText().toInt();
    if(iRegister == 0)
    {
        QMessageBox::information(this, "Error", "Reach empty memory card");
        cont = false;
    }
    else
    {
        cont = step(iRegister);
    }

    return cont;
}

void MainWindow::on_clearCPUBut_clicked()
{
    ui->AC1->clear();
    ui->AC2->clear();
    ui->AC3->clear();
    ui->AC4->clear();

    ui->PC1->clear();
    ui->PC2->clear();

    ui->IR1->clear();
    ui->IR2->clear();
    ui->IR3->clear();

}

void MainWindow::on_clearInBut_clicked()
{
    foreach(QLineEdit* inputCell, inputMap)
    {
        inputCell->clear();
    }
    currentInputKey = 0;
}

void MainWindow::on_clearOutBut_clicked()
{
    foreach(QLineEdit* outputCell, outputMap)
    {
        outputCell->clear();
    }
    currentOutputKey = 0;
}

void MainWindow::on_clearMemBut_clicked()
{
    foreach(QLineEdit* memoryCell, memoryMap)
    {
        memoryCell->clear();
    }

    memoryMap[0]->setText("001");
}


void MainWindow::on_clearAllBut_clicked()
{
    on_clearInBut_clicked();
    on_clearOutBut_clicked();
    on_clearMemBut_clicked();
    on_clearCPUBut_clicked();
}

void MainWindow::on_resetInBtn_clicked()
{
    currentInputKey = 0;
}

void MainWindow::saveFile()
{
    ui->saveLoadWidget->show();

    if(okClicked && ui->fileNameInput->displayText().size() > 0)
    {
        QString fileName = ui->fileNameInput->displayText() + ".txt";
        QFile file(fileName);
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream out(&file);

        //Write memory cells
        foreach(QLineEdit* memoryCell, memoryMap)
        {
            out << memoryCell->displayText();
            out << endl;
        }

        //Write Input cells
        foreach(QLineEdit* inputCell, inputMap)
        {
            out << inputCell->displayText();
            out << endl;
        }

        //Write Output cells
        foreach(QLineEdit* outputCell, outputMap)
        {
            out << outputCell->displayText();
            out << endl;
        }

        out << accumulator << endl;
        out << pCounter << endl;
        out << iRegister << endl;

        out.flush();
        file.close();
        ui->saveLoadWidget->hide();
    }
    else if(okClicked)
    {
        QMessageBox::information(this,"Error", "Please enter a valid file name");
    }
    else if(cancelClicked)
    {
        ui->saveLoadWidget->hide();
    }
    else
    {
        qDebug() << "Shit";
    }
}

void MainWindow::loadFile()
{
    if(okClicked && ui->fileNameInput->displayText().size() > 0)
    {
        QString fileName = ui->fileNameInput->displayText() + ".txt";
        QFile file(fileName);
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream in(&file);

        //Write memory cells
        foreach(QLineEdit* memoryCell, memoryMap)
        {
            memoryCell->setText(in.readLine());
            in << endl;
        }

        //Write Input cells
        foreach(QLineEdit* inputCell, inputMap)
        {
            inputCell->setText(in.readLine());
            in << endl;
        }

        //Write Output cells
        foreach(QLineEdit* outputCell, outputMap)
        {
            outputCell->setText(in.readLine());
            in << endl;
        }

        accumulator = in.readLine().toInt();
        pCounter = in.readLine().toInt();
        iRegister = in.readLine().toInt();

        updateIRDisplay();
        updatePCDisplay();
        updateACDisplay();

        file.close();
        ui->saveLoadWidget->hide();
    }
    else if(okClicked)
    {
        QMessageBox::information(this,"Error", "Please enter a valid file name");
    }
    else if(cancelClicked)
    {
        ui->saveLoadWidget->hide();
    }
    else
    {
        qDebug() << "Shit";
    }

}

void MainWindow::on_okButton_clicked()
{
    okClicked = true;
}

void MainWindow::on_cancelButton_clicked()
{
    cancelClicked = true;
}
