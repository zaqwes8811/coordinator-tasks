# coding: utf-8

import yaml

class Configurer(object):
    def __init__(self, file_name):
        with open(file_name, 'r') as f:
            self.doc_ = yaml.load(f)
            
    def get_list(self, path):
        return self.doc_[path]
    
    def get_int(self, path0, path1):
        return self.doc_[path0][path1]
    
    def get_double(self, path0, path1):
        return self.doc_[path0][path1]

    def get_double(self, path0):
        return self.doc_[path0]
        
if __name__=='__main__':
    file_name = '../yaml-conf.txt'
    path = "ips-master-node"
    print Configurer(file_name).get_list(path)
