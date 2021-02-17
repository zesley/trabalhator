/* keyboard_control */

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 512

struct options {
	const char *opt;
	unsigned char val;
};

static struct options kmod[] = {
	{.opt = "-left-ctrl",						.val = 0x01},
	{.opt = "-right-ctrl",						.val = 0x10},
	{.opt = "-left-shift",						.val = 0x02},
	{.opt = "-right-shift",						.val = 0x20},
	{.opt = "-left-alt",						.val = 0x04},
	{.opt = "-right-alt",						.val = 0x40},
	{.opt = "-left-meta",						.val = 0x08},
	{.opt = "-right-meta",						.val = 0x80},
	{.opt = NULL}
};

static struct options kval[] = {
	{.opt = "-a",			 	 				.val = 0x04},	// a -> A
	{.opt = "-b",				 	 			.val = 0x05},	// b -> B
	{.opt = "-c",				 		 		.val = 0x06},	// c -> C
	{.opt = "-d",				 		 		.val = 0x07},	// d -> D
	{.opt = "-e",				 		 		.val = 0x08},	// e -> E
	{.opt = "-f",				 		 		.val = 0x09},	// f -> F
	{.opt = "-g",				 		 		.val = 0x0a},	// g -> G
	{.opt = "-h",				 		 		.val = 0x0b},	// h -> H
	{.opt = "-i",				 	 			.val = 0x0c},	// i -> I
	{.opt = "-j",					 	 		.val = 0x0d},	// j -> J
	{.opt = "-k",			 	 				.val = 0x0e},	// k -> K
	{.opt = "-l",				 		 		.val = 0x0f},	// l -> L
	{.opt = "-m",				 		 		.val = 0x10},	// m -> M
	{.opt = "-n",				 		 		.val = 0x11},	// n -> N
	{.opt = "-o",				 		 		.val = 0x12},	// o -> O
	{.opt = "-p",				 		 		.val = 0x13},	// p -> P
	{.opt = "-q",				 		 		.val = 0x14},	// q -> Q
	{.opt = "-r",				 		 		.val = 0x15},	// r -> R
	{.opt = "-s",				 		 		.val = 0x16},	// s -> S
	{.opt = "-t",				 		 		.val = 0x17},	// t -> T
	{.opt = "-u",				 	 			.val = 0x18},	// u -> U
	{.opt = "-v",				 		 		.val = 0x19},	// v -> V
	{.opt = "-w",				 	 			.val = 0x1a},	// w -> W
	{.opt = "-x",				 		 		.val = 0x1b},	// x -> X
	{.opt = "-y",				 		 		.val = 0x1c},	// y -> Y
	{.opt = "-z",						 		.val = 0x1d},	// z -> Z
	{.opt = "-1",								.val = 0x1e},	// 1 -> !
	{.opt = "-2",								.val = 0x1f},	// 2 -> @
	{.opt = "-3",								.val = 0x20},	// 3 -> #
	{.opt = "-4",								.val = 0x21},	// 4 -> $
	{.opt = "-5",								.val = 0x22},	// 5 -> %
	{.opt = "-6",								.val = 0x23},	// 6 -> ^
	{.opt = "-7",								.val = 0x24},	// 7 -> &
	{.opt = "-8",								.val = 0x25},	// 8 -> *
	{.opt = "-9",								.val = 0x26},	// 9 -> (
	{.opt = "-0",								.val = 0x27},	// 0 -> )
	{.opt = "-return",				 			.val = 0x28},	// Return
	{.opt = "-esc",								.val = 0x29},	// Escape
	{.opt = "-bckspc",				 			.val = 0x2a},	// Backspace
	{.opt = "-tab",								.val = 0x2b},	// Tab
	{.opt = "-spacebar",			 			.val = 0x2c},	// Spacebar
	{.opt = "-dash",					 		.val = 0x2d},	// - -> _
	{.opt = "-equal",							.val = 0x2e},	// = -> +
	{.opt = "-openbracket",	 					.val = 0x2f},	// [ -> {
	{.opt = "-closebracket",					.val = 0x30},	// ] -> }
	{.opt = "-backslach",						.val = 0x31},	// \ -> |
	{.opt = "-semicolon",						.val = 0x33},	// ; -> :
	{.opt = "-quote",							.val = 0x34},	// ' -> "
	{.opt = "-grave",							.val = 0x35},	// ` -> ~
	{.opt = "-comma",							.val = 0x36},	// , -> <
	{.opt = "-dot",								.val = 0x37},	// . -> >
	{.opt = "-slash",							.val = 0x38},	// / -> ?
	{.opt = "-capslock",						.val = 0x39},	// Caps Lock
	{.opt = "-f1",						 		.val = 0x3a},	// F1
	{.opt = "-f2",						 		.val = 0x3b},	// F2
	{.opt = "-f3",						 		.val = 0x3c},	// F3
	{.opt = "-f4",						 		.val = 0x3d},	// F4
	{.opt = "-f5",						 		.val = 0x3e},	// F5
	{.opt = "-f6",						 		.val = 0x3f},	// F6
	{.opt = "-f7",						 		.val = 0x40},	// F7
	{.opt = "-f8",						 		.val = 0x41},	// F8
	{.opt = "-f9",						 		.val = 0x42},	// F9
	{.opt = "-f10",								.val = 0x43},	// F10
	{.opt = "-f11",								.val = 0x44},	// F11
	{.opt = "-f12",								.val = 0x45},	// F12
	{.opt = "-prtscr",							.val = 0x46},	// Print Screen
	{.opt = "-scrlock",				 			.val = 0x47},	// Scroll Lock
	{.opt = "-pause",					 		.val = 0x48},	// Pause
	{.opt = "-insert",				 			.val = 0x49},	// Insert
	{.opt = "-home",					 		.val = 0x4a},	// Home
	{.opt = "-pageup",				 			.val = 0x4b},	// Page Up
	{.opt = "-del",								.val = 0x4c},	// Delete
	{.opt = "-end",								.val = 0x4d},	// End
	{.opt = "-pagedown",			 			.val = 0x4e},	// Page Down
	{.opt = "-right",							.val = 0x4f},	// Right
	{.opt = "-left",					 		.val = 0x50},	// Left
	{.opt = "-down",					 		.val = 0x51},	// Down
	{.opt = "-up",						 		.val = 0x52},	// Up
	{.opt = "-numlock",			 				.val = 0x53},	// Num Lock
	{.opt = "-kpslash",							.val = 0x54},	// Keypad /
	{.opt = "-kpstar",				 			.val = 0x55},	// Keypad *
	{.opt = "-kpminus",							.val = 0x56},	// Keypad -
	{.opt = "-kpplus",				 			.val = 0x57},	// Keypad +
	{.opt = "-kpreturn",			 			.val = 0x58},	// Keypad Return
	{.opt = "-kp1",								.val = 0x59},	// Keypad 1 -> End
	{.opt = "-kp2",								.val = 0x5a}, 	// Keypad 2 -> Down Arrow
	{.opt = "-kp3",								.val = 0x5b},	// Keypad 3 -> PageDn
	{.opt = "-kp4",								.val = 0x5c},	// Keypad 4 -> Left Arrow
	{.opt = "-kp5",								.val = 0x5d}, 	// Keypad 5
	{.opt = "-kp6",								.val = 0x5e}, 	// Keypad 6 -> Right Arrow
	{.opt = "-kp7",								.val = 0x5f}, 	// Keypad 7 -> Home
	{.opt = "-kp8",								.val = 0x60}, 	// Keypad 8 -> Up Arrow
	{.opt = "-kp9",								.val = 0x61}, 	// Keypad 9 -> Page Up
	{.opt = "-kp0",								.val = 0x62}, 	// Keypad 0 -> Insert
	{.opt = "-kpdot",							.val = 0x63}, 	// Keypad . -> Delete
	{.opt = "-alt-backslach",					.val = 0x64}, 	// Non-US \ -> |
	{.opt = "-app",								.val = 0x65}, 	// Application
	{.opt = NULL}
};

int main(int argc, char *argv[]) {
	const char *filename = NULL;
	int fd = 0;
	char buf[BUF_LEN];
	int cmd_len = 0;
	char report[8];
	int typed = 0;
	int hold = 0;
	int key = 0;
	fd_set rfds;
	int retval, i, j;
	struct timeval *tv;
	tv = malloc(sizeof(struct timeval));
	tv->tv_sec = 0;
	tv->tv_usec = 10000;
	unsigned int usecs = 10000;

	if (argc < 3) {
		return 1;
	}

	filename = argv[1];
	
	if ((fd = open(filename, O_RDWR, 0666)) == -1) {
		return 2;
	}

	hold = 0;

	memset(report, 0x0, sizeof(report));

	for (j = 2; j < argc; j++) {
		if (argv[j][0] != '-') {
			continue;
		}

		if (strcmp(argv[j], "-hold") == 0) {
			hold = 1;
			continue;
		}

		for (i = 0; kval[i].opt != NULL; i++) {
			if (strcmp(argv[j], kval[i].opt) == 0) {
				report[2 + key++] = kval[i].val;
				typed++;
				break;
			}
		}

		for (i = 0; kmod[i].opt != NULL; i++) {
			if (strcmp(argv[j], kmod[i].opt) == 0) {
				report[0] = report[0] | kmod[i].val;
				typed++;
				break;
			}
		} 
	}

	if (typed > 0) {
		if (write(fd, report, sizeof(report)) != sizeof(report)) {
			return 3;
		}
		
		if (!hold) {
			memset(report, 0x0, sizeof(report));
			if (write(fd, report, sizeof(report)) != sizeof(report)) {
				return 4;
			}
		}
	}

	usleep(usecs);

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	retval = select(fd + 1, &rfds, NULL, NULL, tv);
	if (retval == -1 && errno == EINTR) {
		return 5;
	}

	if (retval < 0) {
		return 6;
	}

	if (FD_ISSET(fd, &rfds)) {
		cmd_len = read(fd, buf, BUF_LEN - 1);
		for (i = 0; i < cmd_len; i++) {
			printf("%hhx", buf[i]);
		}
	}

	close(fd);
	return 0;
}
