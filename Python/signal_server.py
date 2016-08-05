#!/usr/bin/env python

import time
import dbus
import dbus.service
import gobject
from dbus.mainloop.glib import DBusGMainLoop

DBUS_BUS_NAME = 'com.kssim.test'
DBUS_OBJECT_PATH = '/com/kssim/test'
DBUS_INTERFACE = 'com.kssim.test.signal'
DBUS_SIGNAL_NAME = 'daemon_test'

class EventHandler(dbus.service.Object):

    print_msg = 'Hello world'

    def __init__(self):
        bus = dbus.SystemBus()
        bus.add_signal_receiver(self.receive_signal, dbus_interface=DBUS_INTERFACE, signal_name=DBUS_SIGNAL_NAME)

        bus_name = dbus.service.BusName(DBUS_BUS_NAME, bus)
        dbus.service.Object.__init__(self, bus_name, DBUS_OBJECT_PATH)

    @dbus.service.method(DBUS_BUS_NAME)
    def receive_signal(self, in_msg):
        print ('I got the message(\'%s\')' % in_msg)
        self.print_msg = in_msg
        return True

    def repeat_print_msg(self):
        print (self.print_msg)
        return True


if __name__ == '__main__':
    DBusGMainLoop(set_as_default=True)

    try:
        loop = gobject.MainLoop()
        handler = EventHandler()

        gobject.timeout_add_seconds(1, handler.repeat_print_msg)
        loop.run()
    except KeyboardInterrupt:
        print ('Daemon exit.')
        GLib.MainLoop().quit()
