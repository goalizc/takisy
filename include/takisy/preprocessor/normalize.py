#!/usr/bin/env python
# coding: utf-8

import os, sys

def match(line):
    for c in line:
        if c not in "# \n":
            return False
    else:
        return True

def deal(hpp):
    print "=================: ", hpp
    content = open(hpp).read()

    while True:
        pos = content.find("/*")
        if pos == -1:
            break
        pos2 = content.find("*/", pos)
        if pos2 == -1:
            print "=====: cannot find */"
        if not match(content[:pos]):
            break
        content = content[:pos] + content[pos2+2:]

    lines = content.split("\n")
    i = 0
    for line in lines:
        if not match(line):
            break
        i += 1

    content = "\n".join(lines[i:])
    os.rename(hpp, "%s.orig" % hpp)
    open(hpp, "w").write(content)

if __name__ == '__main__':
    for hpp in sys.argv[1:]:
        deal(hpp)
