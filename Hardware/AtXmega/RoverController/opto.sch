EESchema Schematic File Version 4
LIBS:czujnikiRpi3b-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 9
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
	2850 2100 2850 2300
Text Label 2850 2100 0    10   ~ 0
+12V
Wire Wire Line
	3550 3300 3550 2300
Wire Wire Line
	3550 2300 3450 2300
$Comp
L czujnikiRpi3b-eagle-import:78LXX IC?
U 1 0 5DDFEC35
P 3150 2300
AR Path="/5DDFEC35" Ref="IC?"  Part="1" 
AR Path="/5DA750F6/5DDFEC35" Ref="IC?"  Part="1" 
AR Path="/5DD2AD78/5DDFEC35" Ref="IC?"  Part="1" 
F 0 "IC?" H 3250 2000 59  0000 L BNN
F 1 "78LXX" H 3250 1900 59  0000 L BNN
F 2 "czujnikiRpi3b:78LXX" H 3150 2300 50  0001 C CNN
F 3 "" H 3150 2300 50  0001 C CNN
	1    3150 2300
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DDFEC3C
P 3150 2700
AR Path="/5DDFEC3C" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DDFEC3C" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DDFEC3C" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3150 2700 50  0001 C CNN
F 1 "GND" H 3050 2600 59  0000 L BNN
F 2 "" H 3150 2700 50  0001 C CNN
F 3 "" H 3150 2700 50  0001 C CNN
	1    3150 2700
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:+12V #P+?
U 1 0 5DDFEC42
P 2850 2000
AR Path="/5DDFEC42" Ref="#P+?"  Part="1" 
AR Path="/5DA750F6/5DDFEC42" Ref="#P+?"  Part="1" 
AR Path="/5DD2AD78/5DDFEC42" Ref="#P+?"  Part="1" 
F 0 "#P+?" H 2850 2000 50  0001 C CNN
F 1 "+12V" V 2750 1800 59  0000 L BNN
F 2 "" H 2850 2000 50  0001 C CNN
F 3 "" H 2850 2000 50  0001 C CNN
	1    2850 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3700 4450 3700
Text GLabel 4350 3700 0    10   BiDi ~ 0
GND
Wire Wire Line
	4350 4300 4450 4300
Text GLabel 4350 4300 0    10   BiDi ~ 0
GND
Wire Wire Line
	4350 4900 4450 4900
Text GLabel 4350 4900 0    10   BiDi ~ 0
GND
Wire Wire Line
	4350 5500 4450 5500
Text GLabel 4350 5500 0    10   BiDi ~ 0
GND
Wire Wire Line
	6150 5500 5850 5500
Text GLabel 6150 5500 0    10   BiDi ~ 0
GND
Wire Wire Line
	6150 4900 5850 4900
Text GLabel 6150 4900 0    10   BiDi ~ 0
GND
Wire Wire Line
	5850 4300 6150 4300
Text GLabel 5850 4300 0    10   BiDi ~ 0
GND
Wire Wire Line
	5850 3700 6150 3700
Text GLabel 5850 3700 0    10   BiDi ~ 0
GND
Text Label 3750 3500 0    70   ~ 0
PC4
Text Label 3750 4700 0    70   ~ 0
PA5
Text Label 3750 4100 0    70   ~ 0
PC5
Text Label 3750 5300 0    70   ~ 0
PA7
Wire Wire Line
	4250 4100 4450 4100
Text Label 4250 4100 0    70   ~ 0
PWMA
Wire Wire Line
	4250 5300 4450 5300
Text Label 4250 5300 0    70   ~ 0
A1_IN
Wire Wire Line
	4250 4700 4450 4700
Text Label 4250 4700 0    70   ~ 0
A2_IN
Wire Wire Line
	4250 3500 4450 3500
Text Label 4250 3500 0    70   ~ 0
PWMB
Wire Wire Line
	3550 3300 5250 3300
Wire Wire Line
	5250 3300 5250 3500
Wire Wire Line
	5250 3500 5250 4100
Wire Wire Line
	5250 4100 5250 4700
Wire Wire Line
	5250 4700 5250 5300
Wire Wire Line
	5250 5300 5150 5300
Wire Wire Line
	5150 4700 5250 4700
Wire Wire Line
	5150 4100 5250 4100
Wire Wire Line
	5150 3500 5250 3500
Connection ~ 5250 4700
Connection ~ 5250 4100
Connection ~ 5250 3500
Wire Wire Line
	5450 3700 5350 3700
Wire Wire Line
	5350 3700 5150 3700
Wire Wire Line
	5550 3500 5350 3500
Wire Wire Line
	5350 3500 5350 3700
Connection ~ 5350 3700
Wire Wire Line
	5450 4900 5350 4900
Wire Wire Line
	5350 4900 5150 4900
Wire Wire Line
	5450 4700 5350 4700
Wire Wire Line
	5350 4700 5350 4900
Connection ~ 5350 4900
Wire Wire Line
	5450 4300 5350 4300
Wire Wire Line
	5350 4300 5150 4300
Wire Wire Line
	5550 4100 5350 4100
Wire Wire Line
	5350 4100 5350 4300
Connection ~ 5350 4300
Wire Wire Line
	5450 5500 5350 5500
Wire Wire Line
	5350 5500 5150 5500
Wire Wire Line
	5550 5300 5350 5300
Wire Wire Line
	5350 5300 5350 5500
Connection ~ 5350 5500
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_PWMB?
U 1 0 5DE292E0
P 4050 4100
AR Path="/5DE292E0" Ref="R_PWMB?"  Part="1" 
AR Path="/5DA750F6/5DE292E0" Ref="R_PWMB?"  Part="1" 
AR Path="/5DD2AD78/5DE292E0" Ref="R_PWMB?"  Part="1" 
F 0 "R_PWMB?" H 4150 4041 59  0000 R TNN
F 1 "1k" H 4150 4230 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 4050 4100 50  0001 C CNN
F 3 "" H 4050 4100 50  0001 C CNN
	1    4050 4100
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_A1IN?
U 1 0 5DE292E7
P 4050 5300
AR Path="/5DE292E7" Ref="R_A1IN?"  Part="1" 
AR Path="/5DA750F6/5DE292E7" Ref="R_A1IN?"  Part="1" 
AR Path="/5DD2AD78/5DE292E7" Ref="R_A1IN?"  Part="1" 
F 0 "R_A1IN?" H 4150 5241 59  0000 R TNN
F 1 "1k" H 4150 5430 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 4050 5300 50  0001 C CNN
F 3 "" H 4050 5300 50  0001 C CNN
	1    4050 5300
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_A2IN?
U 1 0 5DE292EE
P 4050 4700
AR Path="/5DE292EE" Ref="R_A2IN?"  Part="1" 
AR Path="/5DA750F6/5DE292EE" Ref="R_A2IN?"  Part="1" 
AR Path="/5DD2AD78/5DE292EE" Ref="R_A2IN?"  Part="1" 
F 0 "R_A2IN?" H 4150 4641 59  0000 R TNN
F 1 "1k" H 4150 4830 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 4050 4700 50  0001 C CNN
F 3 "" H 4050 4700 50  0001 C CNN
	1    4050 4700
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_PWMA?
U 1 0 5DE292F5
P 4050 3500
AR Path="/5DE292F5" Ref="R_PWMA?"  Part="1" 
AR Path="/5DA750F6/5DE292F5" Ref="R_PWMA?"  Part="1" 
AR Path="/5DD2AD78/5DE292F5" Ref="R_PWMA?"  Part="1" 
F 0 "R_PWMA?" H 3950 3559 59  0000 L BNN
F 1 "1k" H 3950 3370 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 4050 3500 50  0001 C CNN
F 3 "" H 4050 3500 50  0001 C CNN
	1    4050 3500
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_A?
U 1 0 5DE292FC
P 4850 5400
AR Path="/5DE292FC" Ref="OK_A?"  Part="1" 
AR Path="/5DA750F6/5DE292FC" Ref="OK_A?"  Part="1" 
AR Path="/5DD2AD78/5DE292FC" Ref="OK_A?"  Part="1" 
F 0 "OK_A?" H 4575 5625 59  0000 L BNN
F 1 "LTV816S" H 4575 5100 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 4850 5400 50  0001 C CNN
F 3 "" H 4850 5400 50  0001 C CNN
	1    4850 5400
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_PWMA?
U 1 0 5DE29303
P 4850 4200
AR Path="/5DE29303" Ref="OK_PWMA?"  Part="1" 
AR Path="/5DA750F6/5DE29303" Ref="OK_PWMA?"  Part="1" 
AR Path="/5DD2AD78/5DE29303" Ref="OK_PWMA?"  Part="1" 
F 0 "OK_PWMA?" H 4575 4425 59  0000 L BNN
F 1 "LTV816S" H 4575 3900 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 4850 4200 50  0001 C CNN
F 3 "" H 4850 4200 50  0001 C CNN
	1    4850 4200
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_PWMB?
U 1 0 5DE2930A
P 4850 3600
AR Path="/5DE2930A" Ref="OK_PWMB?"  Part="1" 
AR Path="/5DA750F6/5DE2930A" Ref="OK_PWMB?"  Part="1" 
AR Path="/5DD2AD78/5DE2930A" Ref="OK_PWMB?"  Part="1" 
F 0 "OK_PWMB?" H 4575 3825 59  0000 L BNN
F 1 "LTV816S" H 4575 3300 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 4850 3600 50  0001 C CNN
F 3 "" H 4850 3600 50  0001 C CNN
	1    4850 3600
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_A?
U 1 0 5DE29311
P 4850 4800
AR Path="/5DE29311" Ref="OK_A?"  Part="1" 
AR Path="/5DA750F6/5DE29311" Ref="OK_A?"  Part="1" 
AR Path="/5DD2AD78/5DE29311" Ref="OK_A?"  Part="1" 
F 0 "OK_A?" H 4575 5025 59  0000 L BNN
F 1 "LTV816S" H 4575 4500 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 4850 4800 50  0001 C CNN
F 3 "" H 4850 4800 50  0001 C CNN
	1    4850 4800
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE29318
P 4350 3800
AR Path="/5DE29318" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE29318" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE29318" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 4350 3800 50  0001 C CNN
F 1 "GND" H 4250 3700 59  0000 L BNN
F 2 "" H 4350 3800 50  0001 C CNN
F 3 "" H 4350 3800 50  0001 C CNN
	1    4350 3800
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE2931E
P 4350 4400
AR Path="/5DE2931E" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE2931E" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE2931E" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 4350 4400 50  0001 C CNN
F 1 "GND" H 4250 4300 59  0000 L BNN
F 2 "" H 4350 4400 50  0001 C CNN
F 3 "" H 4350 4400 50  0001 C CNN
	1    4350 4400
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_PWMA?
U 1 0 5DE29324
P 5650 3700
AR Path="/5DE29324" Ref="R_PWMA?"  Part="1" 
AR Path="/5DA750F6/5DE29324" Ref="R_PWMA?"  Part="1" 
AR Path="/5DD2AD78/5DE29324" Ref="R_PWMA?"  Part="1" 
F 0 "R_PWMA?" H 5550 3759 59  0000 L BNN
F 1 "1k" H 5550 3570 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 5650 3700 50  0001 C CNN
F 3 "" H 5650 3700 50  0001 C CNN
	1    5650 3700
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_PWMB?
U 1 0 5DE2932B
P 5650 4300
AR Path="/5DE2932B" Ref="R_PWMB?"  Part="1" 
AR Path="/5DA750F6/5DE2932B" Ref="R_PWMB?"  Part="1" 
AR Path="/5DD2AD78/5DE2932B" Ref="R_PWMB?"  Part="1" 
F 0 "R_PWMB?" H 5750 4241 59  0000 R TNN
F 1 "1k" H 5750 4430 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 5650 4300 50  0001 C CNN
F 3 "" H 5650 4300 50  0001 C CNN
	1    5650 4300
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_A2IN?
U 1 0 5DE29332
P 5650 4900
AR Path="/5DE29332" Ref="R_A2IN?"  Part="1" 
AR Path="/5DA750F6/5DE29332" Ref="R_A2IN?"  Part="1" 
AR Path="/5DD2AD78/5DE29332" Ref="R_A2IN?"  Part="1" 
F 0 "R_A2IN?" H 5750 4841 59  0000 R TNN
F 1 "1k" H 5750 5030 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 5650 4900 50  0001 C CNN
F 3 "" H 5650 4900 50  0001 C CNN
	1    5650 4900
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE29339
P 4350 5000
AR Path="/5DE29339" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE29339" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE29339" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 4350 5000 50  0001 C CNN
F 1 "GND" H 4250 4900 59  0000 L BNN
F 2 "" H 4350 5000 50  0001 C CNN
F 3 "" H 4350 5000 50  0001 C CNN
	1    4350 5000
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE2933F
P 4350 5600
AR Path="/5DE2933F" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE2933F" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE2933F" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 4350 5600 50  0001 C CNN
F 1 "GND" H 4250 5500 59  0000 L BNN
F 2 "" H 4350 5600 50  0001 C CNN
F 3 "" H 4350 5600 50  0001 C CNN
	1    4350 5600
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_A1IN?
U 1 0 5DE29345
P 5650 5500
AR Path="/5DE29345" Ref="R_A1IN?"  Part="1" 
AR Path="/5DA750F6/5DE29345" Ref="R_A1IN?"  Part="1" 
AR Path="/5DD2AD78/5DE29345" Ref="R_A1IN?"  Part="1" 
F 0 "R_A1IN?" H 5750 5441 59  0000 R TNN
F 1 "1k" H 5750 5630 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 5650 5500 50  0001 C CNN
F 3 "" H 5650 5500 50  0001 C CNN
	1    5650 5500
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 1 0 5DE2934C
P 5650 3500
AR Path="/5DE2934C" Ref="SV?"  Part="1" 
AR Path="/5DA750F6/5DE2934C" Ref="SV?"  Part="1" 
AR Path="/5DD2AD78/5DE2934C" Ref="SV?"  Part="1" 
F 0 "SV?" H 5750 3470 59  0000 L BNN
F 1 "ML10E" H 5640 3385 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 5650 3500 50  0001 C CNN
F 3 "" H 5650 3500 50  0001 C CNN
	1    5650 3500
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 2 0 5DE29353
P 5650 4100
AR Path="/5DE29353" Ref="SV?"  Part="2" 
AR Path="/5DA750F6/5DE29353" Ref="SV?"  Part="2" 
AR Path="/5DD2AD78/5DE29353" Ref="SV?"  Part="2" 
F 0 "SV?" H 5750 4070 59  0000 L BNN
F 1 "ML10E" H 5640 3985 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 5650 4100 50  0001 C CNN
F 3 "" H 5650 4100 50  0001 C CNN
	2    5650 4100
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 3 0 5DE2935A
P 5650 5300
AR Path="/5DE2935A" Ref="SV?"  Part="3" 
AR Path="/5DA750F6/5DE2935A" Ref="SV?"  Part="3" 
AR Path="/5DD2AD78/5DE2935A" Ref="SV?"  Part="3" 
F 0 "SV?" H 5750 5270 59  0000 L BNN
F 1 "ML10E" H 5640 5185 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 5650 5300 50  0001 C CNN
F 3 "" H 5650 5300 50  0001 C CNN
	3    5650 5300
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 5 0 5DE29361
P 5550 4700
AR Path="/5DE29361" Ref="SV?"  Part="5" 
AR Path="/5DA750F6/5DE29361" Ref="SV?"  Part="5" 
AR Path="/5DD2AD78/5DE29361" Ref="SV?"  Part="5" 
F 0 "SV?" H 5650 4670 59  0000 L BNN
F 1 "ML10E" H 5540 4585 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 5550 4700 50  0001 C CNN
F 3 "" H 5550 4700 50  0001 C CNN
	5    5550 4700
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 9 0 5DE29368
P 5650 3900
AR Path="/5DE29368" Ref="SV?"  Part="9" 
AR Path="/5DA750F6/5DE29368" Ref="SV?"  Part="9" 
AR Path="/5DD2AD78/5DE29368" Ref="SV?"  Part="9" 
F 0 "SV?" H 5750 3870 59  0000 L BNN
F 1 "ML10E" H 5640 3785 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 5650 3900 50  0001 C CNN
F 3 "" H 5650 3900 50  0001 C CNN
	9    5650 3900
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 10 0 5DE2936F
P 5650 5100
AR Path="/5DE2936F" Ref="SV?"  Part="10" 
AR Path="/5DA750F6/5DE2936F" Ref="SV?"  Part="10" 
AR Path="/5DD2AD78/5DE2936F" Ref="SV?"  Part="10" 
F 0 "SV?" H 5750 5070 59  0000 L BNN
F 1 "ML10E" H 5640 4985 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 5650 5100 50  0001 C CNN
F 3 "" H 5650 5100 50  0001 C CNN
	10   5650 5100
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE29376
P 6150 5600
AR Path="/5DE29376" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE29376" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE29376" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 6150 5600 50  0001 C CNN
F 1 "GND" H 6050 5500 59  0000 L BNN
F 2 "" H 6150 5600 50  0001 C CNN
F 3 "" H 6150 5600 50  0001 C CNN
	1    6150 5600
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE2937C
P 6150 5000
AR Path="/5DE2937C" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE2937C" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE2937C" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 6150 5000 50  0001 C CNN
F 1 "GND" H 6050 4900 59  0000 L BNN
F 2 "" H 6150 5000 50  0001 C CNN
F 3 "" H 6150 5000 50  0001 C CNN
	1    6150 5000
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE29382
P 6150 4400
AR Path="/5DE29382" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE29382" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE29382" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 6150 4400 50  0001 C CNN
F 1 "GND" H 6050 4300 59  0000 L BNN
F 2 "" H 6150 4400 50  0001 C CNN
F 3 "" H 6150 4400 50  0001 C CNN
	1    6150 4400
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE29388
P 6150 3800
AR Path="/5DE29388" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE29388" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE29388" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 6150 3800 50  0001 C CNN
F 1 "GND" H 6050 3700 59  0000 L BNN
F 2 "" H 6150 3800 50  0001 C CNN
F 3 "" H 6150 3800 50  0001 C CNN
	1    6150 3800
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE2938E
P 5550 5200
AR Path="/5DE2938E" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE2938E" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE2938E" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 5550 5200 50  0001 C CNN
F 1 "GND" H 5450 5100 59  0000 L BNN
F 2 "" H 5550 5200 50  0001 C CNN
F 3 "" H 5550 5200 50  0001 C CNN
	1    5550 5200
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DE29394
P 5550 4000
AR Path="/5DE29394" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DE29394" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DE29394" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 5550 4000 50  0001 C CNN
F 1 "GND" H 5450 3900 59  0000 L BNN
F 2 "" H 5550 4000 50  0001 C CNN
F 3 "" H 5550 4000 50  0001 C CNN
	1    5550 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 4400 2350 4400
Text GLabel 2250 4400 0    10   BiDi ~ 0
GND
Wire Wire Line
	3500 5350 3600 5350
Wire Wire Line
	3600 5350 3600 5200
Text GLabel 3500 5350 0    10   BiDi ~ 0
GND
Wire Wire Line
	3600 4550 3600 4700
Wire Wire Line
	3600 4700 3500 4700
Text GLabel 3600 4550 0    10   BiDi ~ 0
GND
Wire Wire Line
	2250 5600 2350 5600
Text GLabel 2250 5600 0    10   BiDi ~ 0
GND
Wire Wire Line
	2250 5000 2350 5000
Text GLabel 2250 5000 0    10   BiDi ~ 0
GND
Wire Wire Line
	1800 4200 1700 4200
Text Label 1700 4200 0    70   ~ 0
PR1
Wire Wire Line
	1700 4800 1800 4800
Text Label 1700 4800 0    70   ~ 0
PA6
Wire Wire Line
	1800 5400 1700 5400
Text Label 1700 5400 0    70   ~ 0
PA4
Wire Wire Line
	2200 4800 2350 4800
Text Label 2275 4800 0    70   ~ 0
B2_IN
Wire Wire Line
	2200 5400 2350 5400
Text Label 2350 5400 0    70   ~ 0
B1_IN
Wire Wire Line
	3150 5400 3150 4800
Wire Wire Line
	3150 4800 3150 4200
Wire Wire Line
	3150 4200 3150 4100
Wire Wire Line
	3150 4100 3550 4100
Wire Wire Line
	3550 4100 3550 3300
Wire Wire Line
	3050 4200 3150 4200
Wire Wire Line
	3050 4800 3150 4800
Wire Wire Line
	3050 5400 3150 5400
Wire Wire Line
	3250 4200 3150 4200
Connection ~ 3150 4200
Connection ~ 3150 4800
Wire Wire Line
	2200 4200 2350 4200
Wire Wire Line
	3250 5600 3050 5600
Wire Wire Line
	3050 5800 3050 5600
Connection ~ 3050 5600
Wire Wire Line
	3200 5200 3200 5000
Wire Wire Line
	3050 5000 3200 5000
Wire Wire Line
	3200 5000 3250 5000
Connection ~ 3200 5000
Wire Wire Line
	3200 4400 3200 4550
Wire Wire Line
	3250 4400 3200 4400
Wire Wire Line
	3200 4400 3050 4400
Connection ~ 3200 4400
Entry Wire Line
	1600 4700 1700 4800
Entry Wire Line
	1600 5300 1700 5400
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_B2IN?
U 1 0 5DEE985D
P 2000 4800
AR Path="/5DEE985D" Ref="R_B2IN?"  Part="1" 
AR Path="/5DA750F6/5DEE985D" Ref="R_B2IN?"  Part="1" 
AR Path="/5DD2AD78/5DEE985D" Ref="R_B2IN?"  Part="1" 
F 0 "R_B2IN?" H 1900 4859 59  0000 L BNN
F 1 "1k" H 1900 4670 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 2000 4800 50  0001 C CNN
F 3 "" H 2000 4800 50  0001 C CNN
	1    2000 4800
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_B1IN?
U 1 0 5DEE9864
P 2000 5400
AR Path="/5DEE9864" Ref="R_B1IN?"  Part="1" 
AR Path="/5DA750F6/5DEE9864" Ref="R_B1IN?"  Part="1" 
AR Path="/5DD2AD78/5DEE9864" Ref="R_B1IN?"  Part="1" 
F 0 "R_B1IN?" H 1900 5459 59  0000 L BNN
F 1 "1k" H 1900 5270 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 2000 5400 50  0001 C CNN
F 3 "" H 2000 5400 50  0001 C CNN
	1    2000 5400
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_HEN?
U 1 0 5DEE986B
P 2000 4200
AR Path="/5DEE986B" Ref="R_HEN?"  Part="1" 
AR Path="/5DA750F6/5DEE986B" Ref="R_HEN?"  Part="1" 
AR Path="/5DD2AD78/5DEE986B" Ref="R_HEN?"  Part="1" 
F 0 "R_HEN?" H 2100 4141 59  0000 R TNN
F 1 "1k" H 2100 4330 59  0000 R TNN
F 2 "czujnikiRpi3b:R0805" H 2000 4200 50  0001 C CNN
F 3 "" H 2000 4200 50  0001 C CNN
	1    2000 4200
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_B?
U 1 0 5DEE9872
P 2750 5500
AR Path="/5DEE9872" Ref="OK_B?"  Part="1" 
AR Path="/5DA750F6/5DEE9872" Ref="OK_B?"  Part="1" 
AR Path="/5DD2AD78/5DEE9872" Ref="OK_B?"  Part="1" 
F 0 "OK_B?" H 2475 5725 59  0000 L BNN
F 1 "LTV816S" H 2475 5200 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 2750 5500 50  0001 C CNN
F 3 "" H 2750 5500 50  0001 C CNN
	1    2750 5500
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_B?
U 1 0 5DEE9879
P 2750 4900
AR Path="/5DEE9879" Ref="OK_B?"  Part="1" 
AR Path="/5DA750F6/5DEE9879" Ref="OK_B?"  Part="1" 
AR Path="/5DD2AD78/5DEE9879" Ref="OK_B?"  Part="1" 
F 0 "OK_B?" H 2475 5125 59  0000 L BNN
F 1 "LTV816S" H 2475 4600 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 2750 4900 50  0001 C CNN
F 3 "" H 2750 4900 50  0001 C CNN
	1    2750 4900
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:LTV816S OK_HEN?
U 1 0 5DEE9880
P 2750 4300
AR Path="/5DEE9880" Ref="OK_HEN?"  Part="1" 
AR Path="/5DA750F6/5DEE9880" Ref="OK_HEN?"  Part="1" 
AR Path="/5DD2AD78/5DEE9880" Ref="OK_HEN?"  Part="1" 
F 0 "OK_HEN?" H 2475 4525 59  0000 L BNN
F 1 "LTV816S" H 2475 4000 59  0000 L BNN
F 2 "czujnikiRpi3b:SMD4" H 2750 4300 50  0001 C CNN
F 3 "" H 2750 4300 50  0001 C CNN
	1    2750 4300
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DEE9887
P 2250 4500
AR Path="/5DEE9887" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DEE9887" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DEE9887" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 2250 4500 50  0001 C CNN
F 1 "GND" H 2150 4400 59  0000 L BNN
F 2 "" H 2250 4500 50  0001 C CNN
F 3 "" H 2250 4500 50  0001 C CNN
	1    2250 4500
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_HEN?
U 1 0 5DEE988D
P 3400 4550
AR Path="/5DEE988D" Ref="R_HEN?"  Part="1" 
AR Path="/5DA750F6/5DEE988D" Ref="R_HEN?"  Part="1" 
AR Path="/5DD2AD78/5DEE988D" Ref="R_HEN?"  Part="1" 
F 0 "R_HEN?" H 3250 4609 59  0000 L BNN
F 1 "1k" H 3400 4420 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 3400 4550 50  0001 C CNN
F 3 "" H 3400 4550 50  0001 C CNN
	1    3400 4550
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_B1IN?
U 1 0 5DEE9894
P 3400 5200
AR Path="/5DEE9894" Ref="R_B1IN?"  Part="1" 
AR Path="/5DA750F6/5DEE9894" Ref="R_B1IN?"  Part="1" 
AR Path="/5DD2AD78/5DEE9894" Ref="R_B1IN?"  Part="1" 
F 0 "R_B1IN?" H 3250 5259 59  0000 L BNN
F 1 "1k" H 3400 5070 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 3400 5200 50  0001 C CNN
F 3 "" H 3400 5200 50  0001 C CNN
	1    3400 5200
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 4 0 5DEE989B
P 3350 5000
AR Path="/5DEE989B" Ref="SV?"  Part="4" 
AR Path="/5DA750F6/5DEE989B" Ref="SV?"  Part="4" 
AR Path="/5DD2AD78/5DEE989B" Ref="SV?"  Part="4" 
F 0 "SV?" H 3450 4970 59  0000 L BNN
F 1 "ML10E" H 3340 4885 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 3350 5000 50  0001 C CNN
F 3 "" H 3350 5000 50  0001 C CNN
	4    3350 5000
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 6 0 5DEE98A2
P 3350 5600
AR Path="/5DEE98A2" Ref="SV?"  Part="6" 
AR Path="/5DA750F6/5DEE98A2" Ref="SV?"  Part="6" 
AR Path="/5DD2AD78/5DEE98A2" Ref="SV?"  Part="6" 
F 0 "SV?" H 3450 5570 59  0000 L BNN
F 1 "ML10E" H 3340 5485 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 3350 5600 50  0001 C CNN
F 3 "" H 3350 5600 50  0001 C CNN
	6    3350 5600
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 7 0 5DEE98A9
P 3350 4400
AR Path="/5DEE98A9" Ref="SV?"  Part="7" 
AR Path="/5DA750F6/5DEE98A9" Ref="SV?"  Part="7" 
AR Path="/5DD2AD78/5DEE98A9" Ref="SV?"  Part="7" 
F 0 "SV?" H 3450 4370 59  0000 L BNN
F 1 "ML10E" H 3340 4285 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 3350 4400 50  0001 C CNN
F 3 "" H 3350 4400 50  0001 C CNN
	7    3350 4400
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:ML10E SV?
U 8 0 5DEE98B0
P 3350 4200
AR Path="/5DEE98B0" Ref="SV?"  Part="8" 
AR Path="/5DA750F6/5DEE98B0" Ref="SV?"  Part="8" 
AR Path="/5DD2AD78/5DEE98B0" Ref="SV?"  Part="8" 
F 0 "SV?" H 3450 4170 59  0000 L BNN
F 1 "ML10E" H 3340 4085 59  0000 L BNN
F 2 "czujnikiRpi3b:ML10" H 3350 4200 50  0001 C CNN
F 3 "" H 3350 4200 50  0001 C CNN
	8    3350 4200
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:R-EU_R0805 R_B2IN?
U 1 0 5DEE98B7
P 3250 5800
AR Path="/5DEE98B7" Ref="R_B2IN?"  Part="1" 
AR Path="/5DA750F6/5DEE98B7" Ref="R_B2IN?"  Part="1" 
AR Path="/5DD2AD78/5DEE98B7" Ref="R_B2IN?"  Part="1" 
F 0 "R_B2IN?" H 3150 5859 59  0000 L BNN
F 1 "1k" H 3150 5670 59  0000 L BNN
F 2 "czujnikiRpi3b:R0805" H 3250 5800 50  0001 C CNN
F 3 "" H 3250 5800 50  0001 C CNN
	1    3250 5800
	-1   0    0    1   
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DEE98BE
P 3450 5900
AR Path="/5DEE98BE" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DEE98BE" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DEE98BE" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3450 5900 50  0001 C CNN
F 1 "GND" H 3350 5800 59  0000 L BNN
F 2 "" H 3450 5900 50  0001 C CNN
F 3 "" H 3450 5900 50  0001 C CNN
	1    3450 5900
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DEE98C4
P 3500 5450
AR Path="/5DEE98C4" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DEE98C4" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DEE98C4" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3500 5450 50  0001 C CNN
F 1 "GND" H 3400 5350 59  0000 L BNN
F 2 "" H 3500 5450 50  0001 C CNN
F 3 "" H 3500 5450 50  0001 C CNN
	1    3500 5450
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DEE98CA
P 3500 4800
AR Path="/5DEE98CA" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DEE98CA" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DEE98CA" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 3500 4800 50  0001 C CNN
F 1 "GND" H 3400 4700 59  0000 L BNN
F 2 "" H 3500 4800 50  0001 C CNN
F 3 "" H 3500 4800 50  0001 C CNN
	1    3500 4800
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DEE98D0
P 2250 5100
AR Path="/5DEE98D0" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DEE98D0" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DEE98D0" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 2250 5100 50  0001 C CNN
F 1 "GND" H 2150 5000 59  0000 L BNN
F 2 "" H 2250 5100 50  0001 C CNN
F 3 "" H 2250 5100 50  0001 C CNN
	1    2250 5100
	1    0    0    -1  
$EndComp
$Comp
L czujnikiRpi3b-eagle-import:GND #GND?
U 1 0 5DEE98D6
P 2250 5700
AR Path="/5DEE98D6" Ref="#GND?"  Part="1" 
AR Path="/5DA750F6/5DEE98D6" Ref="#GND?"  Part="1" 
AR Path="/5DD2AD78/5DEE98D6" Ref="#GND?"  Part="1" 
F 0 "#GND?" H 2250 5700 50  0001 C CNN
F 1 "GND" H 2150 5600 59  0000 L BNN
F 2 "" H 2250 5700 50  0001 C CNN
F 3 "" H 2250 5700 50  0001 C CNN
	1    2250 5700
	1    0    0    -1  
$EndComp
Text Notes 1000 4050 0    59   ~ 0
UART/SPI - EXP BUS
Text HLabel 1700 4200 0    50   Input ~ 0
Enable_in
Text HLabel 3400 4200 2    50   Input ~ 0
Enable_out
Text HLabel 1150 4500 0    50   Input ~ 0
PWM_A_in
Text HLabel 1150 4650 0    50   Input ~ 0
PWM_B_in
Text HLabel 1150 4900 0    50   Input ~ 0
A1_in
Text HLabel 1150 5000 0    50   Input ~ 0
A2_in
Text HLabel 1150 5150 0    50   Input ~ 0
B1_in
Text HLabel 1150 5250 0    50   Input ~ 0
B2_in
$EndSCHEMATC
