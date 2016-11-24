/*
TlsTcpClient library for Particle Photon
This software is released under the MIT License.

Copyright (c) 2016 Hirotaka Niisato
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MBED_TLS_TCP_CLIENT_h
#define MBED_TLS_TCP_CLIENT_h

#include "application.h"
#include "check_config.h"

#include "net.h"
#include "debug.h"
#include "ssl.h"
#include "entropy.h"
#include "ctr_drbg.h"
#include "error.h"
#include "timing.h"
#include "ssl_internal.h"
#include "timer_hal.h"

// for debugging.
// #define DEBUG_TLS       1
#if defined(MBEDTLS_DEBUG_C)
#define DEBUG_TLS_CORE_LEVEL 1
#define debug_tls( fmt, ... ) \
    Serial.printf(fmt, ##__VA_ARGS__)
#else /* !DEBUG_TLS */
  #define debug_tls( fmt, ... ) ((void)0)
#endif /* DEBUG_TLS */


class TlsTcpClient {

private:
    mbedtls_entropy_context entropy;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_timing_delay_context timer;

    TCPClient client;
    bool connected;

    static int send_Tls(void *ctx, const unsigned char *buf, size_t len);
    static int recv_Tls(void *ctx, unsigned char *buf, size_t len);
    static int rng_Tls(void* handle, uint8_t* data, const size_t len_);
    static void debug_Tls( void *ctx, int level,
                          const char *file, int line,
                          const char *str );
    int handShake();

public:
    TlsTcpClient();
    ~TlsTcpClient(){ close(); };
    void close();

    int init(const char *rootCaPem, const size_t rootCaPemSize);
    int connect(uint8_t *ip, uint16_t port);
    int connect(char* domain, uint16_t port);
    int write(unsigned char *buff, int length);
    int read(unsigned char *buff, int length);
    void stop() {close();};

    int available();
    bool isConnected();
    bool verify();
};

#endif
