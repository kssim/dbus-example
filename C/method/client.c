#include "common.h"

extern DBusConnection *g_connection;

int sendMethodCall(void) {
    DBusMessage *message;
    DBusPendingCall *pending;

    message = dbus_message_new_method_call(DBUS_BUS_SERVER_NAME, DBUS_INTERFACE_PATH, DBUS_INTERFACE, DBUS_METHOD_NAME);
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

    printf("Got reply.\n");

    dbus_pending_call_unref(pending);
    dbus_message_unref(message);
}

int main(void) {
    if (initConnection() != 0) {
        return -1;
    }

    if (requestBusName(DBUS_BUS_NAME) != 0) {
        return -1;
    }

    if (sendMethodCall() != 0) {
        return -1;
    }

    return 0;
}
