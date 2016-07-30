#include "common.h"

DBusConnection *g_connection;

int initConnection(void) {
    DBusError error;

    dbus_error_init(&error);
    g_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Connection error : %s \n", error.message);
        dbus_error_free(&error);
    }

    if (g_connection == NULL) {
        fprintf(stderr, "Connection is NULL \n");
        return -1;
    }

    return 0;
}

int requestBusName(const char* busName) {
    DBusError error;
    int ret;

    dbus_error_init(&error);
    ret = dbus_bus_request_name(g_connection, busName, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Name error : %s \n", error.message);
        dbus_error_free (&error);
    }

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Not primary owner : %d \n", ret);
        return -1;
    }

    return 0;
}
