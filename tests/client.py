#! /usr/bin/python3

import socket
import sys
from decimal import Decimal, getcontext, ROUND_HALF_UP

getcontext().rounding = ROUND_HALF_UP
def data_interpreter(data):
    if not data:
        raise ValueError("server did not send any data")
    data = data.decode('utf-8')
    if data[-1] == 'C':
        val = Decimal(data[:-1])
        ext = data[-1]
    elif data[-1] == 'm':
        val = Decimal(data[:-1])
        ext = data[-1]
    else:
        val = Decimal(data[:-3])
        ext = data[-3:]

#decimal.InvalidOperation:  TypeError

    return str(val.quantize(Decimal('1.00'))) + ext

paths = ['/tmp/sen0251/temperature.sock', '/tmp/sen0251/pressure.sock', '/tmp/sen0251/altitude.sock']

for path in paths:
    client = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
    try:
        client.connect(path)
    except socket.error as e:
        print(e)
        sys.exit(1)
    try:
        print(data_interpreter(client.recv(1024)))
    finally:
        client.close()
