#include "common.h"

extern DBusConnection *g_connection;

int sendMethodCall(void) {
    DBusMessage *message;
    DBusPendingCall *pending;

    message = dbus_message_new_method_call(DBUS_BUS_SERVER_NAME, DBUS_BUS_INTERFACE_PATH, DBUS_BUS_INTERFACE, DBUS_METHOD_NAME);
    if (message == NULL) {
        fprintf(stderr, "Message is NULL \n");
        return -1;
    }

    if (!dbus_connection_send_with_reply(g_connection, message, &pending, -1)) {
        fprintf(stderr, "Out of memory \n");
        return -1;
    }

    if (pending == NULL) {
        fprintf(stderr, "Pending call is NULL \n");
        return -1;
    }
    printf("Method call has been sent.\n");

    dbus_message_unref(message);
    dbus_pending_call_block(pending);

    message = dbus_pending_call_steal_reply(pending);
    if (message == NULL) {
        fprintf(stderr, "Reply message is NULL \n");
        return -1;
    }

    printf("Received reply.\n");

    dbus_pending_call_unref(pending);
    dbus_message_unref(message);
}

int sendSignal(char *signalParam) {
    DBusMessage *message;
    DBusMessageIter args;
    DBusError error;
    dbus_uint32_t serial = 0;

    dbus_error_init(&error);

    message = dbus_message_new_signal(DBUS_BUS_INTERFACE_PATH, DBUS_BUS_INTERFACE, DBUS_SIGNAL_NAME);
    if (message == NULL) {
        fprintf(stderr, "Message is NULL.\n");
        return -1;
    }

    // signal message parameter setting.
    if (signalParam != NULL) {
        dbus_message_iter_init_append(message, &args);

        if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &signalParam)) {
            fprintf(stderr, "Out of memory\n");
            return -1;
        }
    }

    if (!dbus_connection_send(g_connection, message, &serial)) {
        fprintf(stderr, "Out of memory\n");
        return -1;
    }

    dbus_connection_flush(g_connection);
    printf("Signal has been sent.\n");
    return 0;
}

int main(int argc, char **argv) {
    if (initConnection() != 0) {
        return -1;
    }

    if (requestBusName(DBUS_BUS_NAME) != 0) {
        return -1;
    }

    // Send signal
    printf("Send d-bus signal\n");
    if (sendSignal(argv[1]) != 0) {
        return -1;
    }

    printf("=================================================\n");
    sleep(3);

    // Send method call
    printf("Send d-bus method call\n");
    if (sendMethodCall() != 0) {
        return -1;
    }

    return 0;
}
