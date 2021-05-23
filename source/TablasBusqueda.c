/*
 * TablasBusqueda.c
 *
 *  Created on: 15 dic. 2020
 *      Author: luisd
 */

#include "stdint.h"

// Tabla de búsqueda de (1<<15-1)*sin(x), x E [0,60) pesa 1kB
const uint16_t tabla_sen[512] = {
	    0,    67,   134,   201,   268,   335,   402,   469,   536,   603,   670,   737,   804,   871,   938,  1005,
	 1072,  1139,  1206,  1273,  1340,  1407,  1474,  1541,  1608,  1675,  1742,  1809,  1875,  1942,  2009,  2076,
	 2143,  2210,  2277,  2344,  2410,  2477,  2544,  2611,  2678,  2745,  2811,  2878,  2945,  3012,  3078,  3145,
	 3212,  3278,  3345,  3412,  3478,  3545,  3612,  3678,  3745,  3811,  3878,  3945,  4011,  4078,  4144,  4210,
	 4277,  4343,  4410,  4476,  4543,  4609,  4675,  4742,  4808,  4874,  4940,  5007,  5073,  5139,  5205,  5271,
	 5338,  5404,  5470,  5536,  5602,  5668,  5734,  5800,  5866,  5932,  5998,  6064,  6129,  6195,  6261,  6327,
	 6393,  6458,  6524,  6590,  6655,  6721,  6786,  6852,  6917,  6983,  7048,  7114,  7179,  7245,  7310,  7375,
	 7441,  7506,  7571,  7636,  7701,  7767,  7832,  7897,  7962,  8027,  8092,  8157,  8222,  8286,  8351,  8416,
	 8481,  8545,  8610,  8675,  8739,  8804,  8868,  8933,  8997,  9062,  9126,  9191,  9255,  9319,  9383,  9448,
	 9512,  9576,  9640,  9704,  9768,  9832,  9896,  9960, 10024, 10087, 10151, 10215, 10278, 10342, 10406, 10469,
	10533, 10596, 10659, 10723, 10786, 10849, 10913, 10976, 11039, 11102, 11165, 11228, 11291, 11354, 11417, 11479,
	11542, 11605, 11668, 11730, 11793, 11855, 11918, 11980, 12042, 12105, 12167, 12229, 12291, 12353, 12415, 12477,
	12539, 12601, 12663, 12725, 12787, 12848, 12910, 12972, 13033, 13094, 13156, 13217, 13279, 13340, 13401, 13462,
	13523, 13584, 13645, 13706, 13767, 13828, 13888, 13949, 14010, 14070, 14131, 14191, 14252, 14312, 14372, 14432,
	14492, 14553, 14613, 14673, 14732, 14792, 14852, 14912, 14971, 15031, 15090, 15150, 15209, 15269, 15328, 15387,
	15446, 15505, 15564, 15623, 15682, 15741, 15800, 15858, 15917, 15976, 16034, 16092, 16151, 16209, 16267, 16325,
	16384, 16442, 16499, 16557, 16615, 16673, 16730, 16788, 16846, 16903, 16960, 17018, 17075, 17132, 17189, 17246,
	17303, 17360, 17417, 17474, 17530, 17587, 17643, 17700, 17756, 17812, 17869, 17925, 17981, 18037, 18093, 18149,
	18204, 18260, 18316, 18371, 18427, 18482, 18537, 18593, 18648, 18703, 18758, 18813, 18868, 18922, 18977, 19032,
	19086, 19140, 19195, 19249, 19303, 19357, 19411, 19465, 19519, 19573, 19627, 19680, 19734, 19787, 19841, 19894,
	19947, 20000, 20053, 20106, 20159, 20212, 20265, 20317, 20370, 20422, 20475, 20527, 20579, 20631, 20683, 20735,
	20787, 20839, 20891, 20942, 20994, 21045, 21096, 21148, 21199, 21250, 21301, 21352, 21403, 21453, 21504, 21554,
	21605, 21655, 21705, 21756, 21806, 21856, 21905, 21955, 22005, 22055, 22104, 22154, 22203, 22252, 22301, 22350,
	22399, 22448, 22497, 22546, 22594, 22643, 22691, 22739, 22788, 22836, 22884, 22932, 22979, 23027, 23075, 23122,
	23170, 23217, 23264, 23311, 23359, 23405, 23452, 23499, 23546, 23592, 23639, 23685, 23731, 23778, 23824, 23870,
	23915, 23961, 24007, 24052, 24098, 24143, 24189, 24234, 24279, 24324, 24369, 24413, 24458, 24503, 24547, 24591,
	24636, 24680, 24724, 24768, 24811, 24855, 24899, 24942, 24986, 25029, 25072, 25115, 25158, 25201, 25244, 25287,
	25329, 25372, 25414, 25456, 25498, 25540, 25582, 25624, 25666, 25708, 25749, 25790, 25832, 25873, 25914, 25955,
	25996, 26037, 26077, 26118, 26158, 26198, 26239, 26279, 26319, 26359, 26398, 26438, 26478, 26517, 26556, 26595,
	26635, 26674, 26712, 26751, 26790, 26828, 26867, 26905, 26943, 26981, 27019, 27057, 27095, 27133, 27170, 27207,
	27245, 27282, 27319, 27356, 27393, 27430, 27466, 27503, 27539, 27575, 27611, 27647, 27683, 27719, 27755, 27790,
	27826, 27861, 27896, 27931, 27966, 28001, 28036, 28071, 28105, 28140, 28174, 28208, 28242, 28276, 28310, 28343
};

// Tabla de búsqueda de (1<<15-1)*sin(x), x E [0,90) pesa 1.5kB
const uint16_t tabla_sen90[1024] = {
	    0,    67,   134,   201,   268,   335,   402,   469,   536,   603,   670,   737,   804,   871,   938,  1005,
	 1072,  1139,  1206,  1273,  1340,  1407,  1474,  1541,  1608,  1675,  1742,  1809,  1875,  1942,  2009,  2076,
	 2143,  2210,  2277,  2344,  2410,  2477,  2544,  2611,  2678,  2745,  2811,  2878,  2945,  3012,  3078,  3145,
	 3212,  3278,  3345,  3412,  3478,  3545,  3612,  3678,  3745,  3811,  3878,  3945,  4011,  4078,  4144,  4210,
	 4277,  4343,  4410,  4476,  4543,  4609,  4675,  4742,  4808,  4874,  4940,  5007,  5073,  5139,  5205,  5271,
	 5338,  5404,  5470,  5536,  5602,  5668,  5734,  5800,  5866,  5932,  5998,  6064,  6129,  6195,  6261,  6327,
	 6393,  6458,  6524,  6590,  6655,  6721,  6786,  6852,  6917,  6983,  7048,  7114,  7179,  7245,  7310,  7375,
	 7441,  7506,  7571,  7636,  7701,  7767,  7832,  7897,  7962,  8027,  8092,  8157,  8222,  8286,  8351,  8416,
	 8481,  8545,  8610,  8675,  8739,  8804,  8868,  8933,  8997,  9062,  9126,  9191,  9255,  9319,  9383,  9448,
	 9512,  9576,  9640,  9704,  9768,  9832,  9896,  9960, 10024, 10087, 10151, 10215, 10278, 10342, 10406, 10469,
	10533, 10596, 10659, 10723, 10786, 10849, 10913, 10976, 11039, 11102, 11165, 11228, 11291, 11354, 11417, 11479,
	11542, 11605, 11668, 11730, 11793, 11855, 11918, 11980, 12042, 12105, 12167, 12229, 12291, 12353, 12415, 12477,
	12539, 12601, 12663, 12725, 12787, 12848, 12910, 12972, 13033, 13094, 13156, 13217, 13279, 13340, 13401, 13462,
	13523, 13584, 13645, 13706, 13767, 13828, 13888, 13949, 14010, 14070, 14131, 14191, 14252, 14312, 14372, 14432,
	14492, 14553, 14613, 14673, 14732, 14792, 14852, 14912, 14971, 15031, 15090, 15150, 15209, 15269, 15328, 15387,
	15446, 15505, 15564, 15623, 15682, 15741, 15800, 15858, 15917, 15976, 16034, 16092, 16151, 16209, 16267, 16325,
	16384, 16442, 16499, 16557, 16615, 16673, 16730, 16788, 16846, 16903, 16960, 17018, 17075, 17132, 17189, 17246,
	17303, 17360, 17417, 17474, 17530, 17587, 17643, 17700, 17756, 17812, 17869, 17925, 17981, 18037, 18093, 18149,
	18204, 18260, 18316, 18371, 18427, 18482, 18537, 18593, 18648, 18703, 18758, 18813, 18868, 18922, 18977, 19032,
	19086, 19140, 19195, 19249, 19303, 19357, 19411, 19465, 19519, 19573, 19627, 19680, 19734, 19787, 19841, 19894,
	19947, 20000, 20053, 20106, 20159, 20212, 20265, 20317, 20370, 20422, 20475, 20527, 20579, 20631, 20683, 20735,
	20787, 20839, 20891, 20942, 20994, 21045, 21096, 21148, 21199, 21250, 21301, 21352, 21403, 21453, 21504, 21554,
	21605, 21655, 21705, 21756, 21806, 21856, 21905, 21955, 22005, 22055, 22104, 22154, 22203, 22252, 22301, 22350,
	22399, 22448, 22497, 22546, 22594, 22643, 22691, 22739, 22788, 22836, 22884, 22932, 22979, 23027, 23075, 23122,
	23170, 23217, 23264, 23311, 23359, 23405, 23452, 23499, 23546, 23592, 23639, 23685, 23731, 23778, 23824, 23870,
	23915, 23961, 24007, 24052, 24098, 24143, 24189, 24234, 24279, 24324, 24369, 24413, 24458, 24503, 24547, 24591,
	24636, 24680, 24724, 24768, 24811, 24855, 24899, 24942, 24986, 25029, 25072, 25115, 25158, 25201, 25244, 25287,
	25329, 25372, 25414, 25456, 25498, 25540, 25582, 25624, 25666, 25708, 25749, 25790, 25832, 25873, 25914, 25955,
	25996, 26037, 26077, 26118, 26158, 26198, 26239, 26279, 26319, 26359, 26398, 26438, 26478, 26517, 26556, 26595,
	26635, 26674, 26712, 26751, 26790, 26828, 26867, 26905, 26943, 26981, 27019, 27057, 27095, 27133, 27170, 27207,
	27245, 27282, 27319, 27356, 27393, 27430, 27466, 27503, 27539, 27575, 27611, 27647, 27683, 27719, 27755, 27790,
	27826, 27861, 27896, 27931, 27966, 28001, 28036, 28071, 28105, 28140, 28174, 28208, 28242, 28276, 28310, 28343,
	28377, 28411, 28444, 28477, 28510, 28543, 28576, 28609, 28641, 28674, 28706, 28738, 28771, 28803, 28834, 28866,
	28898, 28929, 28961, 28992, 29023, 29054, 29085, 29116, 29147, 29177, 29208, 29238, 29268, 29298, 29328, 29358,
	29388, 29417, 29447, 29476, 29505, 29534, 29563, 29592, 29621, 29650, 29678, 29706, 29735, 29763, 29791, 29819,
	29846, 29874, 29901, 29929, 29956, 29983, 30010, 30037, 30064, 30090, 30117, 30143, 30169, 30195, 30221, 30247,
	30273, 30298, 30324, 30349, 30374, 30399, 30424, 30449, 30474, 30498, 30523, 30547, 30571, 30595, 30619, 30643,
	30667, 30690, 30714, 30737, 30760, 30783, 30806, 30829, 30852, 30874, 30896, 30919, 30941, 30963, 30985, 31006,
	31028, 31050, 31071, 31092, 31113, 31134, 31155, 31176, 31196, 31217, 31237, 31257, 31277, 31297, 31317, 31337,
	31356, 31375, 31395, 31414, 31433, 31452, 31470, 31489, 31507, 31526, 31544, 31562, 31580, 31598, 31616, 31633,
	31650, 31668, 31685, 31702, 31719, 31736, 31752, 31769, 31785, 31801, 31817, 31833, 31849, 31865, 31880, 31896,
	31911, 31926, 31941, 31956, 31971, 31985, 32000, 32014, 32028, 32043, 32057, 32070, 32084, 32098, 32111, 32124,
	32137, 32150, 32163, 32176, 32189, 32201, 32213, 32226, 32238, 32250, 32261, 32273, 32285, 32296, 32307, 32318,
	32329, 32340, 32351, 32361, 32372, 32382, 32392, 32402, 32412, 32422, 32432, 32441, 32451, 32460, 32469, 32478,
	32487, 32495, 32504, 32512, 32521, 32529, 32537, 32545, 32552, 32560, 32567, 32575, 32582, 32589, 32596, 32603,
	32609, 32616, 32622, 32628, 32634, 32640, 32646, 32652, 32657, 32663, 32668, 32673, 32678, 32683, 32688, 32692,
	32697, 32701, 32705, 32709, 32713, 32717, 32721, 32724, 32728, 32731, 32734, 32737, 32740, 32742, 32745, 32747,
	32749, 32752, 32754, 32755, 32757, 32759, 32760, 32761, 32763, 32764, 32765, 32765, 32766, 32766, 32767, 32767
};


// Código para obtener tablas de búsqueda
/*
import numpy as np
datos_fila = 16
datos_totales = 768
ángulo_final = np.pi/2
theta = 0
for fila in range(int(datos_totales/datos_fila)):
    for dato in range(datos_fila):
        print(f'{32767*np.sin(theta):5.0f}, ', sep='', end='')
        theta = theta + ángulo_final/datos_totales
    print('')
 */
