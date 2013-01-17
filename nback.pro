OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp

SEQUENCE_TEST {
	DEFINES += SEQUENCE_TEST
	QT += testlib
	SOURCES += sequence.cpp
	HEADERS += sequence.h
} else {
	FORMS += mainwindow.ui settings.ui
	SOURCES += main.cpp mainwindow.cpp sequence.cpp settings.cpp
	HEADERS += mainwindow.h sequence.h settings.h
}
