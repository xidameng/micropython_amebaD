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

#include "tcm_heap.h"

STATIC const mp_obj_type_t ussl_socket_type;

typedef struct _mp_obj_ussl_socket_t {
    mp_obj_base_t base;
    mp_obj_t sock;
    ssl_context ssl;
    x509_crt cert;
    pk_context pkey;
} mp_obj_ussl_socket_t;

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/

int ussl_read_wrapper(mp_obj_t *ctx, unsigned char *buf, size_t len) {
    mp_obj_t sock = *(mp_obj_t*)ctx;

    const mp_stream_p_t *sock_stream = mp_get_stream_raise(sock, MP_STREAM_OP_READ);
    int err;

    int out_sz = sock_stream->read(sock, buf, len, &err);
    if (out_sz == MP_STREAM_ERROR) {
        return -err;
    } else {
        return out_sz;
    }
}

int ussl_write_wrapper(mp_obj_t *ctx, const unsigned char *buf, size_t len) {
    mp_obj_t sock = *(mp_obj_t*)ctx;

    const mp_stream_p_t *sock_stream = mp_get_stream_raise(sock, MP_STREAM_OP_WRITE);
    int err;

    int out_sz = sock_stream->write(sock, buf, len, &err);
    if (out_sz == MP_STREAM_ERROR) {
        return -err;
    } else {
        return out_sz;
    }
}

void *ussl_mem_malloc_wrapper(size_t size) {
    return gc_alloc(size, true);
}

void ussl_mem_free_wrapper(void *ptr) {
    gc_free(ptr);
}

static int _random_func(void *p_rng, unsigned char *output, size_t output_len)
{
	rtw_get_random_bytes(output, output_len);
	return 0;
}

STATIC void ussl_socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    mp_obj_ussl_socket_t *self = self_in;
    mp_printf(print, "<_SSLSocket>");
}

STATIC mp_obj_t ussl_socket_make_new(const mp_obj_type_t *type, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_socket, ARG_mode, ARG_key, ARG_cert};
    STATIC const mp_arg_t ussl_init_args[] = {
        { MP_QSTR_socket,  MP_ARG_REQUIRED | MP_ARG_OBJ,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_mode,    MP_ARG_KW_ONLY | MP_ARG_INT,   {.u_int = SSL_IS_CLIENT} },
        { MP_QSTR_key,     MP_ARG_KW_ONLY | MP_ARG_OBJ,   {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_cert,    MP_ARG_KW_ONLY | MP_ARG_OBJ,   {.u_obj = MP_OBJ_NULL} },
    };
    int ret = 0;
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(ussl_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), ussl_init_args, args);

    mp_obj_ussl_socket_t *o = m_new_obj(mp_obj_ussl_socket_t);
    o->base.type = &ussl_socket_type;
    o->sock = args[ARG_socket].u_obj;
    memory_set_own(ussl_mem_malloc_wrapper, ussl_mem_free_wrapper);
    if (ret = ssl_init(&o->ssl) != 0)
        mp_raise_OSError(ret);
    x509_crt_init(&o->cert);
    pk_init(&o->pkey);
    ssl_set_endpoint(&o->ssl, args[ARG_mode].u_int);
    ssl_set_authmode(&o->ssl, SSL_VERIFY_NONE);
    ssl_set_rng(&o->ssl, _random_func, NULL);
    ssl_set_bio(&o->ssl, ussl_read_wrapper, &o->sock, ussl_write_wrapper, &o->sock);

    if (args[ARG_key].u_obj != MP_OBJ_NULL) {
        size_t key_len;
        const byte *key = (const byte*)mp_obj_str_get_data(args[ARG_key].u_obj, &key_len);
        // len should include terminating null
        ret = pk_parse_key(&o->pkey, key, key_len + 1, NULL, 0);
        assert(ret == 0);

        size_t cert_len;
        const byte *cert = (const byte*)mp_obj_str_get_data(args[ARG_cert].u_obj, &cert_len);
        // len should include terminating null
        ret = x509_crt_parse(&o->cert, cert, cert_len + 1);
        assert(ret == 0);

        ret = ssl_set_own_cert(&o->ssl, &o->cert, &o->pkey);
        assert(ret == 0);
    }

    if ((ret = ssl_handshake(&o->ssl)) != 0) 
        mp_raise_OSError(ret);
    return o;
}

STATIC mp_obj_t socket_close(mp_obj_t self_in) {
    mp_obj_ussl_socket_t *self = self_in;
    ssl_free(&self->ssl);
    x509_crt_free(&self->cert);
    pk_free(&self->pkey);
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
    mp_int_t r = ssl_read(&o->ssl, buf, size);
    if (r == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
        return 0;
    if (r >= 0) {
        return r;
    }
    *errcode = r;
    return MP_STREAM_ERROR;
}

STATIC mp_uint_t ussl_socket_write(mp_obj_t self_in, const void *buf, mp_uint_t size, int *errcode) {
    mp_obj_ussl_socket_t *o = self_in;
    mp_int_t r = ssl_write(&o->ssl, buf, size);
    if (r == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
        return 0;
    if (r >= 0) {
        return r;
    }
    *errcode = r;
    return MP_STREAM_ERROR;
}

STATIC const mp_stream_p_t ussl_socket_stream_p = {
    .read = ussl_socket_read,
    .write = ussl_socket_write,
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
