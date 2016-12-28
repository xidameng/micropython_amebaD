import usocket as socket
import uos as os
import ubinascii as binascii
import logging
import uerrno
import utime

class FTPD:
    def __init__(self, sock, remote_addr):
        self.cmd_sock = sock
        self.cmd_sock_addr = remote_addr[0]
        self.cmd_sock_port = remote_addr[1]
        self.data_client_sock = None
        self.data_server_sock = None
        self.pasv_mode = False
        self.mode = 'I'

    def execute(self, cmd, payload):
        try:
            func = getattr(self, cmd.upper())
            logging.info('execute {}, payload: {}'.format(cmd.upper(), payload))
            func(payload)
            logging.info('execute {} done'.format(cmd.upper()))
        except Exception as e:
            print("ERROR: %s" % e)
            self.cmd_sock.send('500 Unsupport CMD.\r\n')

    def USER(self, payload):
        self.cmd_sock.send('331 OK.\r\n')

    def PASS(self, payload):
        self.cmd_sock.send('230 OK.\r\n')

    def MKD(self, payload):
        try:
            logging.info('MKD mkdir %s' % payload)
            os.mkdir(payload)
            self.cmd_sock.send('257 Directory created.\r\n')
        except:
            self.cmd_sock.send('550 Directory create failed.\r\n')

    def RMD(self, payload):
        try:
            logging.info('RMD rmdir %s' % payload)
            os.rmdir(payload)
            self.cmd_sock.send('250 Directory removed.\r\n')
        except:
            self.cmd_sock.send('550 Directory remove failed.\r\n')

    def DELE(self, payload):
        try:
            logging.info('DELE delete %s' % payload)
            os.remove(payload)
            self.cmd_sock.send('250 File removed.\r\n')
        except:
            self.cmd_sock.send('550 File delete failed\r\n')

    def QUIT(self, payload):
        self.cmd_sock.send('221 Goodbye.\r\n')
        self.cmd_sock.close()
        if self.data_client_sock is not None:
            self.data_client_sock.close()
            del self.data_client_sock
        if self.data_server_sock is not None:
            self.data_server_sock.close()
            del self.data_server_sock

    def TYPE(self, payload):
        if len(payload) == 1 and type(payload) == str:
            self.mode = payload
            self.cmd_sock.send('200 Transfer mode set.\r\n')
        else:
            self.cmd_sock.send('500 Syntax error.\r\n')

    def SYST(self, payload):
        self.cmd_sock.send('215 PySniffer WyLink.\r\n')

    def PWD(self, payload):
        pwd = os.getcwd()
        self.cmd_sock.send('257 "%s"\r\n' % pwd)

    def CWD(self, payload):
        try:
            os.chdir(payload)
            self.cmd_sock.send('250 Directory changed success.\r\n')
        except OSError as e:
            logging.info('CWD failed, cause : %s' % str(e))
            self.cmd_sock.send('550 Failed to change directory.\r\n')

    def PORT(self, payload):
        logging.info('PORT command recv')
        if self.pasv_mode:
            logging.info('current is passive mode, close the data server socket')
            self.data_server_sock.close()
            self.pasv_mode = False
        remote_addr = payload.split(',')
        self.data_addr = '.'.join(remote_addr[:4])
        self.data_port = (int(remote_addr[4]) << 8) + int(remote_addr[5])
        logging.info('PORT command, data addr = {} port = {}'.format(self.data_addr, self.data_port))
        self.cmd_sock.send('200 Get port.\r\n')

    def PASV(self, payload):
        lip = '192.168.0.151'
        lport = 8080
        self.pasv_mode = True
        if self.data_server_sock is None:
            self.data_server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.data_server_sock.bind(socket.getaddrinfo('0.0.0.0',lport)[0][-1])
            self.data_server_sock.listen(1)
        logging.info("listen on ip 0.0.0.0 port %d" % lport)
        self.cmd_sock.send('227 Entering Passive Mode ({}, {}, {}).\r\n'.format(','.join(lip.split('.')), lport >> 8 & 0xFF, lport & 0xFF))

    def LIST(self, payload):
        if payload is None:
            wd = os.getcwd()
        else:
            wd = payload
        flist = os.listdir(wd)
        self.cmd_sock.send('150 Here comes, the %s directory listing.\r\n' % wd)
        self.start_data_client_sock()
        for i in flist:
            try:
                state = self.convert_file_state(i)
                self.data_client_sock.send(state)
                utime.sleep_ms(3)
            except:
                pass
        self.stop_data_client_sock()
        self.cmd_sock.send('226 Directory send OK.\r\n')

    def RETR(self, payload):
        try:
            if self.mode == 'I':
                f = open(payload, 'rb')
            else:
                f = open(payload, 'r')
            self.cmd_sock.send('150 opening data connection.\r\n')
            self.start_data_client_sock()
            while True:
                data = f.read(512)
                self.data_client_sock.send(data)
                if len(data) == 0:
                    break
            f.close()
            self.stop_data_client_sock()
            self.cmd_sock.send('226 Transfer complete.\r\n')
        except OSError as e:
            if e == uerrno.ENOENT:
                logging.info('open file %s failed' % payload)
                self.cmd_sock.send('550 Retrive file failed\r\n')

    def STOR(self, payload):
        try:
            if self.mode == 'I':
                f = open(payload, 'wb')
            else:
                f = open(payload, 'w')
            self.cmd_sock.send('150 opening data connection.\r\n')
            self.start_data_client_sock()
            while True:
                data = self.data_client_sock.recv(512)
                if len(data) == 0:
                    break
                f.write(data)
            f.close()
            self.stop_data_client_sock()
            self.cmd_sock.send('226 Transfer complete.\r\n')
        except OSError as e:
            logging.info('open file %s failed cause: %s' % (payload, str(e)))
            self.cmd_sock.send('550 Store file failed\r\n')

    def start_data_client_sock(self):
        if self.pasv_mode:
            self.data_client_sock, data_addr = self.data_server_sock.accept()
            logging.info('Passive mode connection from {} {}'.format(data_addr[0], data_addr[1]))
        else:
            self.data_client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            logging.info("Active mode connect to {} {}".format(self.data_addr, self.data_port))
            self.data_client_sock.connect((self.data_addr, self.data_port))
            logging.info("connection establish")

    def stop_data_client_sock(self):
        logging.info('closing data client sock')
        self.data_client_sock.close()
        del self.data_client_sock
        #if self.pasv_mode:
            #logging.info('closing data server sock')
            #self.data_server_sock.close()

    def convert_file_state(self, file_path):
        try:
            fsize = os.stat(file_path)[6]
            ftype = 'd' if fsize == 0 else '-'
            return "{}rw-rw-rw-  1 user group {} Jan 1 1980 {}\r\n".format(ftype, fsize, file_path)
        except:
            logging.info('conver %s to file state failed' % file_path)
            raise OSError('convert file failed')

def _process_cmd(cmd_sock):
    try:
        msg = cmd_sock.recv(256).decode('utf-8')
        parsed_msg = msg.split(' ')
        if len(parsed_msg) == 1:
            cmd = parsed_msg[0].strip()
            payload = None
        elif len(parsed_msg) == 2:
            cmd = parsed_msg[0].strip()
            payload = parsed_msg[1].strip()
        else:
            pass
        logging.info('Incomming command is {}, payload is {}'.format(cmd, payload))
        ftpd.execute(cmd, payload)
    except OSError as e:
        logging.info('process command error, cause: %s' % str(e))
        cmd_sock.setsockopt(socket.SOL_SOCKET, socket.SO_CALLBACK, None)
        cmd_sock.close()
        del cmd_sock

def _accept_conn(ftpd_sock):
    _cmd_sock, remote_addr = ftpd_sock.accept()
    logging.info("New FTP client connection incomming {}:{}".format(remote_addr[0], remote_addr[1]))
    _cmd_sock.setblocking(False)
    _cmd_sock.setsockopt(socket.SOL_SOCKET, socket.SO_CALLBACK, _process_cmd)
    try:
        _cmd_sock.send("220 Welcome pySniffer FTP server.\r\n")
    except OSError as e:
        logging.info('Send welcome message failed, res: {}'.format(e))
        _cmd.sock.close()
        logging.info('Closing FTP client command socket')
    global ftpd
    ftpd = FTPD(_cmd_sock, remote_addr)

def active(port):
    _ftpd_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    _ftpd_sock.bind(socket.getaddrinfo("0.0.0.0", port)[0][-1])
    _ftpd_sock.listen(1)
    _ftpd_sock.setsockopt(socket.SOL_SOCKET, socket.SO_CALLBACK, _accept_conn)
