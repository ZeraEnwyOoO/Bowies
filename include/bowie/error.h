/*
 * Bowie — P2P Internet Sharing Tool (Repo: Bowie)
 * Copyright (C) 2024 ASBM Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOWIE_ERROR_H
#define BOWIE_ERROR_H

#include "bowie/common.h"

#include <stdarg.h>

/* ---- Error codes ---------------------------------------------------- */

typedef enum {
    /* Success */
    BOWIE_SUCCESS               = 0,

    /* Generic (-1 .. -19) */
    BOWIE_ERR_UNKNOWN           = -1,
    BOWIE_ERR_NOT_FOUND         = -2,
    BOWIE_ERR_EXISTS            = -3,
    BOWIE_ERR_IO                = -4,
    BOWIE_ERR_TIMEOUT           = -5,
    BOWIE_ERR_AGAIN             = -6,
    BOWIE_ERR_PERMISSION        = -7,
    BOWIE_ERR_RANGE             = -8,
    BOWIE_ERR_UNSUPPORTED       = -9,
    BOWIE_ERR_CANCELED          = -10,
    BOWIE_ERR_BUSY              = -11,

    /* Argument (-20 .. -29) */
    BOWIE_ERR_INVALID_ARG       = -20,
    BOWIE_ERR_INVALID_STATE     = -21,
    BOWIE_ERR_NULL_PTR          = -22,
    BOWIE_ERR_OUT_OF_RANGE      = -23,
    BOWIE_ERR_BUFFER_TOO_SMALL  = -24,

    /* Memory (-30 .. -39) */
    BOWIE_ERR_NOMEM             = -30,
    BOWIE_ERR_OVERFLOW          = -31,

    /* Network (-40 .. -59) */
    BOWIE_ERR_NET               = -40,
    BOWIE_ERR_NET_SOCKET        = -41,
    BOWIE_ERR_NET_BIND          = -42,
    BOWIE_ERR_NET_LISTEN        = -43,
    BOWIE_ERR_NET_ACCEPT        = -44,
    BOWIE_ERR_NET_CONNECT       = -45,
    BOWIE_ERR_NET_SEND          = -46,
    BOWIE_ERR_NET_RECV          = -47,
    BOWIE_ERR_NET_CLOSE         = -48,
    BOWIE_ERR_NET_RESOLVE       = -49,
    BOWIE_ERR_NET_UNREACHABLE   = -50,
    BOWIE_ERR_NET_HOST_UNREACH  = -51,
    BOWIE_ERR_NET_CONN_REFUSED  = -52,
    BOWIE_ERR_NET_CONN_RESET    = -53,
    BOWIE_ERR_NET_ADDR_IN_USE   = -54,
    BOWIE_ERR_NET_ADDR_NOT_AVAIL= -55,
    BOWIE_ERR_NET_WOULD_BLOCK   = -56,

    /* Crypto (-60 .. -79) */
    BOWIE_ERR_CRYPTO            = -60,
    BOWIE_ERR_CRYPTO_INIT       = -61,
    BOWIE_ERR_CRYPTO_KEY        = -62,
    BOWIE_ERR_CRYPTO_ENCRYPT    = -63,
    BOWIE_ERR_CRYPTO_DECRYPT    = -64,
    BOWIE_ERR_CRYPTO_HASH       = -65,
    BOWIE_ERR_CRYPTO_SIGN       = -66,
    BOWIE_ERR_CRYPTO_VERIFY     = -67,
    BOWIE_ERR_CRYPTO_RANDOM     = -68,

    /* DHT (-400 .. -499) */
    BOWIE_ERR_DHT               = -400,
    BOWIE_ERR_DHT_INIT          = -401,
    BOWIE_ERR_DHT_BUCKET        = -402,
    BOWIE_ERR_DHT_NODE          = -403,
    BOWIE_ERR_DHT_SEARCH        = -404,
    BOWIE_ERR_DHT_STORAGE       = -405,
    BOWIE_ERR_DHT_TOKEN         = -406,
    BOWIE_ERR_DHT_PARSE         = -407,
    BOWIE_ERR_DHT_SERIALIZE     = -408,
    BOWIE_ERR_DHT_TID           = -409,
    BOWIE_ERR_DHT_NO_PEERS      = -410,

    /* Tunnel (-500 .. -599) */
    BOWIE_ERR_TUNNEL            = -500,
    BOWIE_ERR_TUNNEL_INIT       = -501,
    BOWIE_ERR_TUNNEL_OPEN       = -502,
    BOWIE_ERR_TUNNEL_READ       = -503,
    BOWIE_ERR_TUNNEL_WRITE      = -504,
    BOWIE_ERR_TUNNEL_PACKET     = -505,
    BOWIE_ERR_TUNNEL_MTU        = -506,

    /* Protocol (-800 .. -899) */
    BOWIE_ERR_PROTO             = -800,
    BOWIE_ERR_PROTO_VERSION     = -801,
    BOWIE_ERR_PROTO_HANDSHAKE   = -802,
    BOWIE_ERR_PROTO_MESSAGE     = -803,
    BOWIE_ERR_PROTO_FRAMING     = -804,

    /* Sentinel */
    BOWIE_ERR_MAX               = -900,
} bowie_error_t;

/* ---- Error context -------------------------------------------------- */

typedef struct {
    bowie_error_t   code;
    char            message[256];
    char            file[128];
    int             line;
    char            func[64];
    int64_t         timestamp_ms;
} bowie_error_context_t;

/* ---- Macros --------------------------------------------------------- */

#define BOWIE_ERROR_SET(ctx, code, ...) \
    bowie_error_set((ctx), (code), __FILE__, __LINE__, __func__, __VA_ARGS__)

/* ---- Functions ------------------------------------------------------ */

/*
 * Translate an error code to a stable, human-readable string.
 * Never returns NULL. Unknown codes map to "unknown error".
 */
const char *bowie_error_str(bowie_error_t code);

/*
 * Populate a caller-owned error context.
 * ctx must be non-NULL. fmt must be non-NULL.
 * Records a monotonic timestamp via bowie_time_monotonic_ms().
 */
void bowie_error_set(bowie_error_context_t *ctx,
                     bowie_error_t code,
                     const char *file,
                     int line,
                     const char *func,
                     const char *fmt,
                     ...) BOWIE_ATTR_FORMAT(6, 7);

/*
 * Zero all fields of ctx. ctx must be non-NULL.
 */
void bowie_error_clear(bowie_error_context_t *ctx);

/*
 * Format ctx into buf as:
 *   "[<code>] <file>:<line> <func>: <message>"
 * Returns the number of bytes that would have been written,
 * excluding the NUL terminator (like snprintf).
 * Returns -1 on invalid arguments.
 */
int bowie_error_format(const bowie_error_context_t *ctx,
                       char *buf, size_t size);

/*
 * Print ctx to stderr. ctx must be non-NULL.
 */
void bowie_error_print(const bowie_error_context_t *ctx);

#endif /* BOWIE_ERROR_H */
