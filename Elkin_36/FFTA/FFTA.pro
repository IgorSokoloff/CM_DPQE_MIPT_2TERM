TEMPLATE = app
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4.7): QT += widgets printsupport

SOURCES += main.cpp \
		   ffta.cpp \
		   utilities.cpp

HEADERS += ffta.h \
		   utilities.h \
    functions.h

DISTFILES += \
	README.md

DEFINES += QCUSTOMPLOT_USE_LIBRARY

CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd1
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot1
  else: QCPLIB = qcustomplot
}
LIBS += -L./ -l$$QCPLIB
