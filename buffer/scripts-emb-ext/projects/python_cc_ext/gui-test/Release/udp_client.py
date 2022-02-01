#-*- coding - utf-8 -*-
# file : import udp_client as uc
import socket

def sendMsg(msg):
	UDP_IP="127.0.0.1"
	UDP_PORT=5005
	MESSAGE = msg
 
	sock = socket.socket( socket.AF_INET, socket.SOCK_DGRAM ) # UDP
	sock.sendto( MESSAGE, (UDP_IP, UDP_PORT) )
	
sendMsg('asdf')