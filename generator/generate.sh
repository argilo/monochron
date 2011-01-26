#!/bin/bash
#
# This code is distributed under the GNU Public License
# which can be found at http://www.gnu.org/licenses/gpl.txt
#
# Copyright Clayton Smith 2011
#
# This script generates .png files containing the digits and days of the week
# that will be displayed on the clock's LCD.  To run it on Ubuntu, you will
# need to have the imagemagick and openoffice.org-common packages installed.
# After generating the .png files, run generate.php to convert them to C
# arrays for use in bitmaps.h.

# Large digits
for i in 0 1 2 3 4 5 6 7 8 9; do
	convert +antialias -font NewCenturySchlbk-Bold -pointsize 48 -size 26x40 xc:white -fill black -draw text\ -1,34\ \"$i\" l$i.png
done

# Small digits
for i in 0 1 2 3 4 5 6 7 8 9; do
	convert +antialias -font NewCenturySchlbk-Roman -pointsize 28 -size 15x24 xc:white -fill black -draw text\ -1,19\ \"$i\" s$i.png
done

# Days of the week
#
# Notes: Change the letters below to translate to another language.
# I've manually specified offsets for the second and third letters
# because ImageMagick's kerning is awful.
days1[0]="н";days2[0]="е";days3[0]="д";off2[0]=17;off3[0]=31
days1[1]="п";days2[1]="о";days3[1]="н";off2[1]=16;off3[1]=30
days1[2]="в";days2[2]="т";days3[2]="о";off2[2]=15;off3[2]=31
days1[3]="с";days2[3]="р";days3[3]="я";off2[3]=13;off3[3]=31
days1[4]="ч";days2[4]="е";days3[4]="т";off2[4]=16;off3[4]=31
days1[5]="п";days2[5]="е";days3[5]="т";off2[5]=16;off3[5]=31
days1[6]="с";days2[6]="ъ";days3[6]="б";off2[6]=13;off3[6]=32
for i in 0 1 2 3 4 5 6; do
	convert +antialias -font NewCenturySchlbk-Roman -pointsize 28 -kerning 2 -size 48x24 xc:white -fill black -draw text\ -3,19\ \"${days1[$i]}\" -draw text\ ${off2[$i]},19\ \"${days2[$i]}\" -draw text\ ${off3[$i]},19\ \"${days3[$i]}\" d$i.png
done
