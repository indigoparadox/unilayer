
#ifndef NETWORK_SOCKET
#error "NETWORK_SOCKET not enabled!"
#endif /* !NETWORK_SOCKET */

/* Need to include winsock stuff before windows.h gets pulled in. */
#if defined( _WIN32 ) && defined( USE_NETWORK )
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "Mswsock.lib" )
#pragma comment( lib, "AdvApi32.lib" )
#endif /* _WIN32 && USE_NETWORK */

#define SOCKETN_C
#include "../unilayer.h"

#ifdef _GNU_SOURCE
#include <unistd.h>
#endif /* _GNU_SOURCE */

/* This block is really finicky in terms of order. */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#ifndef PLATFORM_WIN
#include <netdb.h>
#endif /* !PLATFORM_WIN */
#include <sys/types.h>
#ifndef PLATFORM_WIN
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif /* !PLATFORM_WIN */

static void network_cleanup_socket( struct NETWORK_CONNECTION* n ) {
   if( 0 != n->socket ) {
      close( n->socket );
      n->socket = 0;
   }
}

void network_setup() {
#ifdef PLATFORM_WIN
   int result = 0;
   WSADATA wsa_data = { 0 };

   result = WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
   if( 0 != result ) {
      error_printf( "WSAStartup failed with error: %d", result );
   }

cleanup:
#endif /* !PLATFORM_WIN */
   return;
}

void network_shutdown() {
   /* TODO */
}

int8_t network_connect(
   struct NETWORK_CONNECTION* n, const char* server, uint16_t server_sz,
   uint16_t port
) {
   int connect_result;
#ifdef _WIN32
   u_long mode = 1;
#endif /* _WIN32 */
   char service[6];
   struct addrinfo hints = { 0 },
         * result;

   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;

   dio_itoa( service, 6, port, 10 );

   connect_result = getaddrinfo(
      server, service, &hints, &result
   );
   if( 0 > connect_result ) {
      error_printf( "unable to resolve: %s", server );
      goto cleanup;
   }

   n->socket = socket(
      result->ai_family,
      result->ai_socktype,
      result->ai_protocol
   );

   connect_result = connect(
      n->socket,
      result->ai_addr,
      result->ai_addrlen
   );

   if( 0 > connect_result ) {
      error_printf( "connect error: %s", strerror( errno ) );
      network_cleanup_socket( n );
      goto cleanup;
   }

   /* TODO: Check for error. */
#ifdef PLATFORM_WIN
   ioctlsocket( n->socket, FIONBIO, &mode );
#else
   fcntl( n->socket, F_SETFL, O_NONBLOCK );
#endif /* PLATFORM_WIN */

   n->flags |= NETWORK_FLAG_CONNECTED;
   n->port = port;

cleanup:

   freeaddrinfo( result );

   return n->socket;
}

int32_t network_write(
   struct NETWORK_CONNECTION* n, char* buffer, int16_t buffer_sz
) {
   int32_t dest_socket = n->socket,
      sent = -1,
      total_sent = 0;

   if( 0 >= dest_socket ) {
      error_printf( "invalid destination socket" );
      goto cleanup;
   }

   do {
      sent = send(
         dest_socket,
         &(buffer[total_sent]),
         buffer_sz - total_sent,
#ifdef PLATFORM_WIN
         0
#else
         MSG_NOSIGNAL
#endif /* PLATFORM_WIN */
      );
      total_sent += sent;
   } while( total_sent < buffer_sz );

cleanup:
   return sent;
}

int8_t network_accept(
   struct NETWORK_CONNECTION* n_server, struct NETWORK_CONNECTION* n
) {
   unsigned int address_length;
   struct sockaddr_in address;
   #ifdef PLATFORM_WIN
   u_long mode = 1;
   #endif /* PLATFORM_WIN */

   /* Accept and verify the client. */
   address_length = sizeof( address );
   n->socket = accept(
      n_server->socket, (struct sockaddr*)&address,
      &address_length
   );

   /* No connection incoming, this time! */
   if( 0 > n->socket && (EWOULDBLOCK == errno || EAGAIN == errno) ) {
      goto cleanup;
   }

   /* TODO: Check for error. */
#ifdef PLATFORM_WIN
   ioctlsocket( n->socket, FIONBIO, &mode );
#else
   fcntl( n->socket, F_SETFL, O_NONBLOCK );
#endif /* PLATFORM_WIN */

   if( 0 > n->socket ) {
      error_printf(
         "error connecting on %d: %s", n->socket, strerror( errno ) );
      network_stop( n );
      n->socket = 0;
      goto cleanup;
   }

   n->flags |= NETWORK_FLAG_CONNECTED | NETWORK_FLAG_CLIENT;

cleanup:
   return n->socket;
}

int16_t network_listen( struct NETWORK_CONNECTION* n, uint16_t port ) {
   int16_t connect_result = 0;
   struct sockaddr_in address;
#ifdef PLATFORM_WIN
   u_long mode = 1;
#endif /* PLATFORM_WIN */

   if( NETWORK_FLAG_LISTENING == (NETWORK_FLAG_LISTENING & n->flags) ) {
      error_printf( "server already listening!" );
      goto cleanup;
   }

   n->socket = socket( AF_INET, SOCK_STREAM, 0 );
   if( 0 > n->socket ) {
      error_printf( "invalid socket received!" );
      connect_result = -1;
      goto cleanup;
   }

   memory_zero_ptr( &address, sizeof( struct sockaddr_in ) );

   /* TODO: Check for error. */
#ifdef _WIN32
   ioctlsocket( n->socket, FIONBIO, &mode );
#else
   fcntl( n->socket, F_SETFL, O_NONBLOCK );
#endif /* _WIN32 */

   /* Setup and bind the port, first. */
   address.sin_family = AF_INET;
   address.sin_port = htons( port );
   address.sin_addr.s_addr = INADDR_ANY;

   connect_result = bind(
      n->socket, (struct sockaddr*)&address, sizeof( address ) );

   if( 0 > connect_result ) {
      error_printf( "unable to open listening socket" );
      network_cleanup_socket( n );
      goto cleanup;
   }

   /* If we could bind the port, then launch the serving connection. */
   connect_result = listen( n->socket, 5 );
   if( 0 > connect_result ) {
      error_printf( "unable to listen on socket" );
      network_cleanup_socket( n );
      goto cleanup;
   }

   n->flags |= NETWORK_FLAG_LISTENING | NETWORK_FLAG_SERVER;
   debug_printf( 3, "listening for connection on port %d...", port );

cleanup:
   return connect_result;
}

int32_t network_read(
   struct NETWORK_CONNECTION* n, char* buffer, int16_t buffer_sz
) {
	int32_t total_read_count = 0;
   int32_t last_read_count = 0;
   char read_char = '\0';

   while( '\n' != read_char && total_read_count < buffer_sz ) {
      if( NULL == n ) {
         break;
      }
      last_read_count = recv( n->socket, &read_char, 1, 0 );

      if( 0 == last_read_count ) {
         debug_printf( 3, "remote connection (%d) closed", n->socket );
         close( n->socket );
         n->socket = 0;
         goto cleanup;
      }

      if( 0 > last_read_count ) {
         goto cleanup;
      }

      /* No error and something was read, so add it to the string. */
      total_read_count++;
      buffer[total_read_count++] = read_char;
   }

cleanup:
   return total_read_count;
}

void network_stop( struct NETWORK_CONNECTION* n ) {
   network_cleanup_socket( n );
   n->flags &= ~NETWORK_FLAG_LISTENING & ~NETWORK_FLAG_CONNECTED;
}

