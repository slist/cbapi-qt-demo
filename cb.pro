QT = core gui widgets network
#requires(qtConfig(tableview))
#CONFIG -= app

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5")

SOURCES += \
    banned.cpp \
    droparea.cpp \
    edr.cpp \
    feed.cpp \
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
    cb.qrc

TRANSLATIONS = cb_fr_FR.ts
