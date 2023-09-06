#! /usr/bin/python3

import socket
import sys

paths = ['/tmp/sen0251/temperature.sock', '/tmp/sen0251/pressure.sock', '/tmp/sen0251/altitude.sock']

for path in paths:
    client = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
    try:
        client.connect(path)
    except socket.error as e:
        print(e)
        sys.exit(1)
    try:
        print(client.recv(1024))
    finally:
        client.close()
