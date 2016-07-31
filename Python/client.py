#!/usr/bin/env python

import dbus

DBUS_BUS_NAME = 'com.kssim.test'
DBUS_BUS_PATH = '/com/kssim/test'

if __name__ == '__main__':
    bus = dbus.SystemBus()
    system = bus.get_object(DBUS_BUS_NAME, DBUS_BUS_PATH)

    reply_message = system.get_dbus_method('reply_msg', DBUS_BUS_NAME)
    send_arg_and_msg = system.get_dbus_method('send_arg_and_msg', DBUS_BUS_NAME)

    print(reply_message())
    print(send_arg_and_msg('Hello', 'world'))
