#include "capturebuffer.h"

CaptureBuffer::CaptureBuffer(QCamera* camera):
    mycamera(camera)
{
    CaptureBuffer* capturebuffer = new CaptureBuffer(camera);
    camera->setViewfinder(capturebuffer);

}

//Este metodo nos dara el frame para que los mostremos
bool CaptureBuffer::present(const QVideoFrame &frame)
{
    QImage frameAsImage = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    frame.map(QAbstractVideoBuffer::ReadOnly);
    //TODO: Código para trabajar con frameAsImage
    frame.unmap();
    return true;
}

//A traves de este metodo nos preguntan que formato de video soportamos
//Como vamos a guardar los frames en objetos QImage
QList<QVideoFrame::PixelFormat> CaptureBuffer::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const
{
    QList<QVideoFrame::PixelFormat> formats;
    formats << QVideoFrame::Format_ARGB32;
    formats << QVideoFrame::Format_ARGB32_Premultiplied;
    formats << QVideoFrame::Format_RGB32;
    formats << QVideoFrame::Format_RGB24;
    formats << QVideoFrame::Format_RGB565;
    formats << QVideoFrame::Format_RGB555;

    return formats;
}


