#ifndef LIGHTANDCHILL_H
#define LIGHTANDCHILL_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QProgressBar>
#include <QRadioButton>

#include "bass.h"
#include "basswasapi.h"

#define BANDS_DEFAULT 16
#define SPECHEIGHT 255
#define TICK 25

namespace Ui {
class LightAndChill;
}

class QHttpServer;
class QHttpRequest;
class QHttpResponse;
class QSerialPort;

class LightAndChill : public QWidget
{
    Q_OBJECT

public:
    explicit LightAndChill(QWidget *parent = 0);
    ~LightAndChill();

private:
    Ui::LightAndChill *ui;
    bool isEnabled;
    BASS_WASAPI_INFO info;
    QTimer* timer;
    float* fft;
    QVector<QProgressBar*> barList;
    QVector<QRadioButton*> radioList;
    int selected;
    int BANDS;
    QHttpServer* server;
    QSerialPort* serial;
    bool isComOpen;

    void fillDevicesList();
    void initBass();
    void error(QString msg);
    void addBar(int index);
    void removeBar(int index);
    void send(QString cmd);

private slots:
    void onClickEnable();
    void onTick();
    void onSelectedChange();
    void onBandsCountChange(int);
    void onLimitChange();
    void onRequest(QHttpRequest* req, QHttpResponse* resp);
    void onComChange(QString);
};

#endif // LIGHTANDCHILL_H
