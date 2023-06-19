QT = core gui widgets network

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5 minimum")

greaterThan(QT_MAJOR_VERSION, 5) {
    QT += core5compat
}

DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += \
    banned.cpp \
    droparea.cpp \
    edr.cpp \
    feed.cpp \
    instanceappcontrol.cpp \
    instanceedr.cpp \
    ioc.cpp \
    ioclist.cpp \
    iocs.cpp \
    iocsyntaxhighlighter.cpp \
    iplist.cpp \
    iplistadd.cpp \
    main.cpp \
    ngav.cpp \
    qjsonmodel.cpp \
    policy.cpp \
    difftool.cpp \
    instance.cpp \
    instances.cpp \
    policies.cpp \
    copy.cpp \
    homewindow.cpp \
    eraser.cpp \
    iocmainwindow.cpp \
    report.cpp \
    reportobject.cpp \
    servers.cpp \
    welcome.cpp

FORMS += \
    banned.ui \
    edr.ui \
    feed.ui \
    instanceappcontrol.ui \
    instanceedr.ui \
    ioc.ui \
    ioclist.ui \
    iocs.ui \
    iplist.ui \
    iplistadd.ui \
    ngav.ui \
    policy.ui \
    difftool.ui \
    instance.ui \
    instances.ui \
    copy.ui \
    homewindow.ui \
    eraser.ui \
    iocmainwindow.ui \
    report.ui \
    servers.ui \
    welcome.ui

HEADERS += \
    banned.h \
    droparea.h \
    edr.h \
    feed.h \
    instanceappcontrol.h \
    instanceedr.h \
    ioc.h \
    ioclist.h \
    iocs.h \
    iocsyntaxhighlighter.h \
    iplist.h \
    iplistadd.h \
    ngav.h \
    qjsonmodel.h \
    report.h \
    reportobject.h \
    servers.h \
    version.h \
    policy.h \
    difftool.h \
    instance.h \
    instances.h \
    policies.h \
    copy.h \
    homewindow.h \
    eraser.h \
    iocmainwindow.h \
    welcome.h

RESOURCES += \
    cbapi-qt-demo.qrc

TRANSLATIONS = cbapi-qt-demo_fr_FR.ts

DISTFILES += \
    README.md
