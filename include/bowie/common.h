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

#ifndef BOWIE_COMMON_H
#define BOWIE_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ---- Version -------------------------------------------------------- */

#define BOWIE_VERSION_MAJOR 2
#define BOWIE_VERSION_MINOR 0
#define BOWIE_VERSION_PATCH 0
#define BOWIE_VERSION_STRING "2.0.0"
#define BOWIE_VERSION_CODE \
    ((BOWIE_VERSION_MAJOR << 16) | \
     (BOWIE_VERSION_MINOR << 8)  | \
     (BOWIE_VERSION_PATCH))

/* ---- Utility macros ------------------------------------------------- */

#define BOWIE_MIN(a, b)         ((a) < (b) ? (a) : (b))
#define BOWIE_MAX(a, b)         ((a) > (b) ? (a) : (b))
#define BOWIE_CLAMP(x, lo, hi)  BOWIE_MIN(BOWIE_MAX((x), (lo)), (hi))
#define BOWIE_ARRAY_SIZE(a)     (sizeof(a) / sizeof((a)[0]))
#define BOWIE_ALIGN_UP(x, a)    (((x) + ((a) - 1)) & ~((a) - 1))
#define BOWIE_CONTAINER_OF(p, t, m) \
    ((t *)((char *)(p) - offsetof(t, m)))

/* ---- Static assert (C11) -------------------------------------------- */

#define BOWIE_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

/* ---- Compiler attributes -------------------------------------------- */

#if defined(__GNUC__) || defined(__clang__)
    #define BOWIE_ATTR_FORMAT(fmt, args) __attribute__((format(printf, fmt, args)))
    #define BOWIE_ATTR_NORETURN          __attribute__((noreturn))
    #define BOWIE_ATTR_MALLOC            __attribute__((malloc))
    #define BOWIE_ATTR_WARN_UNUSED       __attribute__((warn_unused_result))
    #define BOWIE_LIKELY(x)              __builtin_expect(!!(x), 1)
    #define BOWIE_UNLIKELY(x)            __builtin_expect(!!(x), 0)
#else
    #define BOWIE_ATTR_FORMAT(fmt, args)
    #define BOWIE_ATTR_NORETURN
    #define BOWIE_ATTR_MALLOC
    #define BOWIE_ATTR_WARN_UNUSED
    #define BOWIE_LIKELY(x)              (x)
    #define BOWIE_UNLIKELY(x)            (x)
#endif

#endif /* BOWIE_COMMON_H */
