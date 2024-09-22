#ifndef KUVATESTI_H
#define KUVATESTI_H

#include <QMainWindow>
#include <QSvgRenderer>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class kuvatesti;
}
QT_END_NAMESPACE

class kuvatesti : public QMainWindow
{
    Q_OBJECT

public:
    kuvatesti(QWidget *parent = nullptr);
    ~kuvatesti();

private slots:
    // Slot to update the QR code 60 times per second
    void updateQRCode();

private:
    // Function to save QR code data to a JSON file
    void saveQRCodeDataToJson(const QString &qrData, const QString &timestamp, int frameNumber);

private:
    Ui::kuvatesti *ui;

    QLabel *svgLabel;   // QLabel to display the QR code
    QTimer *timer;      // Timer to trigger updates 60 times per second
    int frameCounter;   // Declare the frame counter here
    double interval;
};

#endif // KUVATESTI_H
