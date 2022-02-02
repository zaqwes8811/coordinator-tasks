# nvidia-docker build -t ml_neon_gpu:latest .
#
# https://blog.codeship.com/the-basics-of-the-docker-run-command/
# sudo nvidia-docker run -v /mnt:/mnt -p 8888:8888 -p 6006:6006 --name ml_neon_gpu_0 --tmpfs /tmp ml_neon_gpu_1:latest 
# sudo nvidia-docker run -v /mnt:/mnt -p 8888:8888 -p 6006:6006 --name ml_neon_gpu_2 --tmpfs /tmp ml_neon_gpu:latest 
# fixme: from tf
#FROM nvidia/cuda:7.5-devel

# Let's from TF
# https://neon.nervanasys.com/index.html/installation.html - есть докеры
FROM tensorflow/tensorflow:latest-gpu

# похоже нужно было прокинуть порты
# https://stackoverflow.com/questions/33636925/how-do-i-start-tensorflow-docker-jupyter-notebook

# "TERM environment variable not set." ???

# boost could not be installed
RUN apt-get update && apt-get install -y \
    git-core wget \
	libjemalloc-dev \
	libtbb-dev \
	python3-setuptools \
	python3-dev \
 	&& rm -rf /var/lib/apt/lists/* 

#python3-pip \
# python 3
# http://jupyter.org/install
RUN easy_install3 pip
RUN pip3 --no-cache-dir install jupyter

# launch
# here will be jypiter
COPY main.sh /sbin
RUN chmod +x /sbin/main.sh
CMD ["/sbin/main.sh"]

# https://github.com/NervanaSystems/neon
# https://software.intel.com/en-us/articles/how-to-install-the-neon-framework-on-ubuntu
#jupyter notebook list
pip3 install virtualenv
#root@68722feaf1ff:/mnt/neon# git clone https://github.com/NervanaSystems/neon.git^C
#root@68722feaf1ff:/mnt/neon# make python3  
#jupyter notebook --allow-root
#  apt-get install python3.4-venv
# make sysinstall
# https://stackoverflow.com/questions/39950254/permissionerror-errno-13-permission-denied-usr-lib-python3-5-site-package
# pip3 install conda
# https://stackoverflow.com/questions/42548991/conda-is-corrupted-after-pip-install-conda
# System Wide
# https://neon.nervanasys.com/index.html/installation.html
# pip install nervananeon
# FIXME: python 3 troubles
# make sysinstall HAS_GPU=true  # но как это сделать для pip3 ?
# https://emilkirkegaard.dk/en/?p=5770
# pip3 install posix_ipc

# Hmmm...
# https://www.quora.com/What-do-Deep-Learning-researchers-think-about-Nervana-Systems-Neon-framework

# DOCKER?

  Using cached https://files.pythonhosted.org/packages/f0/e6/bff62b62b2e75f695b737695951b7a4c1c6595369268a37868f5c34e1c12/posix_ipc-1.0.0.tar.gz
    Complete output from command python setup.py egg_info:
    Traceback (most recent call last):
      File "<string>", line 1, in <module>
      File "/tmp/pip-install-ebDIe5/posix-ipc/setup.py", line 38, in <module>
        d = prober.probe()
      File "prober.py", line 363, in probe
        d["PAGE_SIZE"] = sniff_page_size()
      File "prober.py", line 169, in sniff_page_size
        page_size = compile_and_run("sniff_page_size.c")
      File "prober.py", line 83, in compile_and_run
        stdout=subprocess.PIPE).communicate()[0]
      File "/usr/lib/python2.7/subprocess.py", line 710, in __init__
        errread, errwrite)
      File "/usr/lib/python2.7/subprocess.py", line 1327, in _execute_child
        raise child_exception
    OSError: [Errno 13] Permission denied
    
    ----------------------------------------
Command "python setup.py egg_info" failed with error code 1 in /tmp/pip-install-ebDIe5/posix-ipc/
make: *** [.venv2/bin/activate] Error 1

