/* Copyright (C) 2021 @filterpaper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Graphical bongocat animation, driven by key press timer or WPM.
   It has left and right aligned cats optimized for both OLEDs.
   This code uses space-saving differential pixels, by rendering a
   base frame following by only changed pixels on subsequent frames.

   Inspired by @j-inc's bongocat animation code
   (keyboards/kyria/keymaps/j-inc)

   Cat images courtesy of @plandevida

   Modified from @Dake's Modular Bongo Cat
   (https://github.com/Dakes/kyria/blob/main/keymaps/dakes/)
*/

#include "filterpaper.h"

#define IDLE_FRAMES 5
#define TAP_FRAMES 2
#define FRAME_DURATION 200 // Number of ms per frame
#define WIDTH OLED_DISPLAY_WIDTH // 128x32px


// Base animation frame that all subsequent ones will differ by pixels
#ifndef LEFTCAT
static uint_fast16_t const base[] PROGMEM = {192, 32840, 32841, 32967, 32970, 33094, 33099, 33221, 33227, 33345, 33346, 33347, 33348, 33356, 33469, 33470, 33471, 33472, 33484, 33595, 33596, 33613, 33715, 33716, 33722, 33742, 33789, 33790, 33791, 33842, 33845, 33846, 33849, 33871, 33872, 33913, 33914, 33915, 33916, 33969, 33975, 33976, 34001, 34037, 34038, 34039, 34040, 34097, 34130, 34160, 34161, 34162, 34163, 34164, 34225, 34259, 34283, 34284, 34285, 34286, 34287, 34353, 34379, 34380, 34388, 34407, 34408, 34409, 34410, 34482, 34507, 34508, 34517, 34531, 34532, 34533, 34534, 34610, 34646, 34655, 34656, 34657, 34658, 34738, 34750, 34751, 34761, 34775, 34779, 34780, 34781, 34782, 34867, 34878, 34879, 34883, 34886, 34887, 34888, 34889, 34903, 34904, 34905, 34906, 34995, 35012, 35013, 35032, 35122, 35150, 35151, 35160, 35250, 35273, 35274, 35275, 35276, 35277, 35280, 35281, 35288, 35377, 35397, 35398, 35399, 35400, 35410, 35411, 35412, 35413, 35414, 35415, 35505, 35522, 35523, 35524, 35633, 35649, 35760, 35778, 35888, 35891, 35892, 35893, 35894, 35895, 35896, 35906, 36015, 36016, 36017, 36018, 36025, 36026, 36034, 36138, 36139, 36140, 36141, 36142, 36155, 36156, 36162, 36261, 36262, 36263, 36264, 36265, 36285, 36286, 36287, 36288, 36289, 36384, 36385, 36386, 36387, 36388, 36507, 36508, 36509, 36510, 36511, 36630, 36631, 36632, 36633, 36634, 36753, 36754, 36755, 36756, 36757};
#endif
#ifndef RIGHTCAT
static uint_fast16_t const left_base[] PROGMEM = {192, 32822, 32823, 32949, 32952, 33076, 33081, 33204, 33210, 33331, 33339, 33340, 33341, 33342, 33459, 33471, 33472, 33473, 33474, 33586, 33603, 33604, 33664, 33665, 33666, 33713, 33733, 33739, 33740, 33795, 33796, 33797, 33798, 33839, 33840, 33862, 33865, 33866, 33869, 33927, 33928, 33929, 33930, 33966, 33991, 33992, 33998, 34059, 34060, 34061, 34062, 34063, 34093, 34126, 34192, 34193, 34194, 34195, 34196, 34220, 34254, 34325, 34326, 34327, 34328, 34347, 34355, 34356, 34382, 34457, 34458, 34459, 34460, 34474, 34483, 34484, 34509, 34589, 34590, 34591, 34592, 34601, 34637, 34721, 34722, 34723, 34724, 34728, 34742, 34752, 34753, 34765, 34853, 34854, 34855, 34856, 34870, 34871, 34872, 34873, 34876, 34880, 34881, 34892, 34983, 35002, 35003, 35020, 35111, 35120, 35121, 35149, 35239, 35246, 35247, 35250, 35251, 35252, 35253, 35254, 35277, 35368, 35369, 35370, 35371, 35372, 35373, 35383, 35384, 35385, 35386, 35406, 35515, 35516, 35517, 35534, 35646, 35662, 35773, 35791, 35901, 35911, 35912, 35913, 35914, 35915, 35916, 35919, 36029, 36037, 36038, 36045, 36046, 36047, 36048, 36157, 36163, 36164, 36177, 36178, 36179, 36180, 36181, 36286, 36287, 36288, 36289, 36290, 36310, 36311, 36312, 36313, 36314, 36443, 36444, 36445, 36446, 36447, 36576, 36577, 36578, 36579, 36580, 36709, 36710, 36711, 36712, 36713, 36842, 36843, 36844, 36845, 36846};
#endif


// Loop through array to render changed OLED pixel bits
static void render_array(uint_fast16_t const *frame) {
	// First frame array element is the size
	uint_fast16_t const size = pgm_read_word(&(frame[0]));
	for (uint_fast16_t i = size; i !=0; --i) {
		uint_fast16_t cur_px = pgm_read_word(&(frame[i]));
		// Get pixel state bit
		bool const on = (cur_px & ( 1 << 15 )) >> 15;
		// Remove pixel state bit
		cur_px &= ~(1UL << 15);
		oled_write_pixel(cur_px % WIDTH, cur_px / WIDTH, on);
	}
}


static void render_cat_idle(void) {
	// Differential idle frames
#ifndef LEFTCAT
	static uint_fast16_t const idle_diff_0[] PROGMEM = {0};
	static uint_fast16_t const idle_diff_1[] PROGMEM = {105, 72, 73, 199, 32968, 32969, 202, 326, 33095, 33098, 331, 453, 33222, 577, 578, 579, 580, 33349, 33355, 588, 701, 702, 703, 704, 33473, 33474, 33475, 33476, 827, 828, 33597, 33598, 33599, 33600, 33612, 845, 947, 948, 954, 33723, 33724, 33741, 974, 1074, 33843, 33844, 1077, 1078, 1081, 33850, 33870, 1103, 1104, 1201, 33970, 33973, 33974, 1207, 1208, 33977, 33999, 34000, 1233, 34103, 34104, 34129, 1362, 34258, 1491, 1611, 1612, 34387, 1620, 34481, 1714, 34516, 1749, 34635, 34636, 34645, 1878, 1982, 1983, 1993, 34774, 2007, 34866, 2099, 2115, 2118, 2119, 2120, 35006, 35007, 35011, 2244, 2245, 35014, 35015, 35016, 35017, 2354, 35123, 35140, 35141, 35378};
	static uint_fast16_t const idle_diff_2[] PROGMEM = {31, 72, 73, 199, 202, 326, 33096, 33097, 33098, 331, 453, 33222, 33223, 33349, 947, 948, 1074, 1077, 1078, 1201, 33971, 33972, 33973, 33974, 1329, 34098, 1970, 34739, 35249, 2482, 35632, 2865};
	static uint_fast16_t const idle_diff_3[] PROGMEM = {21, 72, 73, 199, 32968, 32969, 326, 33095, 33222, 459, 33228, 947, 948, 1074, 33843, 33844, 1077, 1078, 1201, 33970, 33973, 33974};
	static uint_fast16_t const *idle_diff[IDLE_FRAMES] = {
		idle_diff_0,
		idle_diff_0, // First two frames are identical
		idle_diff_1,
		idle_diff_2,
		idle_diff_3
	};
#endif
#ifndef RIGHTCAT
	static uint_fast16_t const left_idle_diff_0[] PROGMEM = {0};
	static uint_fast16_t const left_idle_diff_1[] PROGMEM = {105, 54, 55, 181, 32950, 32951, 184, 308, 33077, 33080, 313, 33209, 442, 563, 33332, 33338, 571, 572, 573, 574, 33467, 33468, 33469, 33470, 703, 704, 705, 706, 818, 33587, 33599, 33600, 33601, 33602, 835, 836, 945, 33714, 33731, 33732, 965, 971, 972, 1071, 1072, 33841, 33861, 1094, 1097, 1098, 33867, 33868, 1101, 1198, 33967, 33968, 33990, 1223, 1224, 33993, 33994, 33997, 1230, 1325, 34094, 34119, 34120, 1452, 34221, 1579, 34348, 1587, 1588, 1706, 34475, 1741, 34510, 1833, 34602, 34611, 34612, 1960, 34729, 1974, 1984, 1985, 2103, 2104, 2105, 2108, 2124, 34893, 34998, 34999, 35000, 35001, 2234, 2235, 35004, 35008, 35009, 35130, 35131, 35148, 2381, 35405};
	static uint_fast16_t const left_idle_diff_2[] PROGMEM = {31, 54, 55, 181, 184, 308, 33077, 33078, 33079, 313, 33208, 33209, 442, 33338, 971, 972, 1097, 1098, 1101, 33993, 33994, 33995, 33996, 1230, 34125, 1358, 34764, 1997, 2509, 35278, 2894, 35663};
	static uint_fast16_t const left_idle_diff_3[] PROGMEM = {21, 54, 55, 32950, 32951, 184, 33080, 313, 33203, 436, 33209, 971, 972, 1097, 1098, 33867, 33868, 1101, 33993, 33994, 33997, 1230};
	static uint_fast16_t const *left_idle_diff[IDLE_FRAMES] = {
		left_idle_diff_0,
		left_idle_diff_0, // First two frames are identical
		left_idle_diff_1,
		left_idle_diff_2,
		left_idle_diff_3
	};
#endif
	static uint_fast8_t current_frame = 0;
	current_frame = (current_frame + 1) % IDLE_FRAMES;

	if (is_keyboard_left()) {
	#ifndef RIGHTCAT
		render_array(left_base);
		render_array(left_idle_diff[current_frame]);
	#endif
	} else {
	#ifndef LEFTCAT
		render_array(base);
		render_array(idle_diff[current_frame]);
	#endif
	}
}


static void render_cat_prep(void) {
	// Differential prep frame
#ifndef LEFTCAT
	static uint_fast16_t const prep_diff_0[] PROGMEM = {82, 33875, 33876, 33877, 33878, 34002, 34007, 34133, 34136, 34257, 34263, 34264, 34385, 34389, 34392, 34512, 34516, 34520, 34640, 1878, 34648, 34745, 34746, 34747, 34768, 34872, 34874, 34876, 34999, 35004, 35028, 35029, 35030, 2264, 35127, 35128, 35131, 35133, 35152, 35153, 35154, 35155, 2392, 35254, 35257, 35258, 35261, 2512, 2513, 2520, 35382, 35385, 35386, 35389, 2642, 2643, 2644, 2645, 2646, 2647, 35517, 35644, 35645, 35646, 35647, 35648, 35769, 35770, 35771, 35772, 3010, 3138, 3257, 3258, 3266, 3387, 3388, 3394, 3517, 3518, 3519, 3520, 3521};
	static uint_fast16_t const *prep_diff[] = {
		prep_diff_0
	};
#endif
#ifndef RIGHTCAT
	static uint_fast16_t const left_prep_diff_0[] PROGMEM = {82, 33833, 33834, 33835, 33836, 33960, 33965, 34087, 34090, 34215, 34216, 34222, 34343, 34346, 34350, 34471, 34475, 34479, 34599, 1833, 34607, 34735, 34756, 34757, 34758, 34883, 34885, 34887, 2215, 34985, 34986, 34987, 35011, 35016, 2343, 35116, 35117, 35118, 35119, 35138, 35140, 35143, 35144, 2471, 2478, 2479, 35266, 35269, 35270, 35273, 2600, 2601, 2602, 2603, 2604, 2605, 35394, 35397, 35398, 35401, 35522, 35647, 35648, 35649, 35650, 35651, 3005, 35779, 35780, 35781, 35782, 3133, 3261, 3269, 3270, 3389, 3395, 3396, 3518, 3519, 3520, 3521, 3522};
	static uint_fast16_t const *left_prep_diff[] = {
		left_prep_diff_0
	};
#endif

	if (is_keyboard_left()) {
	#ifndef RIGHTCAT
		render_array(left_base);
		render_array(left_prep_diff[0]);
	#endif
	} else {
	#ifndef LEFTCAT
		render_array(base);
		render_array(prep_diff[0]);
	#endif
	}
}


static void render_cat_tap(void) {
	// Differential tap frames
#ifndef LEFTCAT
	static uint_fast16_t const tap_diff_0[] PROGMEM = {156, 33238, 33239, 33240, 33241, 33366, 33367, 33368, 33369, 33494, 33495, 33496, 33497, 33622, 33623, 33624, 33625, 33750, 33751, 33752, 33753, 33759, 33760, 33878, 33879, 33880, 33881, 33886, 33887, 33888, 33889, 34006, 34007, 34008, 34013, 34014, 34015, 34016, 34017, 34135, 34136, 34140, 34141, 34142, 34143, 34144, 34145, 34146, 34267, 34268, 34269, 34270, 34271, 34272, 34273, 34274, 34395, 34396, 34397, 34398, 34399, 34400, 34523, 34524, 34525, 34745, 34746, 34747, 34872, 34874, 34876, 34999, 35004, 35127, 35128, 35131, 35133, 35162, 35163, 35164, 35165, 35166, 35167, 35168, 35169, 35170, 35254, 35257, 35258, 35261, 35290, 35291, 35292, 35293, 35294, 35295, 35296, 35297, 35298, 35382, 35385, 35386, 35389, 35418, 35419, 35420, 35421, 35422, 35423, 35424, 35425, 35517, 35546, 35547, 35548, 35549, 35550, 35551, 35552, 35553, 35644, 35645, 35646, 35647, 35648, 35675, 35676, 35677, 35678, 35679, 35680, 35769, 35770, 35771, 3010, 35804, 35805, 35806, 35807, 35808, 3138, 35933, 35934, 35935, 3257, 3258, 3266, 36062, 36063, 3387, 3388, 3394, 3517, 3518, 3519, 3520, 3521};
	static uint_fast16_t const tap_diff_1[] PROGMEM = {72, 33875, 33876, 33877, 33878, 34002, 34007, 34133, 34136, 34257, 34263, 34264, 34385, 34389, 34392, 34512, 34516, 34520, 34640, 1878, 34648, 34768, 35028, 35029, 35030, 2264, 35152, 35153, 35154, 35155, 2392, 2512, 2513, 2520, 2642, 2643, 2644, 2645, 2646, 2647, 36273, 36274, 36275, 36276, 36277, 36278, 36279, 36401, 36402, 36403, 36404, 36405, 36406, 36407, 36408, 36529, 36530, 36531, 36532, 36533, 36534, 36535, 36536, 36657, 36658, 36659, 36660, 36661, 36662, 36800, 36801, 36802, 36803};
	static uint_fast16_t const *tap_diff[TAP_FRAMES] = {
		tap_diff_0,
		tap_diff_1
	};
#endif
#ifndef RIGHTCAT
	static uint_fast16_t const left_tap_diff_0[] PROGMEM = {156, 33190, 33191, 33192, 33193, 33318, 33319, 33320, 33321, 33446, 33447, 33448, 33449, 33574, 33575, 33576, 33577, 33695, 33696, 33702, 33703, 33704, 33705, 33822, 33823, 33824, 33825, 33830, 33831, 33832, 33833, 33950, 33951, 33952, 33953, 33954, 33959, 33960, 33961, 34077, 34078, 34079, 34080, 34081, 34082, 34083, 34087, 34088, 34205, 34206, 34207, 34208, 34209, 34210, 34211, 34212, 34335, 34336, 34337, 34338, 34339, 34340, 34466, 34467, 34468, 34756, 34757, 34758, 34883, 34885, 34887, 35011, 35016, 35101, 35102, 35103, 35104, 35105, 35106, 35107, 35108, 35109, 35138, 35140, 35143, 35144, 35229, 35230, 35231, 35232, 35233, 35234, 35235, 35236, 35237, 35266, 35269, 35270, 35273, 35358, 35359, 35360, 35361, 35362, 35363, 35364, 35365, 35394, 35397, 35398, 35401, 35486, 35487, 35488, 35489, 35490, 35491, 35492, 35493, 35522, 35615, 35616, 35617, 35618, 35619, 35620, 35647, 35648, 35649, 35650, 35651, 35743, 35744, 35745, 35746, 35747, 3005, 35780, 35781, 35782, 35872, 35873, 35874, 3133, 36000, 36001, 3261, 3269, 3270, 3389, 3395, 3396, 3518, 3519, 3520, 3521, 3522};
	static uint_fast16_t const left_tap_diff_1[] PROGMEM = {72, 33833, 33834, 33835, 33836, 33960, 33965, 34087, 34090, 34215, 34216, 34222, 34343, 34346, 34350, 34471, 34475, 34479, 34599, 1833, 34607, 34735, 2215, 34985, 34986, 34987, 2343, 35116, 35117, 35118, 35119, 2471, 2478, 2479, 2600, 2601, 2602, 2603, 2604, 2605, 36296, 36297, 36298, 36299, 36300, 36301, 36302, 36423, 36424, 36425, 36426, 36427, 36428, 36429, 36430, 36551, 36552, 36553, 36554, 36555, 36556, 36557, 36558, 36681, 36682, 36683, 36684, 36685, 36686, 36796, 36797, 36798, 36799};
	static uint_fast16_t const *left_tap_diff[TAP_FRAMES] = {
		left_tap_diff_0,
		left_tap_diff_1
	};
#endif
	static uint_fast8_t current_frame = 0;
	current_frame = (current_frame + 1) % TAP_FRAMES;

	if (is_keyboard_left()) {
	#ifndef RIGHTCAT
		render_array(left_base);
		render_array(left_tap_diff[current_frame]);
	#endif
	} else {
	#ifndef LEFTCAT
		render_array(base);
		render_array(tap_diff[current_frame]);
	#endif
	}
}


void render_bongocat(void) {
#ifdef WPM_ENABLE
	// WPM triggered typing timer
	static uint_fast8_t prev_wpm = 0;
	static uint_fast32_t key_timer = 0;

	if (get_current_wpm() >prev_wpm) { key_timer = timer_read32(); }
	prev_wpm = get_current_wpm();
#else
	// process_record_user() triggered typing timer
	extern uint_fast32_t key_timer;
#endif

	static uint_fast16_t anim_timer = 0;
	uint_fast32_t elapsed_time = timer_elapsed32(key_timer);

	void animation_phase(void) {
		oled_clear();
		if (elapsed_time <FRAME_DURATION*3) { render_cat_tap(); }
		else if (elapsed_time <FRAME_DURATION*10) { render_cat_prep(); }
		else { render_cat_idle(); }
	}

	if (elapsed_time >OLED_TIMEOUT) {
		oled_off();
	} else if (timer_elapsed(anim_timer) >FRAME_DURATION) {
		anim_timer = timer_read();
		animation_phase();
	}
}
