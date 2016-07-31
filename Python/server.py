#!/usr/bin/env python

from gi.repository import GLib
import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop

DBUS_BUS_NAME = 'com.kssim.test'
DBUS_BUS_PATH = '/com/kssim/test'

class SystemDBus(dbus.service.Object):

    def __init__(self):
        bus_name = dbus.service.BusName(DBUS_BUS_NAME, bus=dbus.SystemBus())
        dbus.service.Object.__init__(self, bus_name, DBUS_BUS_PATH)

    @dbus.service.method(DBUS_BUS_NAME)
    def reply_msg(self):
        return "Hello Client~"


if __name__ == '__main__':
    DBusGMainLoop(set_as_default=True)
    SystemDBus()

    try:
        GLib.MainLoop().run()
    except KeyboardInterrupt:
        print("\nThe MainLoop will close...")
        GLib.MainLoop().quit()
