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
line_sum_add = []
line_mult_add = []

circle_sum = []
circle_mult = []
circle_sum_add = []
circle_mult_add = []
circle_sum_save = []
circle_mult_save = []

fft_sum = []
fft_mult = []
fft_sum_add = []
fft_mult_add = []
fft_sum_save = []
fft_mult_save = []

fht_sum = []
fht_mult = []
fht_sum_add = []
fht_mult_add = []
fht_sum_save = []
fht_mult_save = []

for row in array:
    if row['au'] == 'line':
        line_sum.append(row['+'])
        line_mult.append(row['*'])
    if row['au'] == 'line-add':
        line_sum_add.append(row['+'])
        line_mult_add.append(row['*'])
    if row['au'] == 'circle':
        circle_sum.append(row['+'])
        circle_mult.append(row['*'])
    if row['au'] == 'circle-add':
        circle_sum_add.append(row['+'])
        circle_mult_add.append(row['*'])
    if row['au'] == 'circle-save':
        circle_sum_save.append(row['+'])
        circle_mult_save.append(row['*'])
    if row['au'] == 'FFT':
        fft_sum.append(row['+'])
        fft_mult.append(row['*'])
    if row['au'] == 'FFT-add':
        fft_sum_add.append(row['+'])
        fft_mult_add.append(row['*'])
    if row['au'] == 'FFT-save':
        fft_sum_save.append(row['+'])
        fft_mult_save.append(row['*'])
    if row['au'] == 'FHT':
        fht_sum.append(row['+'])
        fht_mult.append(row['*'])
    if row['au'] == 'FHT-add':
        fht_sum_add.append(row['+'])
        fht_mult_add.append(row['*'])
    if row['au'] == 'FHT-save':
        fht_sum_save.append(row['+'])
        fht_mult_save.append(row['*'])

arr_line_sum = [float(i) for i in line_sum]
arr_line_mult = [float(i) for i in line_mult]
arr_line_sum_add = [float(i) for i in line_sum_add]
arr_line_mult_add = [float(i) for i in line_mult_add]

arr_circle_sum = [float(i) for i in circle_sum]
arr_circle_mult = [float(i) for i in circle_mult]
arr_circle_sum_add = [float(i) for i in circle_sum_add]
arr_circle_mult_add = [float(i) for i in circle_mult_add]
arr_circle_sum_save = [float(i) for i in circle_sum_save]
arr_circle_mult_save = [float(i) for i in circle_mult_save]

arr_fft_sum = [float(i) for i in fft_sum]
arr_fft_mult = [float(i) for i in fft_mult]
arr_fft_sum_add = [float(i) for i in fft_sum_add]
arr_fft_mult_add = [float(i) for i in fft_mult_add]
arr_fft_sum_save = [float(i) for i in fft_sum_save]
arr_fft_mult_save = [float(i) for i in fft_mult_save]

arr_fht_sum = [float(i) for i in fht_sum]
arr_fht_mult = [float(i) for i in fht_mult]
arr_fht_sum_add = [float(i) for i in fht_sum_add]
arr_fht_mult_add = [float(i) for i in fht_mult_add]
arr_fht_sum_save = [float(i) for i in fht_sum_save]
arr_fht_mult_save = [float(i) for i in fht_mult_save]

fig = plt.figure(figsize=(16, 6.8))

ax1 = fig.add_subplot(211)
ax2 = fig.add_subplot(212)

line1 = ax1.plot(arr_line_sum, 'b', label='line')
line11 = ax1.plot(arr_line_sum_add, 'g', label='line-add')
line2 = ax1.plot(arr_circle_sum, 'r', label='circle')
line22 = ax1.plot(arr_circle_sum_add, 'c', label='circle-add')
line222 = ax1.plot(arr_circle_sum_save, 'm', label='circle-save')
line3 = ax1.plot(arr_fft_sum, 'y', label='fft')
line33 = ax1.plot(arr_fft_sum_add, 'k', label='fft-add')
line333 = ax1.plot(arr_fft_sum_save, 'w', label='fft-save')
line4 = ax1.plot(arr_fht_sum, 'tab:pink', label='fht')
line44 = ax1.plot(arr_fht_sum_add, 'tab:olive', label='fht-add')
line444 = ax1.plot(arr_fht_sum_save, 'tab:orange', label='fht-save')

ax1.set_xlabel('amount of sums')


fig.subplots_adjust(bottom=0.15, left=0.1, top=0.98, right=0.95, hspace=0.31)
lines = line1+line11+line2+line22+line222+line3+line33+line333+line4+line44+line444
labels = [l.get_label() for l in lines]
ax2.legend(lines, labels, loc=(0, -0.4), ncol=11)

ax2.plot(arr_line_mult, 'b', label='line')
ax2.plot(arr_line_mult_add, 'g', label='line-add')
ax2.plot(arr_circle_mult, 'r', label='circle')
ax2.plot(arr_circle_mult_add, 'c', label='circle-add')
ax2.plot(arr_circle_mult_save, 'm', label='circle-save')
ax2.plot(arr_fft_mult, 'y', label='fft')
ax2.plot(arr_fft_mult_add, 'k', label='fft-add')
ax2.plot(arr_fft_mult_save, 'w', label='fft-save')
ax2.plot(arr_fht_mult, 'tab:pink', label='fht')
ax2.plot(arr_fht_mult_add, 'tab:olive', label='fht-add')
ax2.plot(arr_fht_mult_save, 'tab:orange', label='fht-save')

ax2.set_xlabel('amount of mults')

plt.show()
