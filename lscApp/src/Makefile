TOP=../..

include $(TOP)/configure/CONFIG

PROD_IOC = lsc

DBD += lsc.dbd

lsc_DBD += base.dbd
lsc_DBD += stream.dbd
lsc_DBD += asyn.dbd
lsc_DBD += asSupport.dbd
lsc_DBD += drvAsynIPPort.dbd
lsc_DBD += lscSup.dbd

lsc_SRCS += lsc_registerRecordDeviceDriver.cpp lscASub.c
lsc_SRCS_DEFAULT += main.cpp

lsc_LIBS += asyn
lsc_LIBS += autosave
lsc_LIBS += stream

lsc_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES

