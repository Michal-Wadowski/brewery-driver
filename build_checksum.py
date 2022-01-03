import zlib
from os import listdir, rename
from os.path import isfile, join

filename = 'driver'

with open(filename, 'rb') as f:
    content = f.read()

checksum = hex(zlib.crc32(content) & 0xffffffff)[2:]

dst_filename = filename + '-' + checksum
print("rename to " + dst_filename)
rename(filename, dst_filename)

