#-*- coding:utf-8 -*-
import os
print os.environ['PY_S_SB']
import sys
sys.path.append( os.environ['PY_S_SB'] )
import Apps.udp_exchange.udp_serv as us

''' '''
if __name__ == '__main__':
	os.spawnl(os.P_NOWAIT,"BUM3U_2\Release\TransmitersRemoteControl_FM.exe","DUMMY")
	os.chdir( os.getcwd()+"/5_286_051/bin/" )
	os.startfile("bat.bat")
	
	# стартуем udp сервер
	us.run()
