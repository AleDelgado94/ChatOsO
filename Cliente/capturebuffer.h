#ifndef CAPTUREBUFFER_H
#define CAPTUREBUFFER_H

#include <QAbstractVideoSurface>
#include <QList>
#include <QAbstractPlanarVideoBuffer>
#include <QCamera>
#include <QImage>

#include "perfilusers.h"

class CaptureBuffer : public QAbstractVideoSurface
{

private:
    QCamera mycamera;

public:
    CaptureBuffer(QCamera* camera);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool present(QVideoFrame &frame);
};

#endif // CAPTUREBUFFER_H
