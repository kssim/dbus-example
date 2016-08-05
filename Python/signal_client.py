#!/usr/bin/env python

import dbus

DBUS_BUS_NAME = 'com.kssim.test'
DBUS_OBJECT_PATH = '/com/kssim/test'


if __name__ == '__main__':
    try:
        bus = dbus.SystemBus()
        bus_object = bus.get_object(DBUS_BUS_NAME, DBUS_OBJECT_PATH)
        bus_interface = dbus.Interface(bus_object, DBUS_BUS_NAME)

        bus_interface.receive_signal('send_signal')
    except:
        print ('Server is offline.')
