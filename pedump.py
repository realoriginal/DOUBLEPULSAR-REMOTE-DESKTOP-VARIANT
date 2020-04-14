#!/usr/bin/env python2.7
# -*- coding:utf-8 -*-
import pefile
import sys

if __name__ in '__main__':
    g_pe = pefile.PE("scbin.exe");
    sect = g_pe.sections[0].get_data();

    f = open("scbin.bin", "wb+");
    f.write(sect);
    f.close()
