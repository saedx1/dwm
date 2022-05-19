/* See LICENSE file for copyright and license details. */
#include "layouts.c"

/* appearance */
static const unsigned int borderpx = 0; /* border pixel of windows */
static const unsigned int gappx = 8;	/* gap pixel between windows */
static const unsigned int snap = 32;	/* snap pixel */
static const int user_bh = 10;			/* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int showbar = 1;			/* 0 means no bar */
static const int topbar = 1;			/* 0 means bottom bar */
static const int vertpad = 10;			/* vertical padding of bar */
static const int sidepad = 8;			/* horizontal padding of bar */

static char *fonts[] = {"Roboto:pixelsize=13:antialias=true:autohint=true", "JoyPixels:pixelsize=13:antialias=true:autohint=true", "Noto Sans Arabic UI:pixelsize=20:antialias=true:autohint=true"};
static const char dmenufont[] = "Roboto Mono:pixelsize=13:antialias=true:autohint=true";
static const char colprimary[] = "#FFFFFF";
static const char colsecondary[] = "#282A36";
static const char colborders[] = "#8db5cf";
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = {colprimary, colsecondary, colborders},
	[SchemeSel] = {colprimary, colsecondary, colborders},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const char *tagsel[][2] = {
	{"#ffffff", "#ff0000"},
	{"#ffffff", "#ff7f00"},
	{"#000000", "#ffff00"},
	{"#000000", "#00ff00"},
	{"#ffffff", "#0000ff"},
	{"#ffffff", "#4b0082"},
	{"#ffffff", "#9400d3"},
	{"#000000", "#ffffff"},
	{"#ffffff", "#000000"},
};

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{"Gimp", NULL, NULL, 0, 1, -1},
	{"Microsoft Teams - Preview", NULL, NULL, 1 << 3, 0, -1},
	{"Slack", NULL, NULL, 1 << 3, 0, -1},
	// {"Code", NULL, NULL, 1 << 1, 0, -1},
	{"zoom", NULL, NULL, 1 << 5, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;	  /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[T]", tile}, /* first entry is default */
	{"[F]", NULL}, /* no layout function means floating behavior */
	{"[M]", monocle},
	{"[G]", grid},
};

/* key definitions */
#define ALTMASK Mod1Mask
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", colsecondary, "-nf", colprimary, "-sb", colprimary, "-sf", colsecondary, NULL};
static const char *termcmd[] = {"st", NULL};
static const char *cmdprintscreen[] = {"scrot", "-s", "-f", "/home/saedx1/screenshots/%Y-%m-%d-%s_$wx$h.png", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{MODKEY, XK_p, spawn, {.v = dmenucmd}},
	{MODKEY, XK_space, spawn, {.v = termcmd}},
	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_Up, spawn, SHCMD("pulseaudio-ctl up")},
	{MODKEY, XK_Down, spawn, SHCMD("pulseaudio-ctl down")},
	{0, XK_Print, spawn, {.v = cmdprintscreen}},
	{MODKEY, XK_Right, focusstack, {.i = +1}},
	{MODKEY, XK_Left, focusstack, {.i = -1}},
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_d, incnmaster, {.i = -1}},
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},
	{MODKEY, XK_Return, zoom, {0}},
	{MODKEY, XK_Tab, view, {0}},
	{MODKEY | ShiftMask, XK_c, killclient, {0}},
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
	{MODKEY, XK_g, setlayout, {.v = &layouts[3]}},
	{0, 0, setlayout, {0}},
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},
	{MODKEY, XK_0, view, {.ui = ~0}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
	{MODKEY, XK_n, spawn, SHCMD("$BROWSER")},
	{ALTMASK, XK_Shift_L, spawn, SHCMD("language_switch")},
	{ShiftMask, XK_Alt_L, spawn, SHCMD("language_switch")},
	{MODKEY, XK_s, spawn, SHCMD("sleep_pc")},
	{MODKEY, XK_minus, setgaps, {.i = -1}},
	{MODKEY, XK_equal, setgaps, {.i = +1}},
	{MODKEY | ShiftMask, XK_equal, setgaps, {.i = 0}},
	TAGKEYS(XK_1, 0)
		TAGKEYS(XK_2, 1)
			TAGKEYS(XK_3, 2)
				TAGKEYS(XK_4, 3)
					TAGKEYS(XK_5, 4)
						TAGKEYS(XK_6, 5)
							TAGKEYS(XK_7, 6)
								TAGKEYS(XK_8, 7)
									TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}}

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button1, spawn, SHCMD("st -e htop")},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkStatusText, 0, Button3, spawn, SHCMD("killall activate_status_bar && activate_status_bar &")},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
