#!/usr/bin/python3
# coding: utf-8
# statistics.py

from matplotlib import pyplot as plt
import csv

with open('log.txt', 'r') as csvfile:
    reader = csv.DictReader(csvfile, fieldnames=['+', '-', '*', '/', 'neg', 'au'], dialect='excel-tab')
    array = list(reader)

line_sum = []
line_mult = []

circle_sum = []
circle_mult = []

fft_sum = []
fft_mult = []

fht_sum = []
fht_mult = []

for row in array:
    if row['au'] == 'line':
        line_sum.append(row['+'])
        line_mult.append(row['*'])
    if row['au'] == 'circle':
        circle_sum.append(row['+'])
        circle_mult.append(row['*'])
    if row['au'] == 'FFT':
        fft_sum.append(row['+'])
        fft_mult.append(row['*'])
    if row['au'] == 'FHT':
        fht_sum.append(row['+'])
        fht_mult.append(row['*'])

arr_line_sum = [float(i) for i in line_sum]
arr_line_mult = [float(i) for i in line_mult]

arr_circle_sum = [float(i) for i in circle_sum]
arr_circle_mult = [float(i) for i in circle_mult]

arr_fft_sum = [float(i) for i in fft_sum]
arr_fft_mult = [float(i) for i in fft_mult]

arr_fht_sum = [float(i) for i in fht_sum]
arr_fht_mult = [float(i) for i in fht_mult]

fig = plt.figure(figsize=(6, 6.8))

ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)

line1 = ax1.plot(arr_line_sum, 'bo-', label='line')
line2 = ax1.plot(arr_circle_sum, 'go-', label='circle')
line3 = ax1.plot(arr_fft_sum, 'yo-', label='fft')
line4 = ax1.plot(arr_fht_sum, 'ro-', label='fht')

ax1.set_xlabel('amount of sums')


fig.subplots_adjust(bottom=0.15, left=0.1, top=0.98, right=0.95, hspace=0.31)
lines = line1+line2+line3+line4
labels = [l.get_label() for l in lines]
ax2.legend(lines, labels, loc=(0, -0.4), ncol=4)

ax2.plot(arr_line_mult, 'bo-', label='line')
ax2.plot(arr_circle_mult, 'go-', label='circle')
ax2.plot(arr_fft_mult, 'yo-', label='fft')
ax2.plot(arr_fht_mult, 'ro-', label='fht')

ax2.set_xlabel('amount of mults')

plt.show()
