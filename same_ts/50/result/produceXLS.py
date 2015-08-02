#!/usr/bin/env python
import os
import xlwt
import sys
import string
reload(sys)
sys.setdefaultencoding('utf8')

filename = sys.argv[1]
outfilename = sys.argv[2]

def download(filename):
    font0 = xlwt.Font()
    font0.name = 'Times New Roman'
    font0.colour_index = 2
    font0.bold = True

    style0 = xlwt.XFStyle()
    style0.font = font0

    style1 = xlwt.XFStyle()
    style1.num_format_str = 'D-MMM-YY'

    wb = xlwt.Workbook()
    ws = wb.add_sheet('A Test Sheet')

    f = open(filename, "r")
    row = 0
    col = 0
    lines = f.readline()
    while lines:
        for item in lines.strip().split():
            #print row, col, item,
            ws.write(row, col, item)
            col += 1
        row += 1
        col = 0
        lines = f.readline()
    wb.save(outfilename)
    return

download(filename)
