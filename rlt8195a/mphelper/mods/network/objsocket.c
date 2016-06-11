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

#include "py/obj.h"
#include "netutils.h"

#include "objsocket.h"

STATIC mp_obj_t lwip_socket_close(mp_obj_t self_in) {
    socket_obj_t *self = self_in;
    close(self->sock_fd);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lwip_socket_close_obj, lwip_socket_close);

STATIC mp_obj_t lwip_socket_bind(mp_obj_t self_in, mp_obj_t addr_in) {
    socket_obj_t *self = self_in;
    mp_obj_t *addr;
    struct sockaddr_in addrinfo;
    uint16_t port = 0;
    uint8_t addr_len = 0;
    const int8_t *ip_str;;

    if (addr_in == mp_const_none) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Require ip address and port"));
    }

    memset(&addrinfo, 0x0, sizeof(struct sockaddr_in));

    mp_obj_get_array_fixed_n(addr_in, 2, &addr);

    port = mp_obj_get_int(addr[1]);

    if (port < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "port number error"));
    }

    ip_str = mp_obj_str_get_data(addr[0], &addr_len);

    addrinfo.sin_family = self->domain;
    addrinfo.sin_addr.s_addr = inet_addr(ip_str);
    addrinfo.sin_port = htons(port); 
    
    int8_t ret = lwip_bind(self->sock_fd, &addrinfo, sizeof(struct sockaddr_in));

    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Bind failed"));
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_bind_obj, lwip_socket_bind);

STATIC mp_obj_t lwip_socket_listen(mp_obj_t self_in, mp_obj_t backlog_in) {
    socket_obj_t *self = self_in;
    int8_t backlog = mp_obj_get_int(backlog_in);
    if (backlog < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Backlog should be positive"));
    }
    int8_t ret = lwip_listen(self->sock_fd, backlog);

    if(ret < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Listen failed"));
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_listen_obj, lwip_socket_listen);

STATIC mp_obj_t lwip_socket_accept(mp_obj_t self_in) {
    // TODO
    socket_obj_t *self = self_in;
    socket_obj_t *peer = m_new_obj_with_finaliser(socket_obj_t);
    memcpy(peer, self, sizeof(socket_obj_t));
    int16_t client_fd = -1;
    uint16_t port = 0;
    const int8_t *ip_str;
    struct sockaddr_in cli_addr;
    socklen_t client = sizeof(cli_addr);

    client_fd = lwip_accept(self->sock_fd, (struct sockaddr *) &cli_addr, &client);

    if (client_fd < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Accept failed"));
    }

    peer->sock_fd = client_fd;

    mp_obj_tuple_t *client_tuple = mp_obj_new_tuple(2, NULL);
    client_tuple->items[0] = peer;
    client_tuple->items[1] = netutils_format_inet_addr(ip_str, port, NETUTILS_LITTLE);
    return client_tuple;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lwip_socket_accept_obj, lwip_socket_accept);

STATIC mp_obj_t lwip_socket_connect(mp_obj_t self_in, mp_obj_t addr_in) {
    socket_obj_t *self = self_in;
    mp_obj_t *addr;
    uint16_t port = 0;
    uint8_t addr_len = 0;
    const int8_t *ip_str;

    if (addr_in == mp_const_none) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Require ip address and port"));
    }

    memset(&(self->addrinfo), 0x0, sizeof(struct sockaddr_in));

    mp_obj_get_array_fixed_n(addr_in, 2, &addr);

    port = mp_obj_get_int(addr[1]);

    if (port < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "port number error"));
    }

    ip_str = mp_obj_str_get_data(addr[0], &addr_len);

    self->addrinfo.sin_family = self->domain;
    self->addrinfo.sin_addr.s_addr = inet_addr(ip_str);
    self->addrinfo.sin_port = htons(port); 

    int8_t ret = connect(self->sock_fd, (struct sockaddr_in*)&(self->addrinfo), sizeof(struct sockaddr_in));

    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Connection failed"));
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_connect_obj, lwip_socket_connect);

STATIC mp_obj_t lwip_socket_send(mp_obj_t self_in, mp_obj_t buf_in) {
    socket_obj_t *self = self_in;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);
    int8_t ret = lwip_write(self->sock_fd, bufinfo.buf, bufinfo.len);
    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_arg1(&mp_type_OSError, "send failed"));
    }
    return mp_obj_new_int_from_uint(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_send_obj, lwip_socket_send);

STATIC mp_obj_t lwip_socket_sendto(mp_obj_t self_in, mp_obj_t data_in, mp_obj_t addr_in) {
    socket_obj_t *self = self_in;
    mp_buffer_info_t bufinfo;
    const int8_t *ip_str;;
    struct sockaddr_in peer_addr;
    mp_get_buffer_raise(data_in, &bufinfo, MP_BUFFER_READ);
    mp_uint_t port = netutils_parse_inet_addr(addr_in, ip_str, NETUTILS_LITTLE);

    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr(ip_str);
    peer_addr.sin_port = htons(port);

    mp_int_t ret = lwip_sendto(self->sock_fd, bufinfo.buf, bufinfo.len, 0, (struct sockaddr*)&peer_addr, sizeof(struct sockaddr_in));

    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_arg1(&mp_type_OSError, "sendto failed"));
    }

    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(lwip_socket_sendto_obj, lwip_socket_sendto);

STATIC mp_obj_t lwip_socket_recv(mp_obj_t self_in, mp_obj_t len_in) {
    socket_obj_t *self = self_in;
    mp_int_t len = mp_obj_get_int(len_in);
    uint8_t flags = 0;
    vstr_t vstr;
    vstr_init_len(&vstr, len);

    int8_t ret = lwip_recv(self->sock_fd, (int8_t*)vstr.buf, len, flags);

    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_arg1(&mp_type_OSError, "recv failed"));
    } else if (ret == 0) {
        mp_const_empty_bytes;
    }
    vstr.len = ret;
    vstr.buf[vstr.len] = '\0';
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_recv_obj, lwip_socket_recv);

STATIC mp_obj_t lwip_socket_recvfrom(mp_obj_t self_in, mp_obj_t len_in) {
    // TODO 
    socket_obj_t *self = self_in;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_recvfrom_obj, lwip_socket_recvfrom);

STATIC mp_obj_t lwip_socket_settimeout(mp_obj_t self_in, mp_obj_t timeout_in) {
    socket_obj_t *self = self_in;
    int8_t ret = -1;
    int32_t timeout;
    timeout = mp_obj_get_int(timeout_in);
    ret = lwip_setsockopt(self->sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (ret < 0) {
        nlr_raise(mp_obj_new_exception_arg1(&mp_type_OSError, mpexception_socket_settimeout_failed));
    }
    self->timeout = timeout;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_settimeout_obj, lwip_socket_settimeout);

STATIC mp_obj_t lwip_socket_setblocking(mp_obj_t self_in, mp_obj_t blocking_in) {
    socket_obj_t *self = self_in;
    self->blocking = mp_obj_is_true(blocking_in);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(lwip_socket_setblocking_obj, lwip_socket_setblocking);

STATIC const mp_map_elem_t ip_socket_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___del__),         (mp_obj_t)&lwip_socket_close_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_close),           (mp_obj_t)&lwip_socket_close_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_bind),            (mp_obj_t)&lwip_socket_bind_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_listen),          (mp_obj_t)&lwip_socket_listen_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_accept),          (mp_obj_t)&lwip_socket_accept_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_connect),         (mp_obj_t)&lwip_socket_connect_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_send),            (mp_obj_t)&lwip_socket_send_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recv),            (mp_obj_t)&lwip_socket_recv_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_sendto),          (mp_obj_t)&lwip_socket_sendto_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_recvfrom),        (mp_obj_t)&lwip_socket_recvfrom_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_settimeout),      (mp_obj_t)&lwip_socket_settimeout_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_setblocking),     (mp_obj_t)&lwip_socket_setblocking_obj },

    //constant
    { MP_OBJ_NEW_QSTR(MP_QSTR_PF_INET),         MP_OBJ_NEW_SMALL_INT(PF_INET) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PF_UNSPEC),       MP_OBJ_NEW_SMALL_INT(PF_UNSPEC) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_SOCK_STREAM),     MP_OBJ_NEW_SMALL_INT(SOCK_STREAM) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SOCK_DGRAM),      MP_OBJ_NEW_SMALL_INT(SOCK_DGRAM) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SOCK_RAW),        MP_OBJ_NEW_SMALL_INT(SOCK_RAW) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_IPPROTO_IP),      MP_OBJ_NEW_SMALL_INT(IPPROTO_IP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_IPPROTO_TCP),     MP_OBJ_NEW_SMALL_INT(IPPROTO_TCP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_IPPROTO_UDP),     MP_OBJ_NEW_SMALL_INT(IPPROTO_UDP) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_IPPROTO_UDPLITE), MP_OBJ_NEW_SMALL_INT(IPPROTO_UDPLITE) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_ACCEPTCONN),   MP_OBJ_NEW_SMALL_INT(SO_ACCEPTCONN) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_REUSEADDR),    MP_OBJ_NEW_SMALL_INT(SO_REUSEADDR) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_KEEPALIVE),    MP_OBJ_NEW_SMALL_INT(SO_KEEPALIVE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_BROADCAST),    MP_OBJ_NEW_SMALL_INT(SO_BROADCAST) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_LINGER),       MP_OBJ_NEW_SMALL_INT(SO_LINGER) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_DONTLINGER),   MP_OBJ_NEW_SMALL_INT(SO_DONTLINGER) },

    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_RCVBUF),       MP_OBJ_NEW_SMALL_INT(SO_RCVBUF) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_RCVTIMEO),     MP_OBJ_NEW_SMALL_INT(SO_RCVTIMEO) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_ERROR),        MP_OBJ_NEW_SMALL_INT(SO_ERROR) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_TYPE),         MP_OBJ_NEW_SMALL_INT(SO_TYPE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SO_NO_CHECK),     MP_OBJ_NEW_SMALL_INT(SO_NO_CHECK) },
};
STATIC MP_DEFINE_CONST_DICT(ip_socket_locals_dict, ip_socket_locals_dict_table);

STATIC mp_obj_t ip_socket_make_new(mp_obj_type_t *type, mp_uint_t n_args,
    mp_uint_t n_kw, const mp_obj_t *all_args) {
    STATIC const mp_arg_t socket_init_args[] = {
        { MP_QSTR_domain,    MP_ARG_INT,  {.u_int =  AF_INET} },
        { MP_QSTR_type,      MP_ARG_INT,  {.u_int =  SOCK_STREAM} },
        { MP_QSTR_protocol,  MP_ARG_INT,  {.u_int =  IPPROTO_TCP} },
    };
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, all_args + n_args);
    mp_arg_val_t args[MP_ARRAY_SIZE(socket_init_args)];
    mp_arg_parse_all(n_args, all_args, &kw_args, MP_ARRAY_SIZE(args), socket_init_args, args);

    uint8_t domain = 0;
    uint8_t net_type = 0;
    uint8_t protocol = 0;
    int16_t sock_fd = -1;

    socket_obj_t *self = m_new_obj(socket_obj_t);
    memset(self, 0x0, sizeof(*self));

    self->base.type = &ip_socket_type;

    domain = args[0].u_int;

    if (domain != PF_INET &&
        domain != PF_UNSPEC) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid domain value"));
    } else {
        self->domain = domain;
    }

    net_type = args[1].u_int;

    if (net_type != SOCK_RAW &&
        net_type != SOCK_STREAM && 
        net_type != SOCK_DGRAM) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid type value"));
    } else {
        self->type = net_type;
    }

    protocol = args[2].u_int;

    if (protocol != IPPROTO_IP &&
        protocol != IPPROTO_TCP &&
        protocol != IPPROTO_UDP &&
        protocol != IPPROTO_UDPLITE) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "Invalid protocol value"));
    } else {
        self->protocol = protocol;
    }

    sock_fd = socket(self->domain, self->type, self->protocol);

    if (sock_fd < 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "Create socket failed"));
    } else {
        self->sock_fd = sock_fd;
    }
    return (mp_obj_t)self;
}

STATIC void ip_socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    socket_obj_t *self = self_in;
    mp_printf(print, "socket(fd=%d, timeout=%d)", self->sock_fd, self->timeout);
}

const mp_obj_type_t ip_socket_type = {
    { &mp_type_type },
    .name        = MP_QSTR_socket,
    .print       = ip_socket_print,
    .make_new    = ip_socket_make_new,
    .locals_dict = (mp_obj_t)&ip_socket_locals_dict,
};
