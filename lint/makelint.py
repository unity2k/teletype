cfiles = []

with open("cfiles.txt") as file:
    cfiles = [line.strip() for line in file]

o = open("compile_commands.json", "w")

o.write("[\n");

for i,f in enumerate(cfiles):
    #f = "../" + f;
    o.write('\t{ "directory": "./",\n')
    o.write('\t  "command": "/opt/avr32/bin/avr32-gcc -Os -fshort-enums -fno-common -I./src -I./module -I./libavr32 -I./libavr32/src -c -o %s %s",\n' \
            % (f.replace('.c', '.o'), f))
    o.write('\t  "file": "%s" }' % f)
    if (i != len(cfiles) - 1):
        o.write(',');
    o.write('\n');

o.write("]\n");

