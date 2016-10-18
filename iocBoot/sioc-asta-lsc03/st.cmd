#!../../bin/linux-x86_64/lsc

epicsEnvSet( "STREAM_PROTOCOL_PATH","../../lscApp/Db:.")

< envPaths

cd ${TOP}

dbLoadDatabase "dbd/lsc.dbd"
lsc_registerRecordDeviceDriver pdbbase

epicsEnvSet("P",         "ASTA:LSC03")
epicsEnvSet("DESC",      "Shutter 3")
epicsEnvSet("PROTOFILE", "lsc.proto")
epicsEnvSet("DELAY0",    "0.1")

save_restoreSet_status_prefix( "")
save_restoreSet_IncompleteSetsOk( 1)
save_restoreSet_DatedBackupFiles( 1)
set_savefile_path( "/nfs/slac/g/testfac/asta/$(IOC)","autosave")
set_pass0_restoreFile( "lsc.sav")
set_pass1_restoreFile( "lsc.sav")

drvAsynIPPortConfigure("L0","ts-asta-01:2003",0,0,0)

#asynSetTraceMask("L0",-1,0x9)
#asynSetTraceIOMask("L0",-1,0x6)

dbLoadRecords("db/lsc.db","IOCNAME=${IOC},P=$(P),DESC=$(DESC),PROTOFILE=$(PROTOFILE),PORT=L0,DELAY0=$(DELAY0)")
dbLoadRecords("db/asynRecord.db","P=$(P):,R=asyn,PORT=L0,ADDR=0,OMAX=0,IMAX=0")

cd ${TOP}/iocBoot/${IOC}
iocInit()

create_monitor_set( "lsc.req",30,"P=$(P)")

