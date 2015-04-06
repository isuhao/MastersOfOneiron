# Add this to the constructor
# std::system("ln -s ../../../Source/Urho3D/bin/Data");
# std::system("ln -s ../../../Source/Urho3D/bin/CoreData");

LIBS += /home/frode/Sync/Source/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -lSDL \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++11



INCLUDEPATH += \
    /home/frode/Sync/Source/Urho3D/include \
    /home/frode/Sync/Source/Urho3D/include/Urho3D/ThirdParty \
/home/frode/Sync/Source/Urho3D/Source/Samples

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    mastercontrol.cpp \
    imp.cpp \
    tile.cpp \
    slot.cpp \
    #common.cpp \
    platform.cpp

#/home/frode/Sync/Source/Urho3D/Source/Samples/29_SoundSynthesis/SoundSynthesis.cpp
    #poolobject.cpp \
    #objectpool.cpp \
    #poolmaster.cpp \

HEADERS += \
    mastercontrol.h \
    imp.h \
    tile.h \
    slot.h \
    #common.h \
    platform.h

#/home/frode/Sync/Source/Urho3D/Source/Samples/29_SoundSynthesis/SoundSynthesis.h
    #poolobject.h \
    #objectpool.h \
    #poolmaster.h \
