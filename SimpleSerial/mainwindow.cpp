#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    baudrate(QSerialPort::Baud115200),
    serialDevice(new QSerialPort(this))
{
   init();
}

MainWindow::~MainWindow()
{
    on_disconnect_button_clicked();
    delete ui;

}

void MainWindow::init()
{

    linecntr=0;

    ui->setupUi(this);

    ui->disconnect_button->setEnabled(false);

    //window attributes - no resize, minimize btn, close btn
    setWindowFlags(Qt::Window |
                   Qt::MSWindowsFixedSizeDialogHint |
                   Qt::WindowMinimizeButtonHint |
                   Qt::WindowCloseButtonHint);

    connect(serialDevice,&QSerialPort::readyRead,this, &MainWindow::onSerialDataAvailable);

    getAvalilableSerialDevices();

}
void MainWindow::getAvalilableSerialDevices()
{
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    serialComPortList.clear();
    ui->cbserialPort->clear();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
       QString dbgStr = "Vendor ID: ";

	   if(serialPortInfo.hasVendorIdentifier())
	   {
		  dbgStr+= serialPortInfo.vendorIdentifier();
	   }
	   else
	   {
		  dbgStr+= " - ";
	   }
	   dbgStr+= "  Product ID: ";
	   if(serialPortInfo.hasProductIdentifier())
	   {
		  dbgStr+= serialPortInfo.hasProductIdentifier();
	   }
	   else
	   {
		  dbgStr+= " - ";
	   }
	   dbgStr+= " Name: " + serialPortInfo.portName();
	   dbgStr+= " Description: "+serialPortInfo.description();
       qDebug()<<dbgStr;
       serialComPortList.push_back(serialPortInfo);
       ui->cbserialPort->addItem(serialPortInfo.portName() +" "+serialPortInfo.description());
    }
}

void MainWindow::serialWrite(QString message)
{
    if(serialDevice->isOpen())
    {
        serialDevice->write(message.toUtf8()); // Send the message to the device
        qDebug() << "Message to device: "<<message;
    }
}

void MainWindow::serialRead()
{
    if(serialDevice->isOpen())
    {
        serialBuffer += serialDevice->readAll(); // Read the available data
    }
}

void MainWindow::onSerialDataAvailable()
{
    if(serialDevice->isOpen())
    {

//        if (linecntr >= linestoview)
//        {
//            linecntr = 0;
//            ui->ioview->setText("");
//        }

        serialRead(); // Read data from comm port

        QString text = ui->ioview->document()->toPlainText() + serialBuffer.toUtf8();
        ui->ioview->setText(text);
        ui->ioview->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        serialBuffer = "";

        linecntr++;
    }
}

QSerialPort::DataBits MainWindow::translateToDataBits(int value)
{
    QSerialPort::DataBits retval;

    switch (value)
    {
    case 5:
        retval=QSerialPort::Data5;
        break;
    case 6:
        retval=QSerialPort::Data6;
        break;
    case 7:
        retval=QSerialPort::Data7;
        break;
    case 8:
        retval=QSerialPort::Data8;
        break;
    default:
        retval=QSerialPort::UnknownDataBits;
        break;
    }
    return retval;
}

QSerialPort::Parity MainWindow::translateToParity(QString value)
{
    QSerialPort::Parity retval;

    if (value == "None")
    {       
        retval = QSerialPort::NoParity;
    }
    else if (value == "Even")
    {
        retval = QSerialPort::EvenParity;
    }
    else if (value == "Odd")
    {
        retval = QSerialPort::OddParity;
    }
    else if (value == "Space")
    {
        retval = QSerialPort::SpaceParity;
    }
    else if (value == "Mark")
    {
        retval = QSerialPort::MarkParity;
    }
    else
    {
        retval = QSerialPort::UnknownParity;
    }

   return retval;
}

QSerialPort::StopBits MainWindow::translateToStopBits(int value)
{
    QSerialPort::StopBits retval;

    switch (value)
    {
       case 1:
          retval=QSerialPort::OneStop;
          break;
       case 2:
          retval=QSerialPort::OneStop;
          break;
       case 3:
          retval=QSerialPort::OneAndHalfStop;
          break;
       default:
          retval=QSerialPort::UnknownStopBits;
          break;
    }

    return retval;
}

QSerialPort::FlowControl MainWindow::translateToFlowControl(QString value)
{
    QSerialPort::FlowControl retval;

    if (value=="None" )
    {
       retval=QSerialPort::NoFlowControl;
    }
    else if (value=="Hardware")
    {
        retval=QSerialPort::HardwareControl;
    }
    else if (value=="Software")
    {
        retval=QSerialPort::SoftwareControl;
    }
    else
    {
        retval=QSerialPort::UnknownFlowControl;
    }

    return retval;
}

void MainWindow::on_connect_button_clicked()
{
    if(!serialDevice->isOpen())
    {

        ui->ioview->setText("");

        serialDevice->setPortName(serialComPortList[ui->cbserialPort->currentIndex()].portName());
        deviceDescription = serialComPortList[ui->cbserialPort->currentIndex()].description();
        qDebug() << "connecting to: "<<serialDevice->portName();

        if(serialDevice->open(QIODevice::ReadWrite))
        {

            ui->connect_button->setEnabled(false);
            ui->disconnect_button->setEnabled(true);
            bool convok=false;

            qDebug() << "baud rate: " << ui->baudrate->currentText();  //baudrate
            if(!serialDevice->setBaudRate(ui->baudrate->currentText().toInt(&convok)) || !convok)
                qDebug()<<serialDevice->errorString();

            qDebug() << "databits: " << ui->databits->currentText();   //QSerialPort::Data8

            if(!serialDevice->setDataBits(translateToDataBits(ui->databits->currentText().toInt(&convok))) || !convok)
                qDebug()<<serialDevice->errorString();

            qDebug() << "parity: " << ui->parity->currentText();       //QSerialPort::NoParity
            if(!serialDevice->setParity(translateToParity(ui->parity->currentText())))
                qDebug()<<serialDevice->errorString();

            qDebug() << "stopbits: " << ui->stopbits->currentText();   //QSerialPort::OneStop
            if(!serialDevice->setStopBits(translateToStopBits(ui->stopbits->currentText().toInt(&convok))) || !convok)
                qDebug()<<serialDevice->errorString();

            qDebug() << "flowcontrol: " << ui->flowcntrl->currentText(); //QSerialPort::NoFlowControl
            if(!serialDevice->setFlowControl(translateToFlowControl(ui->flowcntrl->currentText())))
                qDebug()<<serialDevice->errorString();

            qDebug() << "Connection to: "<< serialDevice->portName() << " " << deviceDescription << " connected";

        }
        else
        {
            qDebug() << "Connection to: "<< serialDevice->portName() << " " << deviceDescription << " not connected";
            qDebug() <<"         Error: "<<serialDevice->errorString();

        }
    }
    else
    {
        qDebug() << "Can't connect, another device is connected";
    }
}

void MainWindow::on_disconnect_button_clicked()
{
    if(serialDevice->isOpen())
    {

        ui->connect_button->setEnabled(true);
        ui->disconnect_button->setEnabled(false);

        serialDevice->close();
        qDebug() << "Connection to: "<< serialDevice->portName() << " " << deviceDescription << " closed";
    }
    else
    {
       qDebug() << "Can't disconnect, no device is connected";
    }
}
