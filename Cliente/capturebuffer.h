#ifndef CAPTUREBUFFER_H
#define CAPTUREBUFFER_H

#include <QAbstractVideoSurface>
#include <QList>
#include <QAbstractPlanarVideoBuffer>
#include <QCamera>
#include <QImage>


class CaptureBuffer : public QAbstractVideoSurface
{

signals:
    void s_image(const QImage&);

public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const{

        Q_UNUSED(handleType);
        QList<QVideoFrame::PixelFormat> formats;
        formats << QVideoFrame::Format_ARGB32;
        formats << QVideoFrame::Format_ARGB32_Premultiplied;
        formats << QVideoFrame::Format_RGB32;
        formats << QVideoFrame::Format_RGB24;
        formats << QVideoFrame::Format_RGB565;
        formats << QVideoFrame::Format_RGB555;

        return formats;
    }

    bool present(const QVideoFrame &frame){

        QVideoFrame frame2(frame);
        frame2.map(QAbstractVideoBuffer::ReadOnly);

        QImage frameAsImage = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

        //Código para trabajar con frameAsImage
        frameAsImage.copy();
        emit s_image(frameAsImage);
        frame2.unmap();


        return true;
    }
};

#endif // CAPTUREBUFFER_H
