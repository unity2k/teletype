#!/usr/bin/python2

from os.path import realpath 
from os import chdir
import subprocess


cfiles = []
includes = []

cflags = [
    "-march=ucr2",
    "-mpart=uc3b0512",
    "-D __AVR32_UC3B0512__",
    "-D BOARD=USER_BOARD",
    "-D UHD_ENABLE",
    "-Os",
    "-fshort-enums",
    "-fno-common",
    "-mrelax",
    "-mno-cond-exec-before-reload",
    "-funsigned-char",
    "-fno-strict-aliasing",
    "-ffunction-sections",
    "-fdata-sections"
    ]

path = realpath(__file__);
path += '/../../'
path = realpath(path);
chdir(path + '/lint')

with open("cfiles.txt") as file:
    cfiles = [line.strip() for line in file]

with open("includes.txt") as file:
    includes = [line.strip() for line in file]

with open("asfincludes.txt") as file:
    includes.extend(["libavr32/asf/" + line.strip() for line in file])

o = open("compile_commands.json", "w")


o.write("[\n");

for i,f in enumerate(cfiles):
    o.write('\t{ "directory": "%s",\n' % path)
    o.write('\t  "command": "avr32-gcc ' + ' '.join(cflags) + ' ')
    for inc in includes:
        o.write('-I%s ' % inc)
    o.write('-c -o %s %s",\n' % (f.replace('.c', '.o'), f))
    o.write('\t  "file": "%s/%s" }' % (path, f))
    if (i != len(cfiles) - 1):
        o.write(',')
    o.write('\n')

o.write(']\n')
o.close()

subprocess.call("oclint-json-compilation-database")
