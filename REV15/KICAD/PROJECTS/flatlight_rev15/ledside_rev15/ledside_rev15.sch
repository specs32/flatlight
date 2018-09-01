EESchema Schematic File Version 4
LIBS:ledside_rev15-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:LED D1
U 1 1 5B7177F2
P 1500 1950
F 0 "D1" V 1538 1833 50  0000 R CNN
F 1 "LED" V 1447 1833 50  0000 R CNN
F 2 "lt_foots:LED_osram_oslon_ssl_powerled" H 1500 1950 50  0001 C CNN
F 3 "~" H 1500 1950 50  0001 C CNN
	1    1500 1950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 5B71BAAA
P 1800 1000
F 0 "R1" H 1870 1046 50  0000 L CNN
F 1 "R" H 1870 955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1730 1000 50  0001 C CNN
F 3 "~" H 1800 1000 50  0001 C CNN
	1    1800 1000
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP1
U 1 1 5B71BAEF
P 2550 950
F 0 "TP1" V 2750 1150 50  0000 C CNN
F 1 "VCC" V 2650 1250 50  0000 C CNN
F 2 "lt_foots:1_pad_smd" H 2750 950 50  0001 C CNN
F 3 "~" H 2750 950 50  0001 C CNN
	1    2550 950 
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5B71BBFB
P 1150 1000
F 0 "J1" H 1256 1178 50  0000 C CNN
F 1 "Conn_01x02_Male" H 1256 1087 50  0000 C CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x02_P2.00mm_Vertical" H 1150 1000 50  0001 C CNN
F 3 "~" H 1150 1000 50  0001 C CNN
	1    1150 1000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad MH2
U 1 1 5B71BCEB
P 4200 900
F 0 "MH2" H 3950 950 50  0000 L CNN
F 1 "GND" H 3950 850 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 4200 900 50  0001 C CNN
F 3 "~" H 4200 900 50  0001 C CNN
	1    4200 900 
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR01
U 1 1 5B71BD67
P 1500 1800
F 0 "#PWR01" H 1500 1650 50  0001 C CNN
F 1 "VCC" H 1517 1973 50  0000 C CNN
F 2 "" H 1500 1800 50  0001 C CNN
F 3 "" H 1500 1800 50  0001 C CNN
	1    1500 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5B71BDD7
P 1500 2100
F 0 "#PWR02" H 1500 1850 50  0001 C CNN
F 1 "GND" H 1505 1927 50  0000 C CNN
F 2 "" H 1500 2100 50  0001 C CNN
F 3 "" H 1500 2100 50  0001 C CNN
	1    1500 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 850  1350 850 
Wire Wire Line
	1350 850  1350 1000
Wire Wire Line
	1800 1150 1350 1150
Wire Wire Line
	1350 1150 1350 1100
$Comp
L Connector:TestPoint TP2
U 1 1 5B71C184
P 3250 1000
F 0 "TP2" V 3450 1200 50  0000 C CNN
F 1 "SOURCE" V 3350 1300 50  0000 C CNN
F 2 "lt_foots:1_pad_smd" H 3450 1000 50  0001 C CNN
F 3 "~" H 3450 1000 50  0001 C CNN
	1    3250 1000
	0    1    1    0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5B71C1C6
P 1850 1950
F 0 "D2" V 1888 1833 50  0000 R CNN
F 1 "LED" V 1797 1833 50  0000 R CNN
F 2 "lt_foots:LED_osram_oslon_ssl_powerled" H 1850 1950 50  0001 C CNN
F 3 "~" H 1850 1950 50  0001 C CNN
	1    1850 1950
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D3
U 1 1 5B71C216
P 2200 1950
F 0 "D3" V 2238 1833 50  0000 R CNN
F 1 "LED" V 2147 1833 50  0000 R CNN
F 2 "lt_foots:LED_osram_oslon_ssl_powerled" H 2200 1950 50  0001 C CNN
F 3 "~" H 2200 1950 50  0001 C CNN
	1    2200 1950
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D4
U 1 1 5B71C21C
P 2550 1950
F 0 "D4" V 2588 1833 50  0000 R CNN
F 1 "LED" V 2497 1833 50  0000 R CNN
F 2 "lt_foots:LED_osram_oslon_ssl_powerled" H 2550 1950 50  0001 C CNN
F 3 "~" H 2550 1950 50  0001 C CNN
	1    2550 1950
	0    -1   -1   0   
$EndComp
Connection ~ 1500 1800
Wire Wire Line
	1500 1800 1400 1800
Connection ~ 1850 1800
Wire Wire Line
	1850 1800 1500 1800
Connection ~ 2200 1800
Wire Wire Line
	2200 1800 1850 1800
Wire Wire Line
	2550 1800 2200 1800
Connection ~ 1500 2100
Wire Wire Line
	1500 2100 1400 2100
Connection ~ 1850 2100
Wire Wire Line
	1850 2100 1500 2100
Connection ~ 2200 2100
Wire Wire Line
	2200 2100 1850 2100
Wire Wire Line
	2550 2100 2200 2100
$Comp
L Mechanical:MountingHole_Pad MH3
U 1 1 5B71C94B
P 3250 900
F 0 "MH3" H 2950 1000 50  0000 L CNN
F 1 "SOURCE" H 2800 900 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 3250 900 50  0001 C CNN
F 3 "~" H 3250 900 50  0001 C CNN
	1    3250 900 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad MH4
U 1 1 5B71C99D
P 4500 900
F 0 "MH4" H 4600 951 50  0000 L CNN
F 1 "GND" H 4600 860 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 4500 900 50  0001 C CNN
F 3 "~" H 4500 900 50  0001 C CNN
	1    4500 900 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad MH1
U 1 1 5B71CA67
P 2550 1050
F 0 "MH1" H 2450 1008 50  0000 R CNN
F 1 "VCC" H 2450 1099 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 2550 1050 50  0001 C CNN
F 3 "~" H 2550 1050 50  0001 C CNN
	1    2550 1050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5B71CC88
P 3250 1000
F 0 "#PWR05" H 3250 750 50  0001 C CNN
F 1 "GND" H 3255 827 50  0000 C CNN
F 2 "" H 3250 1000 50  0001 C CNN
F 3 "" H 3250 1000 50  0001 C CNN
	1    3250 1000
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR03
U 1 1 5B71CCEC
P 2550 950
F 0 "#PWR03" H 2550 800 50  0001 C CNN
F 1 "VCC" H 2567 1123 50  0000 C CNN
F 2 "" H 2550 950 50  0001 C CNN
F 3 "" H 2550 950 50  0001 C CNN
	1    2550 950 
	1    0    0    -1  
$EndComp
Connection ~ 2550 950 
Connection ~ 3250 1000
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5B71EB07
P 1400 1800
F 0 "#FLG0101" H 1400 1875 50  0001 C CNN
F 1 "PWR_FLAG" V 1400 1928 50  0000 L CNN
F 2 "" H 1400 1800 50  0001 C CNN
F 3 "~" H 1400 1800 50  0001 C CNN
	1    1400 1800
	0    -1   -1   0   
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5B71EB5A
P 1400 2100
F 0 "#FLG0102" H 1400 2175 50  0001 C CNN
F 1 "PWR_FLAG" V 1400 2228 50  0000 L CNN
F 2 "" H 1400 2100 50  0001 C CNN
F 3 "~" H 1400 2100 50  0001 C CNN
	1    1400 2100
	0    -1   -1   0   
$EndComp
NoConn ~ 4500 1000
NoConn ~ 4200 1000
Text GLabel 4050 1950 0    50   Input ~ 0
therm
$Comp
L Connector:TestPoint TP3
U 1 1 5B86EAE0
P 4050 1950
F 0 "TP3" V 4250 2150 50  0000 C CNN
F 1 "THERM" V 4150 2250 50  0000 C CNN
F 2 "lt_foots:1_pad_smd" H 4250 1950 50  0001 C CNN
F 3 "~" H 4250 1950 50  0001 C CNN
	1    4050 1950
	0    1    1    0   
$EndComp
$EndSCHEMATC
