/* See LICENSE file for copyright and license details. */
#include "layouts.c"

/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int gappx = 16;	/* gap pixel between windows */
static const unsigned int snap = 32;	/* snap pixel */
static const int user_bh = 0;			/* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int showbar = 1;			/* 0 means no bar */
static const int topbar = 1;			/* 0 means bottom bar */
static const int vertpad = 10;			/* vertical padding of bar */
static const int sidepad = 20;			/* horizontal padding of bar */

static const char *fonts[] = {"JetBrains Mono:bold:pixelsize=14:antialias=true:autohint=true", "JoyPixels:pixelsize=14:antialias=true:autohint=true", "Traditional Arabic:bold:pixelsize=22:antialias=true:autohint=true"};
static const char dmenufont[] = "JetBrains Mono:pixelsize=14:antialias=true:autohint=true";
static const char colprimary[] = "#FFFFFF";
static const char colsecondary[] = "#282A36";
static const char colaccent[] = "#bd93f9"; /* focused window border   */
static const char coldim[] = "#44475a";	   /* unfocused window border */
static const char *colors[][3] = {
	/*               fg         bg            border   */
	[SchemeNorm] = {colprimary, colsecondary, coldim},
	[SchemeSel] = {colprimary, colsecondary, colaccent},
};

/* tagging */
static const char *tags[] = {"🌐", "📝", "🧨", "4", "5", "6", "7", "8", "9"};

static const char *tagsel[][2] = {
	/*    fg         bg          Dracula palette - cohesive but per-tag */
	{"#282A36", "#8be9fd"}, /* 1  cyan   */
	{"#282A36", "#50fa7b"}, /* 2  green  */
	{"#282A36", "#f1fa8c"}, /* 3  yellow */
	{"#282A36", "#ffb86c"}, /* 4  orange */
	{"#282A36", "#ff5555"}, /* 5  red    */
	{"#282A36", "#ff79c6"}, /* 6  pink   */
	{"#282A36", "#bd93f9"}, /* 7  purple */
	{"#f8f8f2", "#6272a4"}, /* 8  slate  */
	{"#282A36", "#f8f8f2"}, /* 9  white  */
};

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{"Gimp", NULL, NULL, 0, 1, -1},
	// {"Microsoft Teams - Preview", NULL, NULL, 1 << 3, 0, -1},
	// {"Slack", NULL, NULL, 1 << 3, 0, -1},
	// {"Code", NULL, NULL, 1 << 1, 0, -1},
	// {"zoom", NULL, NULL, 1 << 5, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;	  /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[M]", monocle},
	{"[T]", tile}, /* first entry is default */
	{"[F]", NULL}, /* no layout function means floating behavior */
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

static Key keys[] = {
	/* modifier                     key        function        argument */
	{MODKEY, XK_p, spawn, {.v = dmenucmd}},
	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY | ShiftMask, XK_x, spawn, SHCMD("slock")},
	{ALTMASK | ShiftMask, XK_Tab, focusstack, {.i = -1}},
	{ALTMASK, XK_Tab, focusstack, {.i = 1}},
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_d, incnmaster, {.i = -1}},
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},
	{MODKEY, XK_Return, zoom, {0}},
	{MODKEY, XK_Tab, view, {0}},
	{MODKEY | ShiftMask, XK_c, killclient, {0}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_t, setlayout, {.v = &layouts[1]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[2]}},
	{MODKEY, XK_g, setlayout, {.v = &layouts[3]}},
	{0, 0, setlayout, {0}},
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},
	{MODKEY, XK_0, view, {.ui = ~0}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
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
									TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}}};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button1, spawn, SHCMD("st -e htop")},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
