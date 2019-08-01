# Fails:
# https://stackoverflow.com/questions/5686490/detect-socket-hangup-without-sending-or-receiving/8434845
# https://pymotw.com/2/select/

# selectors_echo_client.py
import selectors
import socket

import select
import socket
import sys
import errno
import os

import linuxfd,signal,select,time
from queue import Queue

# mysel = selectors.PollSelector()
# keep_running = True
# outgoing = [
#     b'It will be repeated.',
#     b'This is the message.  ',
# ]
# bytes_sent = 0
# bytes_received = 0

# Connecting is a blocking operation, so call setblocking()
# after it returns.



# # Sockets from which we expect to read
# inputs = [ sock ]

# # Sockets to which we expect to write
# outputs = [ sock ]

# Commonly used flag setes
READ_ONLY = select.POLLIN | select.POLLPRI | select.POLLHUP | select.POLLERR
READ_WRITE = READ_ONLY | select.POLLOUT

# Set up the poller
poller = select.poll()

server_address = ('localhost', 10001)
print('connecting to {} port {}'.format(*server_address))
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setblocking(False)
poller.register(sock, READ_WRITE)
# poller.register(sock, READ_ONLY) # сперва только чтение

# Timer
# https://stackoverflow.com/questions/10201326/timer-fd-python-is-there-an-equivalent

# print(errno.errorcode[err], os.strerror(err))

# https://docs.python.org/2/library/select.html
# https://abelbeck.wordpress.com/2014/01/24/more-on-linuxfd-an-example/
tfd = linuxfd.timerfd(rtc=True,nonBlocking=True)
tfd.settime(3,3)
poller.register(tfd, select.POLLIN)

# Map file descriptors to socket objects
fd_to_socket = {}
fd_to_socket[sock.fileno()] = sock
fd_to_socket[tfd.fileno()] = tfd

# sock.connect(server_address)
err = sock.connect_ex(server_address)

sended = 0
while True:
    events = poller.poll(2000)  
    print(fd_to_socket)
    for fd, flag in events:
        if fd == sock.fileno():
            # Retrieve the actual socket from its file descriptor
            s = fd_to_socket[fd]
            if flag & select.POLLERR or flag & select.POLLHUP:
                # На случай не возможности подключиться проверяем эти флаги
                poller.unregister(s)
                s.close()
                del fd_to_socket[fd]
            elif flag & select.POLLOUT:
                print("Wr connected:", bin(flag))
                if sended == 0:
                    s.send(b"To server")
                    sended = 1
                poller.modify(s, READ_ONLY)
            elif flag & select.POLLIN:
                print("Rd connected")
                data = s.recv(1024)
                if not data:
                    print("Disconnect")
                    poller.modify(s, 0)
                    # s.shutdown(socket.SHUT_RDWR)  # Throw exception
                    poller.unregister(s)
                    s.close()
                    del fd_to_socket[fd]
                    # fd_to_socket.pop(s, None)
                else:
                    print(data)
                    poller.modify(s, READ_ONLY)


        elif fd == tfd.fileno():
            if flag & select.POLLIN:
                print("{0:.3f}: timer has expired".format(0))
                tfd.read()

