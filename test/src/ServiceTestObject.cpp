// Local
#include "ServiceTestObject.h"

// Qt
#include <QtGui/QImage>


ServiceTestObject::ServiceTestObject(QObject* parent)
: CuteIPCService(parent)
{}


int ServiceTestObject::testQByteArrayTransfer(const QByteArray& ba)
{
  m_byteArray = ba;
  return ba.size();
}


int ServiceTestObject::testQImageTransfer(const QImage& img)
{
  m_image = img;
  return img.byteCount();
}


int ServiceTestObject::testQListOfQImageTransfer(const QList<QImage>& imgList)
{
  m_imageList = imgList;

  int byteCount = 0;
  for (int i = 0; i < imgList.size(); ++i)
    byteCount += imgList.at(i).byteCount();

  return byteCount;
}


int ServiceTestObject::testQStringTransfer(const QString& string)
{
  m_string = string;
  return string.size();
}


QString ServiceTestObject::testQStringTransfer2(const QString& string)
{
  m_string = string;
  return string + string;
}


void ServiceTestObject::testIntTransfer(int value)
{
  m_int = value;
}


#include <QEventLoop>
#include <QTimer>
int ServiceTestObject::testCallWithRemoteSignal(int value)
{
  m_int = value;
  emit serviceIntSignal(m_int + 42);

  QTimer timer;
  QEventLoop loop;
  connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
  timer.start(1500);
  loop.exec();

  return value + 15;
}


int ServiceTestObject::testQImageGrayScale(const QImage& image)
{
  QImage originImage(":/images/finger.png");
  if (originImage.width() != image.width() || originImage.height() != image.height())
    return -1;

  for (int y = 0; y < originImage.height(); ++y)
    for (int x = 0; x < originImage.width(); ++x)
      if (qRed(originImage.pixel(x, y)) != qRed(image.pixel(x, y))
          || qGreen(originImage.pixel(x, y)) != qGreen(image.pixel(x, y))
          || qBlue(originImage.pixel(x, y)) != qBlue(image.pixel(x, y)))
        return -1;

  return image.byteCount();
}


QImage ServiceTestObject::testQImageReturn(const QImage& image)
{
  return image;
}


void ServiceTestObject::serviceQByteArraySlot(const QByteArray& ba)
{
  this->testQByteArrayTransfer(ba);
  emit slotWasCalled(Q_FUNC_INFO);
}


void ServiceTestObject::serviceQImageSlot(const QImage& img)
{
  this->testQImageTransfer(img);
  emit slotWasCalled(Q_FUNC_INFO);
}


void ServiceTestObject::serviceQListOfQImageSlot(const QList<QImage>& imgList)
{
  this->testQListOfQImageTransfer(imgList);
  emit slotWasCalled(Q_FUNC_INFO);
}


void ServiceTestObject::serviceQStringSlot(const QString& str)
{
  this->testQStringTransfer(str);
  emit slotWasCalled(Q_FUNC_INFO);
}


void ServiceTestObject::serviceIntSlot(int value)
{
  this->testIntTransfer(value);
  emit slotWasCalled(Q_FUNC_INFO);
}


void ServiceTestObject::serviceQStringIntSlot(const QString& str, int value)
{
  this->testQStringTransfer(str);
  this->testIntTransfer(value);
  emit slotWasCalled(Q_FUNC_INFO);
}


void ServiceTestObject::serviceAnotherIntSlot(int value)
{
  this->testIntTransfer(value);
  emit anotherSlotWasCalled(Q_FUNC_INFO);
}


const QByteArray& ServiceTestObject::getByteArray() const
{
  return m_byteArray;
}


QImage ServiceTestObject::getImage() const
{
  return m_image;
}

QList<QImage> ServiceTestObject::getImageList() const
{
  return m_imageList;
}


const QString& ServiceTestObject::getString() const
{
  return m_string;
}


int ServiceTestObject::getInt() const
{
  return m_int;
}

void ServiceTestObject::emitQByteArraySignal(const QByteArray& ba)
{
  emit serviceQByteArraySignal(ba);
}

void ServiceTestObject::emitQImageSignal(const QImage& img)
{
  emit serviceQImageSignal(img);
}

void ServiceTestObject::emitQListOfQImageSignal(const QList<QImage>& imgList)
{
  emit serviceQListOfQImageSignal(imgList);
}

void ServiceTestObject::emitQStringSignal(const QString& message)
{
  emit serviceQStringSignal(message);
}

void ServiceTestObject::emitIntSignal(int value)
{
  emit serviceIntSignal(value);
}

void ServiceTestObject::emitQStringIntSignal(const QString& message, int value)
{
  emit serviceQStringIntSignal(message, value);
}


void ServiceTestObject::emitTcpFinish()
{
  emit tcpFinish();
}
