QT = core widgets network
requires(qtConfig(tableview))
CONFIG -= app

SOURCES += \
    main.cpp \ 
    mainwindow.cpp \
    policy.cpp \
    difftool.cpp \
    instance.cpp \
    instances.cpp \
    policies.cpp \
    copy.cpp

FORMS += \
    mainwindow.ui \
    policy.ui \
    difftool.ui \
    instance.ui \
    instances.ui \
    copy.ui

HEADERS += \
    mainwindow.h \
    version.h \
    policy.h \
    difftool.h \
    instance.h \
    instances.h \
    policies.h \
    copy.h

RESOURCES += \
    cb.qrc

TRANSLATIONS = cb_fr_FR.ts
QMAKE_POST_LINK = lrelease cb.pro
