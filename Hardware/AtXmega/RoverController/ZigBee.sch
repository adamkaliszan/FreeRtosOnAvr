EESchema Schematic File Version 4
LIBS:czujnikiRpi3b-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 9
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
	7250 3100 7250 3350
Wire Wire Line
	7250 3350 7250 3450
Wire Wire Line
	7250 3450 6900 3450
Wire Wire Line
	7250 3350 6900 3350
Connection ~ 7250 3450
Connection ~ 7250 3350
Text GLabel 7250 3100 0    10   BiDi ~ 0
+3V3
Wire Wire Line
	7250 4050 7250 3950
Wire Wire Line
	7250 3950 6900 3950
Wire Wire Line
	7250 3750 7250 3850
Wire Wire Line
	7250 3850 7250 3950
Wire Wire Line
	7250 3850 6900 3850
Connection ~ 7250 3850
Connection ~ 7250 3950
Text GLabel 7250 4050 0    10   BiDi ~ 0
GND
Wire Wire Line
	6900 4250 7450 4250
Text Label 6900 4250 0    10   ~ 0
PD0
Wire Wire Line
	5550 2850 5800 2850
Wire Wire Line
	5550 2950 5800 2950
$Comp
L czujnikiRpi3b-eagle-import:XCORE2530 U$?
U 1 0 5DD5C5ED
P 8000 3850
AR Path="/5DD5C5ED" Ref="U$?"  Part="1" 
AR Path="/5DA750F6/5DD5C5ED" Ref="U$?"  Part="1" 
AR Path="/5DD2AD87/5DD5C5ED" Ref="U$?"  Part="1" 
F 0 "U$?" H 8000 3850 50  0001 C CNN
F 1 "XCORE2530" H 8000 3850 50  0001 C CNN
F 2 "czujnikiRpi3b:XCORE2530" H 8000 3850 50  0001 C CNN
F 3 "" H 8000 3850 50  0001 C CNN
	1    8000 3850
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DD5C5F3
P 7250 4150
AR Path="/5DD5C5F3" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DD5C5F3" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD87/5DD5C5F3" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 7250 4150 50  0001 C CNN
F 1 "GND" H 7150 4050 59  0000 L BNN
F 2 "" H 7250 4150 50  0001 C CNN
F 3 "" H 7250 4150 50  0001 C CNN
	1    7250 4150
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:+3V3 #+3V?
U 1 0 5DD5C5F9
P 7250 3000
AR Path="/5DD5C5F9" Ref="#+3V?"  Part="1" 
AR Path="/5DA750F6/5DD5C5F9" Ref="#+3V?"  Part="1" 
AR Path="/5DD2AD87/5DD5C5F9" Ref="#+3V?"  Part="1" 
F 0 "#+3V?" H 7250 3000 50  0001 C CNN
F 1 "+3V3" V 7150 2800 59  0000 L BNN
F 2 "" H 7250 3000 50  0001 C CNN
F 3 "" H 7250 3000 50  0001 C CNN
	1    7250 3000
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:C-EUC0805 C?
U 1 0 5DD5C5FF
P 7250 3550
AR Path="/5DD5C5FF" Ref="C?"  Part="1" 
AR Path="/5DA750F6/5DD5C5FF" Ref="C?"  Part="1" 
AR Path="/5DD2AD87/5DD5C5FF" Ref="C?"  Part="1" 
F 0 "C?" H 7310 3565 59  0000 L BNN
F 1 "C-EUC0805" H 7310 3365 59  0000 L BNN
F 2 "czujnikiRpi3b:C0805" H 7250 3550 50  0001 C CNN
F 3 "" H 7250 3550 50  0001 C CNN
	1    7250 3550
	1    0    0    -1  
$EndComp
Text HLabel 7450 4250 2    50   Input ~ 0
RST
Text HLabel 5550 2850 0    50   Input ~ 0
TX
Text HLabel 5550 2950 0    50   Input ~ 0
RX
NoConn ~ 5800 2650
NoConn ~ 5800 2750
NoConn ~ 5800 3050
NoConn ~ 5800 3150
NoConn ~ 5800 3250
NoConn ~ 5800 3350
NoConn ~ 5800 3550
NoConn ~ 5800 3650
NoConn ~ 5800 3750
NoConn ~ 5800 3850
NoConn ~ 5800 3950
NoConn ~ 5800 4050
NoConn ~ 5800 4150
NoConn ~ 5800 4250
NoConn ~ 5800 4450
NoConn ~ 5800 4550
NoConn ~ 5800 4650
NoConn ~ 5800 4750
NoConn ~ 5800 4850
$EndSCHEMATC
