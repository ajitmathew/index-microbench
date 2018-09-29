'''
Python script to run microbenchmark for different
index structure
'''

import json
import optparse
import os
import subprocess
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

def getThroughput(wl):
    fp = open("tmp").readlines()
    if wl == 'a':
        searchStr = "read/update"
    elif wl == 'c':
        searchStr = "read"
    else:
        searchStr = "insert/scan"

    for line in fp:
        w = line.split()
        if w[0] == searchStr:
            return w[1]

###################

parser = optparse.OptionParser()
parser.add_option("-d", "--dest", default = "temp",
        help = "destination folder")
parser.add_option("-c", "--config", default = "config.json",
        help = "config file")
parser.add_option("-p", "--plot", default = False,
        help = "plot only")

(opts, args) = parser.parse_args()

#Create result directory
result_dir = "./resutls/" + opts.dest + "/"
try:
    os.stat(result_dir)
except:
    os.makedirs(rsuult_dir)

## Make binary
if opts.plot == False:
    print "Building binary"
    status = subprocess.check_output('make clean; make', shell = True)

## Read config files
with open(opts.config) as json_data_file:
    json_data = json.load(json_data_file)

for test in json_data:
    data = json_data[test][0]
    final_dir = result_dir + test + "/"

    try:
        os.stat(final_dir)
    except:
        os.makedirs(final_dir)
    
    for index in data["index"]:
        for workload in data["workloads"]:
            for key_type in data["key_type"]:
                log = final_dir + index + "_" + workload + "_" + key_type
                log_file = open(log, "w+")
                
                for threads in data["threads"]:
                    cmd = "./workload " + workload + " " + key_type + " " + index
                    cmd = cmd + " " + str(threads)
                    print cmd
                    os.system(cmd + " >> tmp")
                    thp = getThroughput(workload)
                    print thp
                    os.system("rm -rf tmp")
                    log_file.write(str(threads) + " " + thp)
                
                log_file.close()
