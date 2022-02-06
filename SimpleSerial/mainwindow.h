#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <vector>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();


    private slots:
        void onSerialDataAvailable();
        void on_connect_button_clicked();
        void on_disconnect_button_clicked();

    private:
        void init();
        void getAvalilableSerialDevices();
        void serialRead();
        void serialWrite(QString message);
        QSerialPort::DataBits translateToDataBits(int value);
        QSerialPort::Parity translateToParity(QString value);
        QSerialPort::StopBits translateToStopBits(int value);
        QSerialPort::FlowControl translateToFlowControl(QString value);

        Ui::MainWindow *ui;

        qint32 baudrate;
        QSerialPort *serialDevice;
        std::vector<QSerialPortInfo> serialComPortList;
        QString deviceDescription;
        QString serialBuffer;

        const qint16 linestoview=16;
        qint16 linecntr;

};

#endif // MAINWINDOW_H
