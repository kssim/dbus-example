#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>

#define DBUS_BUS_NAME				"com.kssim.test"
#define DBUS_BUS_SERVER_NAME   		"com.kssim.test.server"
#define DBUS_BUS_INTERFACE    	 	"com.kssim.test.bus"
#define DBUS_BUS_INTERFACE_PATH		"/com/kssim/test/bus"
#define DBUS_SIGNAL_NAME        	"SignalTest"
#define DBUS_METHOD_NAME        "MethodTest"

DBusConnection *g_connection;

int initConnection(void);
int requestBusName(const char* busName);

#endif //__COMMON_H__
