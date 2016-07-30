#include <unistd.h>
#include <stdbool.h>
#include "common.h"

extern DBusConnection *g_connection;

void listen(void) {
    DBusError error;
    DBusMessage *message;

    dbus_error_init(&error);

    printf("Listening for Method Calls \n");
    if (dbus_error_is_set(&error)) {
        printf("%s \n", error.message);
        dbus_error_free(&error);
    }

    while (1) {
        dbus_connection_read_write(g_connection, 0);
        message = dbus_connection_pop_message(g_connection);

        if (message == NULL) {
            usleep(100);
            continue;
        }

        if (dbus_message_is_method_call(message, DBUS_INTERFACE, DBUS_METHOD_NAME)) {
            printf("Got message\n");
            DBusMessage *reply;
            reply = dbus_message_new_method_return(message);

            dbus_connection_send(g_connection, reply, NULL);
            dbus_connection_flush(g_connection);
            printf("Send request\n");
        }
    }
}

int main(void) {
    if (initConnection() != 0) {
        return -1;
    }

    if (requestBusName(DBUS_BUS_SERVER_NAME) != 0) {
        return -1;
    }

    listen();
    return 0;
}
