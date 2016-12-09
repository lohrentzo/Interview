TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    testbed.cpp

include(deployment.pri)
qtcAddDeployment()

