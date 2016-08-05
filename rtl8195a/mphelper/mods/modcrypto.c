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

#include "hal_crypto.h"

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
void crypto_init0(void) {
    const char *error_str = "Crypto engine init failed\r\n";
    if (rtl_cryptoEngine_init() != 0)
        mp_hal_stdout_tx_strn_cooked(error_str, strlen(error_str));
}

STATIC mp_obj_t crypto_md5(mp_obj_t array_in) {
    mp_buffer_info_t bufinfo;
    uint8_t cache;
    rtl_crypto_md5_init();
    vstr_t vstr;
    pyb_buf_get_for_send(array_in, &bufinfo, &cache);
    pyb_buf_get_for_recv(mp_obj_new_int(CRYPTO_MD5_DIGEST_LENGTH), &vstr);
    rtl_crypto_md5_process(bufinfo.buf, bufinfo.len, vstr.buf);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC  MP_DEFINE_CONST_FUN_OBJ_1(crypto_md5_object, crypto_md5);

STATIC mp_obj_t crypto_sha1(mp_obj_t array_in) {
    mp_buffer_info_t bufinfo;
    uint8_t cache;
    rtl_crypto_sha1_init();
    vstr_t vstr;
    pyb_buf_get_for_send(array_in, &bufinfo, &cache);
    pyb_buf_get_for_recv(mp_obj_new_int(CRYPTO_SHA1_DIGEST_LENGTH), &vstr);
    rtl_crypto_sha1_process(bufinfo.buf, bufinfo.len, vstr.buf);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC  MP_DEFINE_CONST_FUN_OBJ_1(crypto_sha1_object, crypto_sha1);

STATIC mp_obj_t crypto_sha2(mp_obj_t array_in, mp_obj_t type_in) {
    mp_buffer_info_t bufinfo;
    uint8_t cache;
    uint8_t sha2_type;

    sha2_type = mp_obj_get_int(type_in);
    
    if ((sha2_type != SHA2_NONE) &&
        (sha2_type != SHA2_224) &&
        (sha2_type != SHA2_256) &&
        (sha2_type != SHA2_384) &&
        (sha2_type != SHA2_512))
    {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Error SHA2 type"));
    }
    rtl_crypto_sha2_init(sha2_type);
    vstr_t vstr;
    pyb_buf_get_for_send(array_in, &bufinfo, &cache);
    pyb_buf_get_for_recv(mp_obj_new_int(CRYPTO_SHA2_DIGEST_LENGTH), &vstr);
    rtl_crypto_sha2_process(bufinfo.buf, bufinfo.len, vstr.buf);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC  MP_DEFINE_CONST_FUN_OBJ_2(crypto_sha2_object, crypto_sha2);

STATIC mp_obj_t crypto_hmac_md5(mp_obj_t array_in, mp_obj_t key_in) {
    mp_buffer_info_t bufinfo;
    mp_buffer_info_t keyinfo;
    uint8_t cache;
    
    pyb_buf_get_for_send(key_in, &keyinfo, &cache);
    rtl_crypto_hmac_md5_init(keyinfo.buf, keyinfo.len);

    vstr_t vstr;
    pyb_buf_get_for_send(array_in, &bufinfo, &cache);
    pyb_buf_get_for_recv(mp_obj_new_int(CRYPTO_MD5_DIGEST_LENGTH), &vstr);
    rtl_crypto_hmac_md5_process(bufinfo.buf, bufinfo.len, vstr.buf);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC  MP_DEFINE_CONST_FUN_OBJ_2(crypto_hmac_md5_object, crypto_hmac_md5);

STATIC mp_obj_t crypto_hmac_sha1(mp_obj_t array_in, mp_obj_t key_in) {
    mp_buffer_info_t bufinfo;
    mp_buffer_info_t keyinfo;
    uint8_t cache;
    
    pyb_buf_get_for_send(key_in, &keyinfo, &cache);
    rtl_crypto_hmac_sha1_init(keyinfo.buf, keyinfo.len);

    vstr_t vstr;
    pyb_buf_get_for_send(array_in, &bufinfo, &cache);
    pyb_buf_get_for_recv(mp_obj_new_int(CRYPTO_SHA1_DIGEST_LENGTH), &vstr);
    rtl_crypto_hmac_sha1_process(bufinfo.buf, bufinfo.len, vstr.buf);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC  MP_DEFINE_CONST_FUN_OBJ_2(crypto_hmac_sha1_object, crypto_hmac_sha1);

STATIC mp_obj_t crypto_hmac_sha2(mp_obj_t array_in, mp_obj_t key_in, mp_obj_t type_in) {
    mp_buffer_info_t bufinfo;
    mp_buffer_info_t keyinfo;
    uint8_t cache;
    uint8_t sha2_type;

    sha2_type = mp_obj_get_int(type_in);
    
    if ((sha2_type != SHA2_NONE) &&
        (sha2_type != SHA2_224) &&
        (sha2_type != SHA2_256) &&
        (sha2_type != SHA2_384) &&
        (sha2_type != SHA2_512))
    {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Error SHA2 type"));
    }
    
    pyb_buf_get_for_send(key_in, &keyinfo, &cache);
    rtl_crypto_hmac_sha2_init(sha2_type, keyinfo.buf, keyinfo.len);

    vstr_t vstr;
    pyb_buf_get_for_send(array_in, &bufinfo, &cache);
    pyb_buf_get_for_recv(mp_obj_new_int(CRYPTO_MD5_DIGEST_LENGTH), &vstr);
    rtl_crypto_hmac_sha2_process(bufinfo.buf, bufinfo.len, vstr.buf);

    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC  MP_DEFINE_CONST_FUN_OBJ_3(crypto_hmac_sha2_object, crypto_hmac_sha2);

STATIC const mp_map_elem_t crypto_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),        MP_OBJ_NEW_QSTR(MP_QSTR_crypto) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_md5),             (mp_obj_t)&crypto_md5_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_sha1),            (mp_obj_t)&crypto_sha1_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_sha2),            (mp_obj_t)&crypto_sha2_object },

    { MP_OBJ_NEW_QSTR(MP_QSTR_hmac_md5),        (mp_obj_t)&crypto_hmac_md5_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hmac_sha1),       (mp_obj_t)&crypto_hmac_sha1_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hmac_sha2),       (mp_obj_t)&crypto_hmac_sha2_object },
#if 0
    { MP_OBJ_NEW_QSTR(MP_QSTR_aes_cbc),         (mp_obj_t)&crypto_aes_cbc_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_aes_ecb),         (mp_obj_t)&crypto_aes_aes_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_aes_ctr),         (mp_obj_t)&crypto_aes_ctr_object },

    { MP_OBJ_NEW_QSTR(MP_QSTR_3des_cbc),        (mp_obj_t)&crypto_3des_cbc_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_3des_ecb),        (mp_obj_t)&crypto_3des_ecb_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_des_cbc),         (mp_obj_t)&crypto_des_cbc_object },
    { MP_OBJ_NEW_QSTR(MP_QSTR_des_ecb),         (mp_obj_t)&crypto_des_ecb_object },
#endif
    { MP_OBJ_NEW_QSTR(MP_QSTR_SHA2_NONE),       MP_OBJ_NEW_SMALL_INT(SHA2_NONE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SHA2_224),        MP_OBJ_NEW_SMALL_INT(SHA2_224) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SHA2_256),        MP_OBJ_NEW_SMALL_INT(SHA2_256) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SHA2_384),        MP_OBJ_NEW_SMALL_INT(SHA2_384) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SHA2_512),        MP_OBJ_NEW_SMALL_INT(SHA2_512) },
};
STATIC MP_DEFINE_CONST_DICT(crypto_module_globals, crypto_module_globals_table);

const mp_obj_module_t mp_crypto_module = {
    .base    = { &mp_type_module },
    .name    = MP_QSTR_crypto,
    .globals = (mp_obj_dict_t*)&crypto_module_globals,
};
