#include <string.h>
#include <unistd.h>
#include "common.h"

DBusConnection *g_connection;

void initConnection(void) {
    DBusError error;

    dbus_error_init(&error);
    g_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Connection error : %s\n", error.message);
        dbus_error_free(&error);
    }

    if (g_connection == NULL) {
        fprintf(stderr, "Connection is NULL\n");
        exit(1);
    }
}

void requestBusName(void) {
    DBusError error;
    int ret;

    dbus_error_init(&error);
    ret = dbus_bus_request_name(g_connection, DBUS_BUS_SERVER_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Name error : %s\n", error.message);
        dbus_error_free(&error);
    }

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Not primary owner : %d\n", ret);
        exit(1);
    }
}

void waitSignal(void) {
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
        exit(1);
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
}

int main(void) {
    initConnection();
    requestBusName();
    waitSignal();

    return 0;
}
