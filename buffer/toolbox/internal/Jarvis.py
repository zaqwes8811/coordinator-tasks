#-*- coding:utf-8 -*-
import os

# будут собраны пакеты, поэтому не нужно будет добавлять пути
import UdpScaner.UdpServer as us	# изменится
#

import udp_conf as udp_cfg


''' '''
if __name__ == '__main__':
	#os.spawnl(os.P_NOWAIT,"BUM3U_2\Release\TransmitersRemoteControl_FM.exe","DUMMY")
	os.chdir( os.getcwd()+"/5_286_051/bin/" )
	#os.startfile("bat.bat")
	
	# стартуем udp сервер
	us.run( udp_cfg.printTx, udp_cfg.printRx)
