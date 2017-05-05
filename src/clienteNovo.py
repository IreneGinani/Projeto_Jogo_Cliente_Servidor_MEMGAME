#!/usr/bin/python
#coding=utf-8
import socket
import os
import sys
import contextlib
import termios
import thread
import time

GPIO115 = "/sys/class/gpio/gpio115/value"
POTEN1 = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
POTEN0 = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"



class read:
    def readPoten1(self):
        global potencia1
        arquivo = open(POTEN1, "r")
        potencia1 = int(arquivo.readline(4))
        arquivo.close()

    def readPoten0(self):
        global potencia0
        arquivo = open(POTEN0, "r")
        potencia0 = int(arquivo.readline(4))
        arquivo.close()

    def readBotao(self):
        global valor
        arquivo = open(GPIO115, "r")
        valor = int(arquivo.readline())
        arquivo.close()

class _Getch:
    """Gets a single character from standard input.  Does not echo to the
screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()

class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch()

getch = _Getch()

def envioDados(tcp,msg):
	tcp.send(msg)

def le(tcp):
    rad =  read()
    rad.readBotao()
    c = ord(getch())
    if c == 27:
        c = ord(getch())
        c = ord(getch())
        rad.readPoten1()
        rad.readPoten0()
        rad.readBotao()
        if potencia1 > 2500:
	    envioDados(tcp,str(1))
        elif potencia0 > 2500:
	    envioDados(tcp,str(3))
        elif potencia1 < 1500:
	    envioDados(tcp,str(2))
        elif potencia0 < 1500:
	    envioDados(tcp,str(4))

    if c == 3:
        sys.exit()
    if valor  == 1:
        envioDados(tcp,str(5))
    else:
        print "%d" % c

def main():

	if len(sys.argv) < 2:
		print 'Uso correto: cliente <servidor>'
		sys.exit()

	HOST = sys.argv[1] # Endereco IP do Servidor
	PORT = 4320       # Porta que o Servidor esta
	
	while(True):

		tcp=socket.socket(socket.AF_INET, socket.SOCK_STREAM,0)
		tcp.connect(("127.0.0.1", PORT))

		try:
			le(tcp)
			#thread.start_new_thread(le, (tcp,))
		except Exception as e:
			print e
			sys.exit()

		asdasd = tcp.recv(1024);
		print asdasd;
		tcp.close();	

if __name__ == '__main__': main()
