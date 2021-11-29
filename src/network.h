
#ifndef NETWORK_H
#define NETWORK_H

#define NETWORK_FLAG_LISTENING   0x02
#define NETWORK_FLAG_CONNECTED   0x04
#define NETWORK_FLAG_SERVER      0x08
#define NETWORK_FLAG_CLIENT      0x10

struct NETWORK_CONNECTION {
   uint16_t flags;
   uint16_t port;
   int32_t socket;
};

void network_setup();
void network_shutdown();
int8_t network_connect(
   struct NETWORK_CONNECTION* n, const char* server, uint16_t server_sz,
   uint16_t port );
int32_t network_write(
   struct NETWORK_CONNECTION* n, char* buffer, int16_t buffer_sz );
int8_t network_accept(
   struct NETWORK_CONNECTION* n_server, struct NETWORK_CONNECTION* n );
int16_t network_listen( struct NETWORK_CONNECTION* n, uint16_t port );
int32_t network_read(
   struct NETWORK_CONNECTION* n, char* buffer, int16_t buffer_sz );
void network_stop( struct NETWORK_CONNECTION* n );

#endif /* !NETWORK_H */

