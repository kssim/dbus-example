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
        fprintf(stderr, "Connection is NULL.\n");
        exit(1);
    }
}

void requestBusName(void) {
    DBusError error;
    int ret;

    dbus_error_init(&error);

    ret = dbus_bus_request_name(g_connection, DBUS_BUS_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Request bus name error : %s\n", error.message);
        dbus_error_free(&error);
    }

    if (ret!= DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "Not primary owner : %d \n", ret);
        exit (1);
    }
}

void sendSignal(char *signalParam) {
    DBusMessage *message;
    DBusMessageIter args;
    DBusError error;
    dbus_uint32_t serial = 0;

    dbus_error_init(&error);

    message = dbus_message_new_signal(DBUS_SIGNAL_PATH, DBUS_BUS_INTERFACE, DBUS_SIGNAL_NAME);
    if (message == NULL) {
        fprintf(stderr, "Message is NULL.\n");
        exit(1);
    }

    if (signalParam != NULL) {
        dbus_message_iter_init_append(message, &args);

        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &signalParam)) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
    }

    if (!dbus_connection_send(g_connection, message, &serial)) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    dbus_connection_flush(g_connection);
    printf("Signal has been sent.\n");
}

int main(int argc, char **argv) {
    initConnection();
    requestBusName();
    sendSignal(argv[1]);

    return 0;
}
