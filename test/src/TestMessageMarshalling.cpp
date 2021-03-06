// Local
#include "TestMessageMarshalling.h"

// CuteIPC
#include <CuteIPCMessage_p.h>
#include <CuteIPCMarshaller_p.h>

// Qt
#include <QtTest/QtTest>
#include <QtGui>


void TestMessageMarshalling::init()
{
  qRegisterMetaType<QList<QImage> >("QList<QImage>");
}


void TestMessageMarshalling::constructMessageWithoutArgs()
{
  CuteIPCMessage::Arguments args;
  CuteIPCMessage message(CuteIPCMessage::SignalConnectionRequest, "testSignalSignature()", args);
  QCOMPARE(message.messageType(), CuteIPCMessage::SignalConnectionRequest);
  QCOMPARE(message.method(), QString("testSignalSignature()"));
  QVERIFY(message.returnType().isNull());

  CuteIPCMessage message2(CuteIPCMessage::MessageCallWithReturn, "testMethod", args, "QString");
  QCOMPARE(message2.messageType(), CuteIPCMessage::MessageCallWithReturn);
  QCOMPARE(message2.method(), QString("testMethod"));
  QCOMPARE(message2.returnType(), QString("QString"));

  CuteIPCMessage message3(CuteIPCMessage::MessageError, "errorString");
  QCOMPARE(message3.messageType(), CuteIPCMessage::MessageError);
  QCOMPARE(message3.method(), QString("errorString"));
  QVERIFY(message3.returnType().isNull());

  QVERIFY(message.arguments().isEmpty());
  QVERIFY(message2.arguments().isEmpty());
  QVERIFY(message3.arguments().isEmpty());
}


void TestMessageMarshalling::constructMessageWithArgs()
{
  int testInt = 5;
  QString testString = "test";
  bool testBool = true;

  CuteIPCMessage message(CuteIPCMessage::MessageCallWithReturn, "testCallMessage", Q_ARG(int, testInt),
                         Q_ARG(QString, testString), Q_ARG(bool, testBool));

  QCOMPARE(message.messageType(), CuteIPCMessage::MessageCallWithReturn);
  QCOMPARE(message.method(), QString("testCallMessage"));
  QVERIFY(message.returnType().isNull());

  QCOMPARE(message.arguments().size(),3);
  QCOMPARE(message.arguments().at(0).name(), "int");
  QCOMPARE(message.arguments().at(0).data(), &testInt);
  QCOMPARE(message.arguments().at(1).name(), "QString");
  QCOMPARE(message.arguments().at(1).data(), &testString);
  QCOMPARE(message.arguments().at(2).name(), "bool");
  QCOMPARE(message.arguments().at(2).data(), &testBool);

  CuteIPCMessage::Arguments args;
  args.append(QGenericArgument("int", &testInt));
  args.append(QGenericArgument("QString", &testString));
  args.append(QGenericArgument("bool", &testBool));

  CuteIPCMessage message2(CuteIPCMessage::MessageCallWithReturn, "testCallMessageWithDirectArgs", args);

  QCOMPARE(message2.messageType(), CuteIPCMessage::MessageCallWithReturn);
  QCOMPARE(message2.method(), QString("testCallMessageWithDirectArgs"));
  QVERIFY(message2.returnType().isNull());

  QCOMPARE(message2.arguments().size(),3);
  QCOMPARE(message2.arguments().at(0).name(), "int");
  QCOMPARE(message2.arguments().at(0).data(), &testInt);
  QCOMPARE(message2.arguments().at(1).name(), "QString");
  QCOMPARE(message2.arguments().at(1).data(), &testString);
  QCOMPARE(message2.arguments().at(2).name(), "bool");
  QCOMPARE(message2.arguments().at(2).data(), &testBool);
}


void TestMessageMarshalling::marshallMessageParameters()
{
  int intVal = 5;
  CuteIPCMessage testMessage(CuteIPCMessage::MessageCallWithReturn, "testMessage", Q_ARG(int, intVal), "int");

  QByteArray serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  CuteIPCMessage deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QCOMPARE(testMessage.messageType(), deserializedMessage.messageType());
  QCOMPARE(testMessage.method(), deserializedMessage.method());
  QCOMPARE(testMessage.returnType(), deserializedMessage.returnType());
  QCOMPARE(testMessage.arguments().size(), deserializedMessage.arguments().size());
  for (int i = 0; i < testMessage.arguments().size(); ++i)
  {
    QCOMPARE(testMessage.arguments().at(i).name(), deserializedMessage.arguments().at(i).name());
  }


  testMessage = CuteIPCMessage(CuteIPCMessage::MessageError, "testErrorMessage");
  serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QCOMPARE(testMessage.messageType(), deserializedMessage.messageType());
  QCOMPARE(testMessage.method(), deserializedMessage.method());
  QCOMPARE(testMessage.returnType(), deserializedMessage.returnType());
  QCOMPARE(testMessage.arguments().size(), deserializedMessage.arguments().size());
  for (int i = 0; i < testMessage.arguments().size(); ++i)
  {
    QCOMPARE(testMessage.arguments().at(i).name(), deserializedMessage.arguments().at(i).name());
  }
}


void TestMessageMarshalling::marshallIntergers()
{
  bool boolVal = true;
  int intVal = 2;
  uint uintVal = 3;
  long longVal= 4;
  short shortVal = 5;

  float floatVal = 3.0;
  double doubleVal = 4.0;

  CuteIPCMessage testMessage(CuteIPCMessage::MessageCallWithoutReturn, "testMessage",
                             Q_ARG(bool, boolVal), Q_ARG(int, intVal), Q_ARG(uint, uintVal), Q_ARG(long, longVal),
                             Q_ARG(short, shortVal), Q_ARG(float, floatVal), Q_ARG(double, doubleVal));

  QByteArray serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  CuteIPCMessage deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QVERIFY(deserializedMessage.arguments().size() == 7);
  QCOMPARE(deserializedMessage.arguments().at(0).name(), "bool");
  QCOMPARE(deserializedMessage.arguments().at(1).name(), "int");
  QCOMPARE(deserializedMessage.arguments().at(2).name(), "uint");
  QCOMPARE(deserializedMessage.arguments().at(3).name(), "long");
  QCOMPARE(deserializedMessage.arguments().at(4).name(), "short");
  QCOMPARE(deserializedMessage.arguments().at(5).name(), "float");
  QCOMPARE(deserializedMessage.arguments().at(6).name(), "double");

  QCOMPARE(*reinterpret_cast<bool*>(deserializedMessage.arguments().at(0).data()), boolVal);
  QCOMPARE(*reinterpret_cast<int*>(deserializedMessage.arguments().at(1).data()), intVal);
  QCOMPARE(*reinterpret_cast<uint*>(deserializedMessage.arguments().at(2).data()), uintVal);
  QCOMPARE(*reinterpret_cast<long*>(deserializedMessage.arguments().at(3).data()), longVal);
  QCOMPARE(*reinterpret_cast<short*>(deserializedMessage.arguments().at(4).data()), shortVal);
  QCOMPARE(*reinterpret_cast<float*>(deserializedMessage.arguments().at(5).data()), floatVal);
  QCOMPARE(*reinterpret_cast<double*>(deserializedMessage.arguments().at(6).data()), doubleVal);

}


void TestMessageMarshalling::marshallLiterals()
{
  QString testString = "testString";
  QChar testChar(65);

  CuteIPCMessage testMessage(CuteIPCMessage::MessageCallWithoutReturn, "testMessage",
                             Q_ARG(QString, testString), Q_ARG(QChar, testChar));

  QByteArray serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  CuteIPCMessage deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QVERIFY(deserializedMessage.arguments().size() == 2);
  QCOMPARE(deserializedMessage.arguments().at(0).name(), "QString");
  QCOMPARE(deserializedMessage.arguments().at(1).name(), "QChar");

  QCOMPARE(*(reinterpret_cast<QString*>(deserializedMessage.arguments().at(0).data())), testString);
  QCOMPARE(*(reinterpret_cast<QChar*>(deserializedMessage.arguments().at(1).data())), testChar);
}


void TestMessageMarshalling::marshallQImageRGB888()
{
  QImage testImage = createRGB888Image();
  CuteIPCMessage testMessage(CuteIPCMessage::MessageCallWithoutReturn, "testMessage", Q_ARG(QImage, testImage));

  QByteArray serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  CuteIPCMessage deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QVERIFY(deserializedMessage.arguments().size() == 1);
  QCOMPARE(deserializedMessage.arguments().at(0).name(), "QImage");
  QImage* deserializedImage = reinterpret_cast<QImage*>(deserializedMessage.arguments().at(0).data());

  QCOMPARE(deserializedImage->size(), testImage.size());
  QCOMPARE(deserializedImage->pixel(0, 0), testImage.pixel(0, 0));
  QCOMPARE(deserializedImage->pixel(50, 50), testImage.pixel(50, 50));
  QCOMPARE(deserializedImage->pixel(deserializedImage->width() - 1, deserializedImage->height() - 1), testImage.pixel(testImage.width() - 1, testImage.height() - 1));
  QCOMPARE(deserializedImage->format(), testImage.format());
  QCOMPARE(deserializedImage->colorCount(), testImage.colorCount());
  QCOMPARE(deserializedImage->colorTable(), testImage.colorTable());
  QCOMPARE(deserializedImage->bytesPerLine(), testImage.bytesPerLine());

  CuteIPCMarshaller::freeArguments(deserializedMessage.arguments());
}


void TestMessageMarshalling::marshallQImageIndexed8()
{
  QImage testImage = createIndexed8Image();
  CuteIPCMessage testMessage(CuteIPCMessage::MessageCallWithoutReturn, "testMessage", Q_ARG(QImage, testImage));

  QByteArray serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  CuteIPCMessage deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QVERIFY(deserializedMessage.arguments().size() == 1);
  QCOMPARE(deserializedMessage.arguments().at(0).name(), "QImage");
  QImage* deserializedImage = reinterpret_cast<QImage*>(deserializedMessage.arguments().at(0).data());

  QCOMPARE(deserializedImage->size(), testImage.size());
  QCOMPARE(deserializedImage->pixel(0, 0), testImage.pixel(0, 0));
  QCOMPARE(deserializedImage->pixel(50, 50), testImage.pixel(50, 50));
  QCOMPARE(deserializedImage->pixel(deserializedImage->width() - 1, deserializedImage->height() - 1), testImage.pixel(testImage.width() - 1, testImage.height() - 1));
  QCOMPARE(deserializedImage->format(), testImage.format());
  QCOMPARE(deserializedImage->colorCount(), testImage.colorCount());
  QCOMPARE(deserializedImage->colorTable(), testImage.colorTable());
  QCOMPARE(deserializedImage->bytesPerLine(), testImage.bytesPerLine());

  CuteIPCMarshaller::freeArguments(deserializedMessage.arguments());
}


void TestMessageMarshalling::marshallQListOfQImage()
{
  QImage testRGB888Image = createRGB888Image();
  QImage testIndexed8Image = createIndexed8Image();
  QList<QImage> testImageList = QList<QImage>() << testRGB888Image << testIndexed8Image;

  CuteIPCMessage testMessage(CuteIPCMessage::MessageCallWithoutReturn, "testMessage", Q_ARG(QList<QImage>, testImageList));

  QByteArray serializedMessage = CuteIPCMarshaller::marshallMessage(testMessage);
  CuteIPCMessage deserializedMessage = CuteIPCMarshaller::demarshallMessage(serializedMessage);

  QVERIFY(deserializedMessage.arguments().size() == 1);
  QCOMPARE(deserializedMessage.arguments().at(0).name(), "QList<QImage>");
  QList<QImage>* deserializedImageList = reinterpret_cast<QList<QImage>*>(deserializedMessage.arguments().at(0).data());
  QCOMPARE(deserializedImageList->size(), testImageList.length());

  const QImage& deserializedRGB888Image = deserializedImageList->first();
  QCOMPARE(deserializedRGB888Image.size(), testRGB888Image.size());
  QCOMPARE(deserializedRGB888Image.pixel(0, 0), testRGB888Image.pixel(0, 0));
  QCOMPARE(deserializedRGB888Image.pixel(50, 50), testRGB888Image.pixel(50, 50));
  QCOMPARE(deserializedRGB888Image.pixel(deserializedRGB888Image.width() - 1, deserializedRGB888Image.height() - 1), testRGB888Image.pixel(testRGB888Image.width() - 1, testRGB888Image.height() - 1));
  QCOMPARE(deserializedRGB888Image.format(), testRGB888Image.format());
  QCOMPARE(deserializedRGB888Image.colorCount(), testRGB888Image.colorCount());
  QCOMPARE(deserializedRGB888Image.colorTable(), testRGB888Image.colorTable());
  QCOMPARE(deserializedRGB888Image.bytesPerLine(), testRGB888Image.bytesPerLine());

  const QImage& deserializedIndexed8Image = deserializedImageList->last();
  QCOMPARE(deserializedIndexed8Image.size(), testIndexed8Image.size());
  QCOMPARE(deserializedIndexed8Image.pixel(0, 0), testIndexed8Image.pixel(0, 0));
  QCOMPARE(deserializedIndexed8Image.pixel(50, 50), testIndexed8Image.pixel(50, 50));
  QCOMPARE(deserializedIndexed8Image.pixel(deserializedIndexed8Image.width() - 1, deserializedIndexed8Image.height() - 1), testIndexed8Image.pixel(testIndexed8Image.width() - 1, testIndexed8Image.height() - 1));
  QCOMPARE(deserializedIndexed8Image.format(), testIndexed8Image.format());
  QCOMPARE(deserializedIndexed8Image.colorCount(), testIndexed8Image.colorCount());
  QCOMPARE(deserializedIndexed8Image.colorTable(), testIndexed8Image.colorTable());
  QCOMPARE(deserializedIndexed8Image.bytesPerLine(), testIndexed8Image.bytesPerLine());

  CuteIPCMarshaller::freeArguments(deserializedMessage.arguments());
}


QImage TestMessageMarshalling::createRGB888Image() const
{
  QImage image(759, 663, QImage::Format_RGB888);
  image.fill(0);
  image.setPixel(50, 50, qRgb(255, 0, 0));
  return image;
}


QImage TestMessageMarshalling::createIndexed8Image() const
{
  QImage image(759, 657, QImage::Format_Indexed8);
  image.setColorCount(255);
  for (int i = 0; i <= 255; ++i)
    image.setColor(i, qRgb(i, i, i));
  image.fill(255);
  image.scanLine(50)[50] = 0;
  return image;
}


QTEST_MAIN(TestMessageMarshalling)
