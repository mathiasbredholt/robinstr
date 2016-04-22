# from Phidgets.PhidgetException import *
# from Phidgets.Events.Events import *

"""Copyright 2011 Phidgets Inc.
This work is licensed under the Creative Commons Attribution 2.5 Canada License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/
"""

__author__ = "Adam Stelmack"
__version__ = "2.1.8"
__date__ = "14-Jan-2011 2:29:14 PM"


# Basic imports
import sys
from time import sleep
# Phidget specific imports
from Phidgets.PhidgetException import PhidgetException
from Phidgets.Devices.Bridge import Bridge, BridgeGain
from Phidgets.Phidget import PhidgetLogLevel
import gfx

WAIT_TIME = 2000

# Create a load cell object
try:
    bridge = Bridge()
except RuntimeError as e:
    gfx.log("Runtime Exception: %s" % e.details)
    gfx.log("Exiting....")
    exit(1)

# Information Display Function


def displayDeviceInfo():
    gfx.log(
        "|------------|----------------------------------|--------------|------------|")
    gfx.log(
        "|- Attached -|-              Type              -|- Serial No. -|-  Version -|")
    gfx.log(
        "|------------|----------------------------------|--------------|------------|")
    gfx.log("|- %8s -|- %30s -|- %10d -|- %8d -|" % (bridge.isAttached(),
                                                     bridge.getDeviceName(), bridge.getSerialNum(), bridge.getDeviceVersion()))
    gfx.log(
        "|------------|----------------------------------|--------------|------------|")
    gfx.log("Number of bridge inputs: %i" % (bridge.getInputCount()))
    gfx.log("Data Rate Max: %d" % (bridge.getDataRateMax()))
    gfx.log("Data Rate Min: %d" % (bridge.getDataRateMin()))
    gfx.log("Input Value Max: %d" % (bridge.getBridgeMax(0)))
    gfx.log("Input Value Min: %d" % (bridge.getBridgeMin(0)))

# Event Handler Callback Functions


def BridgeAttached(e):
    attached = e.device
    # gfx.log("Bridge %i Attached!" % (attached.getSerialNum()))


def BridgeDetached(e):
    detached = e.device
    # gfx.log("Bridge %i Detached!" % (detached.getSerialNum()))


def BridgeError(e):
    try:
        source = e.device
        gfx.log("Bridge %i: Phidget Error %i: %s" %
                (source.getSerialNum(), e.eCode, e.description))
    except PhidgetException as e:
        gfx.log("Phidget Exception %i: %s" % (e.code, e.details))


def BridgeData(e):
    source = e.device
    gfx.log("Bridge %i: Input %i: %f" %
            (source.getSerialNum(), e.index, e.value))


def init_phidget():
    try:
            # logging example, uncomment to generate a log file
        #bridge.enableLogging(PhidgetLogLevel.PHIDGET_LOG_VERBOSE, "phidgetlog.log")

        bridge.setOnAttachHandler(BridgeAttached)
        bridge.setOnDetachHandler(BridgeDetached)
        bridge.setOnErrorhandler(BridgeError)
        # bridge.setOnBridgeDataHandler(BridgeData)
    except PhidgetException as e:
        gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
        gfx.log("Exiting....")
        return False

    gfx.log("Opening phidget object....")

    #----------Attach phidget----------------
    try:
        bridge.openPhidget()
    except PhidgetException as e:
        gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
        return False

    gfx.log("Waiting for attach....")

    try:
        bridge.waitForAttach(WAIT_TIME)
    except PhidgetException as e:
        gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
        try:
            bridge.closePhidget()
        except PhidgetException as e:
            gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
            # gfx.log("Exiting....")
            return False
        # gfx.log("Exiting....")
        return False
    else:
        displayDeviceInfo()

    #----------Set parameters-----------------
    try:
        gfx.log("Set data rate to 8ms ...")
        bridge.setDataRate(8)
        sleep(2)

        gfx.log("Set Gain to 128...")
        bridge.setGain(2, BridgeGain.PHIDGET_BRIDGE_GAIN_128)
        sleep(2)

    #---------enable reading------------------
        gfx.log("Enable the Bridge input for reading data...")
        bridge.setEnabled(2, True)
        sleep(2)

    except PhidgetException as e:
        gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
        try:
            bridge.closePhidget()
        except PhidgetException as e:
            gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
            gfx.log("Exiting....")
            return False
        gfx.log("Exiting....")
        return False

    return True


def get_value(main_d):
    while main_d["running"]:
        summ = 0
        buffersize = 64
        for i in range(0, buffersize):
            summ = summ + bridge.getBridgeValue(2)
        main_d["pid"][0]["current"] = summ / buffersize
        gfx.log(main_d["pid1"]["current"])

    #----------disable reading --------------
    #gfx.log("Press Enter to quit....")

    #chr = sys.stdin.read(1)

    # gfx.log("Closing...")

    # try:
    #    gfx.log("Disable the Bridge input for reading data...")
    #    bridge.setEnabled(0, False)
    #    sleep(2)
    # except PhidgetException as e:
    #    gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
    #    try:
    #        bridge.closePhidget()
    #    except PhidgetException as e:
    #        gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
    #        gfx.log("Exiting....")
    #        exit(1)
    #    gfx.log("Exiting....")
    #    exit(1)

    # try:
    #    bridge.closePhidget()
    # except PhidgetException as e:
    #    gfx.log("Phidget Exception %i: %s" % (e.code, e.details))
    #    gfx.log("Exiting....")
    #    exit(1)

    # gfx.log("Done.")
    # exit(0)
