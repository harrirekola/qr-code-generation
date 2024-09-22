#include "kuvatesti.h"
#include "ui_kuvatesti.h"
#include "qrcodegen.hpp"
#include <QPixmap>
#include <QStandardPaths>
#include <QDebug>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <QByteArray>
#include <QSvgRenderer>
#include <QPainter>
#include <QVBoxLayout>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

static QString toSvgString(const QrCode &qr, int border) {
    if (border < 0)
        throw std::domain_error("Border must be non-negative");
    if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
        throw std::overflow_error("Border too large");

    std::ostringstream sb;
    sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
    sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
    sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
    sb << "\t<path d=\"";
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                if (x != 0 || y != 0)
                    sb << " ";
                sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
            }
        }
    }
    sb << "\" fill=\"#000000\"/>\n";
    sb << "</svg>\n";
    QString qstr = QString::fromStdString(sb.str());
    return qstr;
}

kuvatesti::kuvatesti(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::kuvatesti)
    , interval(double(1000) / double(120))
    // 60 FPS = +0.5
{
    ui->setupUi(this);

    // Create a QLabel to display the QR code
    svgLabel = new QLabel(this);

    // Center the QR code within the QLabel
    svgLabel->setAlignment(Qt::AlignCenter);

    // Set up the layout to add the QLabel to the main window
    QVBoxLayout *layout = new QVBoxLayout;

    // Set the alignment for the layout to center the QLabel
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(svgLabel);

    // Create a central widget and set the layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    frameCounter = 1;
    QTimer::singleShot(5000, [this]() {
        // Start the QR code update logic after 5 seconds
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &kuvatesti::updateQRCode);

        int intervalMs = static_cast<int>(interval);  // Integer part of the interval
        qDebug() << "Interval (float):" << interval;
        timer->start(16);
    });

}

void kuvatesti::updateQRCode() {
    // Get the current timestamp
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("yyyy-MM-dd HH:mm:ss.zzz");

    // Append the timestamp and frame number to the text to encode
    QString textToEncode = "QR Code: " + timestamp + QString::number(frameCounter);

    // Encode the text (with the timestamp and frame number) into the QR code
    const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;
    const QrCode qr = QrCode::encodeText(textToEncode.toUtf8().constData(), errCorLvl);

    // Convert the QR code into an SVG string
    QString svgData = toSvgString(qr, 4);

    // Convert the SVG string to QByteArray
    QByteArray byteArray = svgData.toUtf8();

    // Pass the actual QByteArray to QSvgRenderer
    QSvgRenderer svgRenderer(byteArray);

    // Create a QPixmap to render the SVG into (set size if necessary)
    QPixmap pixmap(600, 600);
    pixmap.fill(Qt::transparent);  // Ensure the background is transparent

    // Create a QPainter to paint on the QPixmap
    QPainter painter(&pixmap);

    // Render the SVG onto the pixmap
    svgRenderer.render(&painter);

    // Set the pixmap to the QLabel to display
    svgLabel->setPixmap(pixmap);

    // Save the QR code data to the JSON file
    saveQRCodeDataToJson(textToEncode, timestamp, frameCounter);

    // Increment the frame counter for the next frame
    frameCounter++;
}

void kuvatesti::saveQRCodeDataToJson(const QString &qrData, const QString &timestamp, int frameNumber) {
    // Set the path for the JSON file (for example, in the user's Documents folder)
    QString documentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = documentsFolder + "/qr_data.json";

    // Open the file for reading the current JSON data
    QFile file(filePath);
    QJsonDocument jsonDoc;
    QJsonArray framesArray;

    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            // Load the existing content of the file
            QByteArray fileData = file.readAll();
            jsonDoc = QJsonDocument::fromJson(fileData);
            file.close();
        }

        if (!jsonDoc.isNull() && jsonDoc.isArray()) {
            // Get the existing array of frames
            framesArray = jsonDoc.array();
        }
    }

    // Create a JSON object to store the QR code data, timestamp, and frame number
    QJsonObject frameObject;
    frameObject["qr_data"] = qrData;
    frameObject["timestamp"] = timestamp;
    frameObject["frame_number"] = frameNumber;

    // Append the new frame data to the array
    framesArray.append(frameObject);

    // Create a JSON document from the updated array
    jsonDoc = QJsonDocument(framesArray);

    // Open the file for writing (overwrite with new data)
    if (file.open(QIODevice::WriteOnly)) {
        // Write the updated JSON document to the file
        file.write(jsonDoc.toJson());
        file.close();
    } else {
        qDebug() << "Failed to open file for writing: " << filePath;
    }
}

kuvatesti::~kuvatesti()
{
    delete ui;
}
