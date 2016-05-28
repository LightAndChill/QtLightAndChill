#include "lightandchill.h"
#include "ui_lightandchill.h"

#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QtSerialPort>

LightAndChill::LightAndChill(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightAndChill),
    isEnabled(false),
    selected(0),
    BANDS(BANDS_DEFAULT)
{
    ui->setupUi(this);

    setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    fft = new float[1024];
    timer = new QTimer(this);

    ui->bandsSpin->setValue(BANDS);

    connect(timer, SIGNAL(timeout()), this, SLOT(onTick()));
    connect(ui->enableBtn, SIGNAL(clicked()), this, SLOT(onClickEnable()));
    connect(ui->bandsSpin, SIGNAL(valueChanged(int)), this, SLOT(onBandsCountChange(int)));
    connect(ui->limitSlider, SIGNAL(sliderReleased()), this, SLOT(onLimitChange()));

    for (int i = 0; i < BANDS; i++)
    {
        addBar(i);
    }

    fillDevicesList();
    initBass();

    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    foreach(QSerialPortInfo port, portList)
    {
        qDebug() << port.portName();
    }

    /*QSerialPort serial;
    serial.setPortName("COM1");
    serial.write();*/
}

LightAndChill::~LightAndChill()
{
    BASS_WASAPI_Free();
    BASS_Free();
    delete ui;
}

void LightAndChill::error(QString msg)
{
    QMessageBox::warning(this, "Error", msg);
    QCoreApplication::quit();
}

DWORD CALLBACK process(void* /*buffer*/, DWORD /*length*/, void* /*user*/)
{
    return true;
}

void LightAndChill::onClickEnable()
{
    isEnabled = !isEnabled;
    ui->enableBtn->setText((isEnabled ? "Disable" : "Enable"));

    if (isEnabled)
    {
        int deviceIndex = ui->devicesList->currentData().toInt();

        if (!BASS_WASAPI_Init(deviceIndex, 0, 0, BASS_WASAPI_BUFFER, 1, 0.1, &process, NULL))
        {
            error("Can't initialize WASAPI device");
            return;
        }

        BASS_WASAPI_GetInfo(&info);
        BASS_WASAPI_Start();

        timer->start(TICK);
    }
    else
    {
        timer->stop();
        BASS_WASAPI_Stop(true);
        BASS_WASAPI_Free();

        foreach (QProgressBar* bar, barList)
        {
            bar->setValue(0);
        }
    }

    ui->devicesList->setEnabled(!isEnabled);
}

void LightAndChill::fillDevicesList()
{
    BASS_WASAPI_DEVICEINFO device;

    for (int i = 0; BASS_WASAPI_GetDeviceInfo(i, &device); i++)
    {
        if (device.flags & BASS_DEVICE_ENABLED && (device.flags & BASS_DEVICE_LOOPBACK || device.type == BASS_WASAPI_TYPE_MICROPHONE))
        {
            ui->devicesList->addItem(QString("%1 - %2").arg(i).arg(device.name), i);
        }
    }
}

void LightAndChill::initBass()
{
    HWND h = (HWND)this->winId();

    if (!BASS_Init(0, 44100, BASS_DEVICE_DEFAULT, h, NULL))
    {
        error("Can't initialize BASS");
        return;
    }

    BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, false);
}

void LightAndChill::onTick()
{
    int result = BASS_WASAPI_GetData(fft, BASS_DATA_FFT2048);
    if (result < -1) return;

    int x, y;
    int b0 = 0;

    for (x = 0; x < BANDS; x++)
    {
        float peak = 0;
        int b1 = pow(2, x*10.0 / (BANDS-1));
        if (b1 > 1023) b1=1023;
        if (b1 <= b0) b1=b0+1;
        for (; b0 < b1; b0++)
        {
            if (peak < fft[1+b0]) peak = fft[1+b0];
        }
        y = sqrt(peak) * 3 * SPECHEIGHT - 4;
        if (y > SPECHEIGHT) y = SPECHEIGHT;
        if (y < 0) y = 0;

        barList.at(x)->setValue(y);

        if (x == selected)
        {
            //qDebug() << "Serial:" << y;
        }
    }

    timer->start(TICK);
}

void LightAndChill::onSelectedChange()
{
    QRadioButton* radio = (QRadioButton*)sender();
    selected = radio->accessibleName().toInt();
}

void LightAndChill::addBar(int index)
{
    QProgressBar* bar = new QProgressBar(this);
    bar->setGeometry(250 + (index * 20), 10, 16, 130);
    bar->setMaximum(SPECHEIGHT);
    bar->setValue(0);
    bar->setTextVisible(false);
    bar->setOrientation(Qt::Vertical);
    bar->show();
    barList.append(bar);

    QRadioButton* radio = new QRadioButton(this);
    radio->setGeometry(251 + (index * 20), 145, 16, 16);
    radio->setAccessibleName(QString::number(index));
    radio->show();
    if (index == 0) radio->setChecked(true);
    connect(radio, SIGNAL(clicked()), this, SLOT(onSelectedChange()));
    radioList.append(radio);

    resize(width() + 20, height());
}

void LightAndChill::removeBar(int index)
{
    barList.at(index)->deleteLater();
    barList.removeAt(index);
    radioList.at(index)->deleteLater();
    radioList.removeAt(index);

    if (selected == index)
    {
        selected--;
        radioList.at(selected)->setChecked(true);
    }

    resize(width() - 20, height());
}

void LightAndChill::onBandsCountChange(int number)
{
    if (number < 1)
    {
        ui->bandsSpin->setValue(1);
        return;
    }

    if (number > BANDS)
    {
        for (int i = BANDS; i < number; i++)
        {
            addBar(i);
            BANDS++;
        }
    }
    else if (number < BANDS)
    {

        for (int i = BANDS; i > number; i--)
        {
            removeBar(i - 1);
            BANDS--;
        }
    }
}

void LightAndChill::onLimitChange()
{
    int limit = ui->limitSlider->value();
    qDebug() << limit;
}
