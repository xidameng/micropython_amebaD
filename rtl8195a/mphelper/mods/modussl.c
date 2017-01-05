/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Chester Tseng
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*****************************************************************************
 *                              Header includes
 * ***************************************************************************/
// micropython headers
#include "py/mpstate.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/stream.h"

#include "polarssl/net.h"
#include "polarssl/ssl.h"
#include "polarssl/error.h"
#include "polarssl/memory.h"

STATIC const mp_obj_type_t ussl_socket_type;

typedef struct _mp_obj_ussl_socket_t {
    mp_obj_base_t base;
    mp_obj_t sock;
    ssl_context *ssl;
    byte *buf;
    uint32_t bytes_left;
} mp_obj_ussl_socket_t;

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/

int ussl_read_wrapper(mp_obj_t *ctx, unsigned char *buf, size_t len) {
    int16_t errcode = 0;
    mp_uint_t ret = 0;
    mp_obj_t obj = *ctx;
    mp_obj_type_t *type = mp_obj_get_type(obj);
    mp_stream_p_t *stream = type->protocol;
    ret = stream->read(obj, buf, len, &errcode);
    return ret;
}

int ussl_write_wrapper(mp_obj_t *ctx, const unsigned char *buf, size_t len) {
    int16_t errcode = 0;
    mp_uint_t ret = 0;
    mp_obj_t obj = *ctx;
    mp_obj_type_t *type = mp_obj_get_type(obj);
    mp_stream_p_t *stream = type->protocol;
    ret = stream->write(obj, buf, len, &errcode);
    return ret;
}

static unsigned int arc4random(void) {
    unsigned int res = xTaskGetTickCount();
    static unsigned int seed = 0xDEADB00B;
    seed = ((seed & 0x007F00FF) << 7) ^
        ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
        (res << 13) ^ (res >> 9);    // using the clock too!
    return seed;
}

static void get_random_bytes(void *buf, size_t len) {
    unsigned int ranbuf;
    unsigned int *lp;
    int i, count;
    count = len / sizeof(unsigned int);
    lp = (unsigned int *) buf;

    for(i = 0; i < count; i ++) {
        lp[i] = arc4random();
        len -= sizeof(unsigned int);
    }

    if(len > 0) { 
        ranbuf = arc4random();
        memcpy(&lp[i], &ranbuf, len);
    }
}

static int my_random(void *p_rng, unsigned char *output, size_t output_len) {
    get_random_bytes(output, output_len);
    return 0;
}

STATIC void ussl_socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    mp_obj_ussl_socket_t *self = self_in;
    mp_printf(print, "<_SSLSocket>");
}

STATIC mp_obj_t ussl_socket_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_socket, ARG_mode };
    STATIC const mp_arg_t ussl_init_args[] = {
        { MP_QSTR_socket, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_mode,   MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = SSL_IS_CLIENT} },
    };
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(ussl_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), ussl_init_args, args);

    mp_obj_ussl_socket_t *o = m_new_obj(mp_obj_ussl_socket_t);
    o->base.type = &ussl_socket_type;
    o->sock = args[ARG_socket].u_obj;
    o->ssl = m_new_obj(ssl_context);
    memory_set_own(malloc, free);
    if (ssl_init(o->ssl) != 0)
        mp_raise_msg(&mp_type_OSError, "ssl context init error");
    ssl_set_endpoint(o->ssl, args[ARG_mode].u_int);
    ssl_set_authmode(o->ssl, SSL_VERIFY_NONE);
    ssl_set_rng(o->ssl, my_random, NULL);
    ssl_set_bio(o->ssl, ussl_read_wrapper, &o->sock, ussl_write_wrapper, &o->sock);
    int32_t ret = 0;
    if ((ret = ssl_handshake(o->ssl)) != 0) 
        mp_raise_msg(&mp_type_OSError, "SSL handshake error");
    return o;
}

STATIC mp_obj_t socket_close(mp_obj_t self_in) {
    mp_obj_ussl_socket_t *self = self_in;
    ssl_free(self->ssl);
    m_del_obj(ssl_context, self->ssl);
    return mp_stream_close(self->sock);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(socket_close_obj, socket_close);

STATIC const mp_rom_map_elem_t ussl_socket_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_read),     MP_OBJ_FROM_PTR(&mp_stream_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_readinto), MP_OBJ_FROM_PTR(&mp_stream_readinto_obj) },
    { MP_ROM_QSTR(MP_QSTR_readline), MP_OBJ_FROM_PTR(&mp_stream_unbuffered_readline_obj) },
    { MP_ROM_QSTR(MP_QSTR_write),    MP_OBJ_FROM_PTR(&mp_stream_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_close),    MP_OBJ_FROM_PTR(&socket_close_obj) },
};
STATIC MP_DEFINE_CONST_DICT(ussl_socket_locals_dict, ussl_socket_locals_dict_table);

STATIC mp_uint_t ussl_socket_read(mp_obj_t self_in, void *buf, mp_uint_t size, int *errcode) {
    mp_obj_ussl_socket_t *o = self_in;
    mp_int_t r = ssl_read(o->ssl, buf, size);
    if (r < 0) {
        if (r == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY) 
            return 0;
        *errcode = r;
        return MP_STREAM_ERROR;
    }
    return r;
}

STATIC mp_uint_t ussl_socket_write(mp_obj_t self_in, const void *buf, mp_uint_t size, int *errcode) {
    mp_obj_ussl_socket_t *o = self_in;
    mp_int_t r = ssl_write(o->ssl, buf, size);
    if (r < 0) {
        if (r == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY) 
            return 0;
        *errcode = r;
        return MP_STREAM_ERROR;
    }
    return r;
}

STATIC const mp_stream_p_t ussl_socket_stream_p = {
    .read = ussl_socket_read,
    .write = ussl_socket_write,
    .is_text = false,
};

STATIC const mp_obj_type_t ussl_socket_type = {
    { &mp_type_type },
    .name        = MP_QSTR_ussl,
    .print       = ussl_socket_print,
    .make_new    = ussl_socket_make_new,
    .getiter     = NULL,
    .iternext    = NULL,
    .protocol    = &ussl_socket_stream_p,
    .locals_dict = (mp_obj_dict_t *)&ussl_socket_locals_dict,
};

STATIC const mp_map_elem_t ssl_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_ussl) },
    { MP_ROM_QSTR(MP_QSTR_wrap_socket),  MP_OBJ_FROM_PTR(&ussl_socket_type) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_CLIENT),   MP_OBJ_NEW_SMALL_INT(SSL_IS_CLIENT) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SERVER),   MP_OBJ_NEW_SMALL_INT(SSL_IS_SERVER) },
};
STATIC MP_DEFINE_CONST_DICT(ssl_module_globals, ssl_module_globals_table);

const mp_obj_module_t mp_module_ussl = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ssl_module_globals,
};
