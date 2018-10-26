#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T10:30:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pop_wil
TEMPLATE = app

INCLUDEPATH += ../pop_wil/charview_include

#注： 1、-L 参数指定 .lib/.a 文件的位置
#        2、-l  参数指定导入库文件名(不要加扩展名)
#        3、另外，导入库文件的路径中，反斜杠用的是向右倾斜的
#        4、Qt要隐式调用DLL的话，须把dll文件放在程序的输出目录中，lib所存放的文件路径随便，但是须由上述.pro文件中的LIBS指定语句指定其所在文件夹，.h文件放在程序当前目录下即可。

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.


DEFINES += QT_DEPRECATED_WARNINGS CHARTDIR_HIDE_OBSOLETE _CRT  _SECURE_NO_WARNINGS




# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    loginscreen.cpp \
    channel_param.cpp \
    control_param.cpp \
    performancetimer.cpp \
    qchartviewer.cpp \
    randomwalk.cpp \
    new_experiment.cpp \
    set_measurement_unit.cpp \
    loadingscreen.cpp \
    sinewave.cpp \
    restricted_para.cpp \
    shake_table_para.cpp



HEADERS += \
    mainwindow.h \
    loginscreen.h \
    control_param.h \
    qdoublebufferedqueue.h \
    performancetimer.h \
    qchartviewer.h \
    randomwalk.h \
    channel_param.h \
    new_experiment.h \
    set_measurement_unit.h \
    loadingscreen.h \
    sinewave.h \
    restricted_para.h \
    shake_table_para.h



FORMS += \
        mainwindow.ui \
    loginscreen.ui \
    channel_param.ui \
    control_param.ui \
    new_experiment.ui \
    set_measurement_unit.ui \
    sinewave.ui \
    restricted_para.ui \
    shake_table_para.ui

RESOURCES += \
    assets.qrc

include($$PWD/tcp_server/tcp_server.pri)
include($$PWD/protocol/protocol.pri)
include($$PWD/init_helper/init_helper.pri)
include($$PWD/ui_style/ui_style.pri)
#qcharview动态链接库

LIBS += -lwinmm  #PerformanceTimer required
win32:LIBS += ../pop_wil/lib/chartdir60.lib
win32:QMAKE_POST_LINK = copy /Y ..\\pop_wil\\lib\\chartdir60.dll $(DESTDIR)

macx:LIBS += -L./lib -lchartdir
macx:QMAKE_POST_LINK += mkdir -p \"`dirname $(TARGET)`/../Frameworks\";
macx:QMAKE_POST_LINK += cp ../pop_wil/lib/libchartdir.6.dylib \"`dirname $(TARGET)`/../Frameworks\";
macx:QMAKE_POST_LINK += install_name_tool -change libchartdir.6.dylib \
     \@loader_path/../Frameworks/libchartdir.6.dylib \"$(TARGET)\";

unix:!macx:LIBS += -L./lib -lchartdir
unix:!macx:QMAKE_RPATHDIR += ./lib
