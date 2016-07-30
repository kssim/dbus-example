#include <string.h>
#include <unistd.h>
#include "common.h"

extern DBusConnection *g_connection;

int waitSignal(void) {
    DBusMessage *message;
    DBusMessageIter args;
    DBusError error;
    char *signalParam;
    char busRule[BUFSIZ];

    dbus_error_init(&error);
    printf("Waiting signal message with interface '%s'\n", DBUS_BUS_INTERFACE);

    memset(busRule, 0x0, BUFSIZ);
    snprintf(busRule, BUFSIZ, "type='signal', interface='%s'", DBUS_BUS_INTERFACE);

    dbus_bus_add_match(g_connection, busRule, &error);
    dbus_connection_flush(g_connection);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, " Rule is error : %s \n", error.message);
        dbus_error_free(&error);
        return -1;
    }

    while (1) {
        dbus_connection_read_write(g_connection, 0);
        message = dbus_connection_pop_message(g_connection);

        if (message == NULL) {
            sleep(1);
            continue;
        }

        if (dbus_message_is_signal(message, DBUS_BUS_INTERFACE, DBUS_SIGNAL_NAME)) {
            if (!dbus_message_iter_init(message, &args)) {
                printf("Message has no parameters.\n");
            }
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) {
                printf("Argument is not string.\n");
            }
            else {
                dbus_message_iter_get_basic(&args, &signalParam);
            }

            if (signalParam == NULL) {
                printf("Cache empty signal.\n");
            }
            else {
                printf("Signal value is '%s'.\n", signalParam);
                signalParam = NULL;
            }
        }
        dbus_message_unref(message);
    }
    dbus_error_free(&error);
    return 0;
}

int main(void) {
    if (initConnection() != 0) {
        return -1;
    }

    if (requestBusName(DBUS_BUS_SERVER_NAME) != 0) {
        return -1;
    }

    if (waitSignal() != 0) {
        return -1;
    }

    return 0;
}
