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

#include "bowie/error.h"

#include <stdio.h>
#include <string.h>

/* ---- Internal helpers ----------------------------------------------- */

/*
 * Copy src into a fixed-size destination, always NUL-terminating.
 * Truncates silently when src is longer than dst_size - 1.
 * Avoids -Wformat-truncation from snprintf("%s") on GCC.
 */
static void safe_strcpy(char *dst, size_t dst_size, const char *src)
{
    if (dst == NULL || dst_size == 0) {
        return;
    }
    if (src == NULL) {
        dst[0] = '\0';
        return;
    }
    size_t len = strlen(src);
    if (len >= dst_size) {
        len = dst_size - 1;
    }
    memcpy(dst, src, len);
    dst[len] = '\0';
}

/* ---- bowie_error_str ------------------------------------------------ */

const char *bowie_error_str(bowie_error_t code)
{
    switch (code) {
    case BOWIE_SUCCESS:               return "success";

    case BOWIE_ERR_UNKNOWN:           return "unknown error";
    case BOWIE_ERR_NOT_FOUND:         return "not found";
    case BOWIE_ERR_EXISTS:            return "already exists";
    case BOWIE_ERR_IO:                return "I/O error";
    case BOWIE_ERR_TIMEOUT:           return "timed out";
    case BOWIE_ERR_AGAIN:             return "try again";
    case BOWIE_ERR_PERMISSION:        return "permission denied";
    case BOWIE_ERR_RANGE:             return "out of range";
    case BOWIE_ERR_UNSUPPORTED:       return "unsupported";
    case BOWIE_ERR_CANCELED:          return "canceled";
    case BOWIE_ERR_BUSY:              return "busy";

    case BOWIE_ERR_INVALID_ARG:       return "invalid argument";
    case BOWIE_ERR_INVALID_STATE:     return "invalid state";
    case BOWIE_ERR_NULL_PTR:          return "null pointer";
    case BOWIE_ERR_OUT_OF_RANGE:      return "out of range";
    case BOWIE_ERR_BUFFER_TOO_SMALL:  return "buffer too small";

    case BOWIE_ERR_NOMEM:             return "out of memory";
    case BOWIE_ERR_OVERFLOW:          return "integer overflow";

    case BOWIE_ERR_NET:               return "network error";
    case BOWIE_ERR_NET_SOCKET:        return "socket error";
    case BOWIE_ERR_NET_BIND:          return "bind failed";
    case BOWIE_ERR_NET_LISTEN:        return "listen failed";
    case BOWIE_ERR_NET_ACCEPT:        return "accept failed";
    case BOWIE_ERR_NET_CONNECT:       return "connect failed";
    case BOWIE_ERR_NET_SEND:          return "send failed";
    case BOWIE_ERR_NET_RECV:          return "receive failed";
    case BOWIE_ERR_NET_CLOSE:         return "close failed";
    case BOWIE_ERR_NET_RESOLVE:       return "resolve failed";
    case BOWIE_ERR_NET_UNREACHABLE:   return "network unreachable";
    case BOWIE_ERR_NET_HOST_UNREACH:  return "host unreachable";
    case BOWIE_ERR_NET_CONN_REFUSED:  return "connection refused";
    case BOWIE_ERR_NET_CONN_RESET:    return "connection reset";
    case BOWIE_ERR_NET_ADDR_IN_USE:   return "address in use";
    case BOWIE_ERR_NET_ADDR_NOT_AVAIL:return "address not available";
    case BOWIE_ERR_NET_WOULD_BLOCK:   return "would block";

    case BOWIE_ERR_CRYPTO:            return "crypto error";
    case BOWIE_ERR_CRYPTO_INIT:       return "crypto init failed";
    case BOWIE_ERR_CRYPTO_KEY:        return "crypto key error";
    case BOWIE_ERR_CRYPTO_ENCRYPT:    return "encryption failed";
    case BOWIE_ERR_CRYPTO_DECRYPT:    return "decryption failed";
    case BOWIE_ERR_CRYPTO_HASH:       return "hash failed";
    case BOWIE_ERR_CRYPTO_SIGN:       return "sign failed";
    case BOWIE_ERR_CRYPTO_VERIFY:     return "verify failed";
    case BOWIE_ERR_CRYPTO_RANDOM:     return "crypto random failed";

    case BOWIE_ERR_DHT:               return "DHT error";
    case BOWIE_ERR_DHT_INIT:          return "DHT init failed";
    case BOWIE_ERR_DHT_BUCKET:        return "DHT bucket error";
    case BOWIE_ERR_DHT_NODE:          return "DHT node error";
    case BOWIE_ERR_DHT_SEARCH:        return "DHT search error";
    case BOWIE_ERR_DHT_STORAGE:       return "DHT storage error";
    case BOWIE_ERR_DHT_TOKEN:         return "DHT token error";
    case BOWIE_ERR_DHT_PARSE:         return "DHT parse error";
    case BOWIE_ERR_DHT_SERIALIZE:     return "DHT serialize error";
    case BOWIE_ERR_DHT_TID:           return "DHT transaction id error";
    case BOWIE_ERR_DHT_NO_PEERS:      return "DHT no peers";

    case BOWIE_ERR_TUNNEL:            return "tunnel error";
    case BOWIE_ERR_TUNNEL_INIT:       return "tunnel init failed";
    case BOWIE_ERR_TUNNEL_OPEN:       return "tunnel open failed";
    case BOWIE_ERR_TUNNEL_READ:       return "tunnel read failed";
    case BOWIE_ERR_TUNNEL_WRITE:      return "tunnel write failed";
    case BOWIE_ERR_TUNNEL_PACKET:     return "tunnel packet error";
    case BOWIE_ERR_TUNNEL_MTU:        return "tunnel MTU error";

    case BOWIE_ERR_PROTO:             return "protocol error";
    case BOWIE_ERR_PROTO_VERSION:     return "protocol version mismatch";
    case BOWIE_ERR_PROTO_HANDSHAKE:   return "protocol handshake failed";
    case BOWIE_ERR_PROTO_MESSAGE:     return "protocol message error";
    case BOWIE_ERR_PROTO_FRAMING:     return "protocol framing error";

    case BOWIE_ERR_MAX:               return "sentinel";
    default:                          return "unknown error";
    }
}

/* ---- bowie_error_set ------------------------------------------------ */

void bowie_error_set(bowie_error_context_t *ctx,
                     bowie_error_t code,
                     const char *file,
                     int line,
                     const char *func,
                     const char *fmt,
                     ...)
{
    if (ctx == NULL) {
        return;
    }

    ctx->code = code;
    ctx->line = line;
    ctx->timestamp_ms = 0;

    safe_strcpy(ctx->file, sizeof(ctx->file), file);
    safe_strcpy(ctx->func, sizeof(ctx->func), func);

    if (fmt != NULL) {
        va_list args;
        va_start(args, fmt);
        vsnprintf(ctx->message, sizeof(ctx->message), fmt, args);
        va_end(args);
    } else {
        ctx->message[0] = '\0';
    }
}

/* ---- bowie_error_clear ---------------------------------------------- */

void bowie_error_clear(bowie_error_context_t *ctx)
{
    if (ctx == NULL) {
        return;
    }
    memset(ctx, 0, sizeof(*ctx));
}

/* ---- bowie_error_format --------------------------------------------- */

int bowie_error_format(const bowie_error_context_t *ctx,
                       char *buf, size_t size)
{
    if (ctx == NULL || buf == NULL || size == 0) {
        return -1;
    }

    const char *file = (ctx->file[0] != '\0') ? ctx->file : "?";
    const char *func = (ctx->func[0] != '\0') ? ctx->func : "?";
    const char *msg  = (ctx->message[0] != '\0')
                       ? ctx->message
                       : bowie_error_str(ctx->code);

    int n = snprintf(buf, size,
                     "[%d] %s:%d %s: %s",
                     (int)ctx->code,
                     file,
                     ctx->line,
                     func,
                     msg);

    return n;
}

/* ---- bowie_error_print ---------------------------------------------- */

void bowie_error_print(const bowie_error_context_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    char buf[512];
    int n = bowie_error_format(ctx, buf, sizeof(buf));
    if (n < 0) {
        return;
    }

    fprintf(stderr, "%s\n", buf);
}
