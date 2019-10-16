EESchema Schematic File Version 4
LIBS:czujnikiRpi3b-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6700 3500 6700 3300
Text GLabel 6700 3500 0    10   BiDi ~ 0
+3V3
Wire Wire Line
	3900 3800 3600 3800
Wire Wire Line
	3600 3800 3600 4450
Wire Wire Line
	3900 3100 3600 3100
Wire Wire Line
	3600 3100 3600 3400
Wire Wire Line
	3600 3400 3600 3800
Wire Wire Line
	3900 2400 3600 2400
Wire Wire Line
	3600 2400 3600 2600
Wire Wire Line
	3600 2600 3600 3100
Wire Wire Line
	3900 2600 3600 2600
Wire Wire Line
	3900 3400 3600 3400
Connection ~ 3600 3800
Connection ~ 3600 2600
Connection ~ 3600 3100
Connection ~ 3600 3400
Text GLabel 3900 3800 0    10   BiDi ~ 0
GND
Wire Wire Line
	7100 2900 7100 2800
Text GLabel 6600 4100 0    10   BiDi ~ 0
GND
Wire Wire Line
	3400 3900 3900 3900
Wire Wire Line
	3900 4000 3400 4000
Wire Wire Line
	3400 4000 3400 3900
Connection ~ 3400 3900
Connection ~ 3900 4000
Wire Wire Line
	6000 4000 6050 4000
Wire Wire Line
	6050 4000 6700 4000
Wire Wire Line
	6000 3200 6050 3200
Wire Wire Line
	6050 3200 6050 4000
Wire Wire Line
	6700 4000 6700 3900
Connection ~ 6050 4000
Connection ~ 6700 4000
Wire Wire Line
	3400 2050 3400 3050
Wire Wire Line
	3100 3050 3400 3050
Wire Wire Line
	3400 3050 3400 3500
Connection ~ 3400 3050
Text GLabel 3400 2050 0    10   BiDi ~ 0
+4V35
Wire Wire Line
	3150 4500 3150 4250
Wire Wire Line
	6500 2800 6500 2500
Wire Wire Line
	6500 3600 6500 2800
Wire Wire Line
	6500 2800 6650 2800
Connection ~ 6500 2800
Wire Wire Line
	3100 3450 3100 3600
Wire Wire Line
	3100 3600 3100 4050
Wire Wire Line
	3100 4050 3350 4050
Wire Wire Line
	3200 3600 3100 3600
Connection ~ 3100 3600
Wire Wire Line
	3900 3700 3700 3700
Wire Wire Line
	3700 3700 3700 5250
Wire Wire Line
	3800 3600 3900 3600
Wire Wire Line
	3800 3600 3800 5550
$Comp
L czujnikiRpi3b-eagle-import:RASPBERRY_PI_B+TPLACE_A PCB?
U 1 0 5DF43393
P 4900 3100
AR Path="/5DF43393" Ref="PCB?"  Part="1" 
AR Path="/5DA750F6/5DF43393" Ref="PCB?"  Part="1" 
AR Path="/5DD2AD62/5DF43393" Ref="PCB?"  Part="1" 
F 0 "PCB?" H 4000 4200 50  0000 L BNN
F 1 "RASPBERRY_PI_B+TPLACE_A" H 4000 4125 50  0000 L BNN
F 2 "czujnikiRpi3b:RASPBERRY_PI_APLUS_TPLACE" H 4900 3100 50  0001 C CNN
F 3 "" H 4900 3100 50  0001 C CNN
	1    4900 3100
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:BSP170P Q?
U 1 0 5DF4339A
P 6600 3700
AR Path="/5DF4339A" Ref="Q?"  Part="1" 
AR Path="/5DA750F6/5DF4339A" Ref="Q?"  Part="1" 
AR Path="/5DD2AD62/5DF4339A" Ref="Q?"  Part="1" 
F 0 "Q?" H 6150 3650 59  0000 L BNN
F 1 "BSP170P" H 6150 3550 59  0000 L BNN
F 2 "czujnikiRpi3b:SOT223" H 6600 3700 50  0001 C CNN
F 3 "" H 6600 3700 50  0001 C CNN
	1    6600 3700
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:BSP170P Q?
U 1 0 5DF433A1
P 3300 3700
AR Path="/5DF433A1" Ref="Q?"  Part="1" 
AR Path="/5DA750F6/5DF433A1" Ref="Q?"  Part="1" 
AR Path="/5DD2AD62/5DF433A1" Ref="Q?"  Part="1" 
F 0 "Q?" H 3100 3600 59  0000 L BNN
F 1 "BSP170P" H 3100 3500 59  0000 L BNN
F 2 "czujnikiRpi3b:SOT223" H 3300 3700 50  0001 C CNN
F 3 "" H 3300 3700 50  0001 C CNN
	1    3300 3700
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:+3V3 #+3V?
U 1 0 5DF433A8
P 6700 3200
AR Path="/5DF433A8" Ref="#+3V?"  Part="1" 
AR Path="/5DA750F6/5DF433A8" Ref="#+3V?"  Part="1" 
AR Path="/5DD2AD62/5DF433A8" Ref="#+3V?"  Part="1" 
F 0 "#+3V?" H 6700 3200 50  0001 C CNN
F 1 "+3V3" V 6600 3000 59  0000 L BNN
F 2 "" H 6700 3200 50  0001 C CNN
F 3 "" H 6700 3200 50  0001 C CNN
	1    6700 3200
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:+3V3 #+3V?
U 1 0 5DF433AE
P 3400 1950
AR Path="/5DF433AE" Ref="#+3V?"  Part="1" 
AR Path="/5DA750F6/5DF433AE" Ref="#+3V?"  Part="1" 
AR Path="/5DD2AD62/5DF433AE" Ref="#+3V?"  Part="1" 
F 0 "#+3V?" H 3400 1950 50  0001 C CNN
F 1 "+4V25" V 3300 1750 59  0000 L BNN
F 2 "" H 3400 1950 50  0001 C CNN
F 3 "" H 3400 1950 50  0001 C CNN
	1    3400 1950
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:BC817SMD Q?
U 1 0 5DF433B4
P 3250 4250
AR Path="/5DF433B4" Ref="Q?"  Part="1" 
AR Path="/5DA750F6/5DF433B4" Ref="Q?"  Part="1" 
AR Path="/5DD2AD62/5DF433B4" Ref="Q?"  Part="1" 
F 0 "Q?" H 3350 4250 59  0000 L BNN
F 1 "BC817" H 3350 4150 59  0000 L BNN
F 2 "czujnikiRpi3b:SOT23-BEC" H 3250 4250 50  0001 C CNN
F 3 "" H 3250 4250 50  0001 C CNN
	1    3250 4250
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R2RPI?
U 1 0 5DF433BB
P 3100 3250
AR Path="/5DF433BB" Ref="R2RPI?"  Part="1" 
AR Path="/5DA750F6/5DF433BB" Ref="R2RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF433BB" Ref="R2RPI?"  Part="1" 
F 0 "R2RPI?" H 2950 3309 59  0000 L BNN
F 1 "56k" H 2950 3120 59  0000 L BNN
F 2 "czujnikiRpi3b:R1206" H 3100 3250 50  0001 C CNN
F 3 "" H 3100 3250 50  0001 C CNN
	1    3100 3250
	0    -1   -1   0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R_RPI?
U 1 0 5DF433C2
P 6500 2300
AR Path="/5DF433C2" Ref="R_RPI?"  Part="1" 
AR Path="/5DA750F6/5DF433C2" Ref="R_RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF433C2" Ref="R_RPI?"  Part="1" 
F 0 "R_RPI?" H 6350 2359 59  0000 L BNN
F 1 "56k" H 6350 2170 59  0000 L BNN
F 2 "czujnikiRpi3b:R1206" H 6500 2300 50  0001 C CNN
F 3 "" H 6500 2300 50  0001 C CNN
	1    6500 2300
	0    1    1    0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:_T679-?-1D2E2 LED?
U 1 0 5DF433C9
P 3900 4100
AR Path="/5DF433C9" Ref="LED?"  Part="1" 
AR Path="/5DA750F6/5DF433C9" Ref="LED?"  Part="1" 
AR Path="/5DD2AD62/5DF433C9" Ref="LED?"  Part="1" 
F 0 "LED?" V 3970 4010 59  0000 L BNN
F 1 "_T679-?-1D2E2" V 3970 3925 59  0000 L BNN
F 2 "czujnikiRpi3b:PLCC2" H 3900 4100 50  0001 C CNN
F 3 "" H 3900 4100 50  0001 C CNN
	1    3900 4100
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R3RPI?
U 1 0 5DF433D0
P 3900 4500
AR Path="/5DF433D0" Ref="R3RPI?"  Part="1" 
AR Path="/5DA750F6/5DF433D0" Ref="R3RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF433D0" Ref="R3RPI?"  Part="1" 
F 0 "R3RPI?" V 3859 4450 59  0000 R TNN
F 1 "1k" V 3770 4450 59  0000 R TNN
F 2 "czujnikiRpi3b:R1206" H 3900 4500 50  0001 C CNN
F 3 "" H 3900 4500 50  0001 C CNN
	1    3900 4500
	0    -1   -1   0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:_T679-?-1D2E2 LED?
U 1 0 5DF433D7
P 6800 4000
AR Path="/5DF433D7" Ref="LED?"  Part="1" 
AR Path="/5DA750F6/5DF433D7" Ref="LED?"  Part="1" 
AR Path="/5DD2AD62/5DF433D7" Ref="LED?"  Part="1" 
F 0 "LED?" V 6870 3910 59  0000 L BNN
F 1 "_T679-?-1D2E2" V 6870 3825 59  0000 L BNN
F 2 "czujnikiRpi3b:PLCC2" H 6800 4000 50  0001 C CNN
F 3 "" H 6800 4000 50  0001 C CNN
	1    6800 4000
	0    -1   -1   0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R3RPI?
U 1 0 5DF433DE
P 7200 4000
AR Path="/5DF433DE" Ref="R3RPI?"  Part="1" 
AR Path="/5DA750F6/5DF433DE" Ref="R3RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF433DE" Ref="R3RPI?"  Part="1" 
F 0 "R3RPI?" V 7159 3950 59  0000 R TNN
F 1 "1k" V 7070 3950 59  0000 R TNN
F 2 "czujnikiRpi3b:R1206" H 7200 4000 50  0001 C CNN
F 3 "" H 7200 4000 50  0001 C CNN
	1    7200 4000
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DF433E5
P 7400 4100
AR Path="/5DF433E5" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DF433E5" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD62/5DF433E5" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 7400 4100 50  0001 C CNN
F 1 "GND" H 7300 4000 59  0000 L BNN
F 2 "" H 7400 4100 50  0001 C CNN
F 3 "" H 7400 4100 50  0001 C CNN
	1    7400 4100
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:BZV55-C5V1 ZD1B?
U 1 0 5DF433EB
P 6750 2800
AR Path="/5DF433EB" Ref="ZD1B?"  Part="1" 
AR Path="/5DA750F6/5DF433EB" Ref="ZD1B?"  Part="1" 
AR Path="/5DD2AD62/5DF433EB" Ref="ZD1B?"  Part="1" 
F 0 "ZD1B?" H 6650 2870 59  0000 L BNN
F 1 "3v6" H 6650 2655 59  0000 L BNN
F 2 "czujnikiRpi3b:SOD80C" H 6750 2800 50  0001 C CNN
F 3 "" H 6750 2800 50  0001 C CNN
	1    6750 2800
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DF433F2
P 7100 3000
AR Path="/5DF433F2" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DF433F2" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD62/5DF433F2" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 7100 3000 50  0001 C CNN
F 1 "GND" H 7000 2900 59  0000 L BNN
F 2 "" H 7100 3000 50  0001 C CNN
F 3 "" H 7100 3000 50  0001 C CNN
	1    7100 3000
	1    0    0    -1  
$EndComp
Text GLabel 6000 2800 0    10   BiDi ~ 0
GND
Text GLabel 6000 2100 0    10   BiDi ~ 0
GND
Text GLabel 6000 3600 0    10   BiDi ~ 0
GND
Wire Wire Line
	3600 4450 3600 4500
Wire Wire Line
	3350 4450 3600 4450
Wire Wire Line
	3050 4900 3050 5350
Text GLabel 3050 4900 0    10   BiDi ~ 0
GND
Wire Wire Line
	3250 4950 3250 7700
Wire Wire Line
	3250 4550 3250 4500
Wire Wire Line
	3050 4700 3050 4500
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DF5C04E
P 3600 4600
AR Path="/5DF5C04E" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DF5C04E" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD62/5DF5C04E" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3600 4600 50  0001 C CNN
F 1 "GND" H 3500 4500 59  0000 L BNN
F 2 "" H 3600 4600 50  0001 C CNN
F 3 "" H 3600 4600 50  0001 C CNN
	1    3600 4600
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R1RPI?
U 1 0 5DF5C054
P 3250 4750
AR Path="/5DF5C054" Ref="R1RPI?"  Part="1" 
AR Path="/5DA750F6/5DF5C054" Ref="R1RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF5C054" Ref="R1RPI?"  Part="1" 
F 0 "R1RPI?" H 3100 4809 59  0000 L BNN
F 1 "56k" H 3100 4620 59  0000 L BNN
F 2 "czujnikiRpi3b:R1206" H 3250 4750 50  0001 C CNN
F 3 "" H 3250 4750 50  0001 C CNN
	1    3250 4750
	0    1    1    0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DF5C05B
P 3900 4800
AR Path="/5DF5C05B" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DF5C05B" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD62/5DF5C05B" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3900 4800 50  0001 C CNN
F 1 "GND" H 3800 4700 59  0000 L BNN
F 2 "" H 3900 4800 50  0001 C CNN
F 3 "" H 3900 4800 50  0001 C CNN
	1    3900 4800
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R3RPI?
U 1 0 5DF5C061
P 3700 5450
AR Path="/5DF5C061" Ref="R3RPI?"  Part="1" 
AR Path="/5DA750F6/5DF5C061" Ref="R3RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF5C061" Ref="R3RPI?"  Part="1" 
F 0 "R3RPI?" V 3809 5700 59  0000 R TNN
F 1 "1k" V 3620 5650 59  0000 R TNN
F 2 "czujnikiRpi3b:R1206" H 3700 5450 50  0001 C CNN
F 3 "" H 3700 5450 50  0001 C CNN
	1    3700 5450
	0    -1   -1   0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R1206 R3RPI?
U 1 0 5DF5C068
P 3800 5750
AR Path="/5DF5C068" Ref="R3RPI?"  Part="1" 
AR Path="/5DA750F6/5DF5C068" Ref="R3RPI?"  Part="1" 
AR Path="/5DD2AD62/5DF5C068" Ref="R3RPI?"  Part="1" 
F 0 "R3RPI?" V 3809 6150 59  0000 R TNN
F 1 "1k" V 3720 6050 59  0000 R TNN
F 2 "czujnikiRpi3b:R1206" H 3800 5750 50  0001 C CNN
F 3 "" H 3800 5750 50  0001 C CNN
	1    3800 5750
	0    -1   -1   0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:BZV55-C5V1 ZD1B?
U 1 0 5DF5C06F
P 3050 4800
AR Path="/5DF5C06F" Ref="ZD1B?"  Part="1" 
AR Path="/5DA750F6/5DF5C06F" Ref="ZD1B?"  Part="1" 
AR Path="/5DD2AD62/5DF5C06F" Ref="ZD1B?"  Part="1" 
F 0 "ZD1B?" H 2950 4870 59  0000 L BNN
F 1 "3v6" H 2950 4655 59  0000 L BNN
F 2 "czujnikiRpi3b:SOD80C" H 3050 4800 50  0001 C CNN
F 3 "" H 3050 4800 50  0001 C CNN
	1    3050 4800
	0    -1   -1   0   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DF5C076
P 3050 5450
AR Path="/5DF5C076" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DF5C076" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD62/5DF5C076" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3050 5450 50  0001 C CNN
F 1 "GND" H 2950 5350 59  0000 L BNN
F 2 "" H 3050 5450 50  0001 C CNN
F 3 "" H 3050 5450 50  0001 C CNN
	1    3050 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 4500 3250 4500
Wire Wire Line
	6850 2800 7100 2800
Text HLabel 7100 1950 2    50   Input ~ 0
PWR_3.3_EN
Wire Wire Line
	7100 1950 6500 1950
Wire Wire Line
	6500 1950 6500 2100
Text HLabel 3700 6300 0    50   Input ~ 0
TX
Text HLabel 3800 6450 0    50   Input ~ 0
RX
Wire Wire Line
	3700 6300 3700 5650
Wire Wire Line
	3800 6450 3800 5950
Text HLabel 3250 7700 0    50   Input ~ 0
PWR_5_EN
$EndSCHEMATC
