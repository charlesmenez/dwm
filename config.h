/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx = 2; /* border pixel of windows */
static unsigned int snap = 0;     /* snap pixel */
static unsigned int gappih = 20;  /* horiz inner gap between windows */
static unsigned int gappiv = 20;  /* vert inner gap between windows */
static unsigned int gappoh =
    20; /* horiz outer gap between windows and screen edge */
static unsigned int gappov =
    20; /* vert outer gap between windows and screen edge */
static int swallowfloating =
    0; /* 1 means swallow floating windows by default */
static int smartgaps =
    0;                  /* 1 means no outer gap when there is only one window */
static int showbar = 1; /* 0 means no bar */
static int topbar = 1;  /* 0 means bottom bar */
static char *fonts[] = {
    "monospace:size=8:autohint=true",
    "NotoColorEmoji:pixelsize=8:antialias=true:autohint=true"};
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#770000";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL};
const char *spcmd2[] = {TERMINAL, "-n",    "spcalc", "-f", "monospace:size=16",
                        "-g",     "50x20", "-e",     "bc", "-lq",
                        NULL};
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm", spcmd1},
    {"spcalc", spcmd2},
};

/* tagging */
static const char *tags[] = {"one", "two",   "three", "four", "five",
                             "six", "seven", "eight", "nine"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class    instance      title       	 tags mask    isfloating
       isterminal  noswallow  monitor */
    /*{"Gimp", NULL, NULL, 1 << 8, 0, 0, 0, -1},*/
    {TERMCLASS, NULL, NULL, 0, 0, 1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1},
    {TERMCLASS, "bg", NULL, 1 << 7, 0, 1, 0, -1},
    {TERMCLASS, "spterm", NULL, SPTAG(0), 1, 1, 0, -1},
    {TERMCLASS, "spcalc", NULL, SPTAG(1), 1, 1, 0, -1},
};

/* layout(s) */
static float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static int nmaster = 2;     /* number of clients in master area */
static int resizehints = 0; /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[@]", spiral},         /* Fibonacci spiral */
    {"|M|", centeredmaster}, /* Master in middle, slaves on sides */
    {"[]=", tile},           /* Default: Master on left, slaves on right */
    {"[\\]", dwindle},       /* Decreasing in size right and leftward */
    {"><>", NULL},           /* no layout function means floating behavior */
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD, XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
      {MOD, XK_k, ACTION##stack, {.i = INC(-1)}},                              \
      {MOD,                                                                    \
       XK_v,                                                                   \
       ACTION##stack,                                                          \
       {.i = 0}}, /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
                  /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
                  /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
                  /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static const char *termcmd[] = {TERMINAL, NULL};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    {"color0", STRING, &normbordercolor},
    {"color14", STRING, &selbordercolor},
    {"color0", STRING, &normbgcolor},
    {"color4", STRING, &normfgcolor},
    {"color0", STRING, &selfgcolor},
    {"color4", STRING, &selbgcolor},
    {"borderpx", INTEGER, &borderpx},
    {"snap", INTEGER, &snap},
    {"showbar", INTEGER, &showbar},
    {"topbar", INTEGER, &topbar},
    {"nmaster", INTEGER, &nmaster},
    {"resizehints", INTEGER, &resizehints},
    {"mfact", FLOAT, &mfact},
    {"gappih", INTEGER, &gappih},
    {"gappiv", INTEGER, &gappiv},
    {"gappoh", INTEGER, &gappoh},
    {"gappov", INTEGER, &gappov},
    {"swallowfloating", INTEGER, &swallowfloating},
    {"smartgaps", INTEGER, &smartgaps},
};

#include "shiftview.c"
#include <X11/XF86keysym.h>

static Key keys[] = {
    /* modifier                     key        function        argument */
    STACKKEYS(MODKEY, focus) STACKKEYS(MODKEY | ShiftMask, push)
    /* { MODKEY|ShiftMask,		XK_Escape,	spawn,	SHCMD("") }, */
    {MODKEY, XK_e, spawn, {.v = (const char *[]){"dmenu-emoji", NULL}}},
    {MODKEY, XK_q, spawn, {.v = (const char *[]){"pkill", "dwm", NULL}}},
    {MODKEY | ShiftMask,
     XK_e,
     spawn,
     {.v = (const char *[]){"dwm-colors", NULL}}},

    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY, XK_0, view, {.ui = ~0}},

    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

    {MODKEY, XK_BackSpace, spawn, {.v = (const char *[]){"sysact", NULL}}},

    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_q, killclient, {0}},
    {MODKEY | ShiftMask,
     XK_s,
     spawn,
     {.v = (const char *[]){"flameshot", "gui", NULL}}},
    {MODKEY, XK_w, spawn, {.v = (const char *[]){BROWSER, NULL}}},
    {MODKEY | ShiftMask,
     XK_w,
     spawn,
     {.v = (const char *[]){"networkmanager_dmenu", NULL}}},
    {MODKEY,
     XK_r,
     spawn,
     {.v = (const char *[]){TERMINAL, "-e", "ranger", NULL}}},
    {MODKEY | ShiftMask,
     XK_r,
     spawn,
     {.v = (const char *[]){TERMINAL, "-e", "htop", NULL}}},

    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}}, /* spiral */
    {MODKEY | ShiftMask,
     XK_t,
     setlayout,
     {.v = &layouts[1]}}, /* centeredmaster */

    {MODKEY | ShiftMask, XK_u, setlayout, {.v = &layouts[2]}}, /* tile */
    {MODKEY, XK_u, setlayout, {.v = &layouts[4]}},             /* floating */

    {MODKEY, XK_o, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_o, incnmaster, {.i = -1}},

    {MODKEY, XK_a, togglegaps, {0}},
    {MODKEY | ShiftMask, XK_a, defaultgaps, {0}},
    {MODKEY, XK_s, togglesticky, {0}},
    {MODKEY, XK_d, spawn, {.v = (const char *[]){"dmenu_run", NULL}}},
    {MODKEY | ShiftMask,
     XK_d,
     spawn,
     {.v = (const char *[]){"keepassxc", NULL}}},
    {MODKEY, XK_f, togglefullscr, {0}},
    {MODKEY | ShiftMask, XK_f, setlayout, {.v = &layouts[8]}},
    {MODKEY, XK_g, shiftview, {.i = -1}},
    {MODKEY | ShiftMask, XK_g, shifttag, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    /* J and K are automatically bound above in STACKEYS */
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_semicolon, shiftview, {.i = 1}},
    {MODKEY | ShiftMask, XK_semicolon, shifttag, {.i = 1}},
    {MODKEY, XK_apostrophe, togglescratch, {.ui = 1}},
    {MODKEY | ShiftMask, XK_apostrophe, togglesmartgaps, {0}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY | ShiftMask, XK_Return, togglescratch, {.ui = 0}},

    {MODKEY, XK_z, incrgaps, {.i = +3}},
    {MODKEY, XK_x, incrgaps, {.i = -3}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_c, spawn, SHCMD(TERMINAL " -e lvim ~/.src-repos/dwm/config.h")},

    {MODKEY, XK_y, spawn, {.v = (const char *[]){"ytfzf", "-D", NULL}}},
    {MODKEY | ControlMask,
     XK_m,
     spawn,
     {.v = (const char *[]){"Dmenu-Streaming", NULL}}},
    {MODKEY | ControlMask,
     XK_y,
     spawn,
     {.v = (const char *[]){"st", "-e", "ytccf", "--no-thumbnails", NULL}}},

    {MODKEY | ControlMask,
     XK_b,
     spawn,
     {.v = (const char *[]){"bookmarks", NULL}}},

    {MODKEY | ShiftMask,
     XK_y,
     spawn,
     {.v = (const char *[]){"youtube-dmenu", NULL}}},

    {MODKEY | ControlMask,
     XK_t,
     spawn,
     {.v = (const char *[]){"telegram-choice", NULL}}},

    {MODKEY,
     XK_m,
     spawn,
     {.v = (const char *[]){TERMINAL, "-e", "termusic", NULL}}},

    {MODKEY, XK_Left, focusmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_Left, tagmon, {.i = -1}},
    {MODKEY, XK_Right, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_Right, tagmon, {.i = +1}},

    {MODKEY, XK_Page_Up, shiftview, {.i = -1}},
    {MODKEY | ShiftMask, XK_Page_Up, shifttag, {.i = -1}},
    {MODKEY, XK_Page_Down, shiftview, {.i = +1}},
    {MODKEY | ShiftMask, XK_Page_Down, shifttag, {.i = +1}},
    {MODKEY | ShiftMask, XK_m, spawn,
     SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)")},
    {MODKEY, XK_space, zoom, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},

    {MODKEY, XK_p, spawn, SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png")},

    /* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } },
     */
    /* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } },
     */
    /* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
     */
    /* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
     */
    /* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
     */
    /* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
     */
    /* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
    /* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
     */
    /* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
     */
    /* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
     */
    /* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
     */
    /* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
     */
    /* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
     */
    /* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
     */
    /* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
     */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */
    {ClkStatusText, ShiftMask, Button3, spawn,
     SHCMD(TERMINAL " -e lvim ~/.src-repos/dwm/config.h")},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, defaultgaps, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkClientWin, MODKEY, Button4, incrgaps, {.i = +1}},
    {ClkClientWin, MODKEY, Button5, incrgaps, {.i = -1}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
    {ClkTagBar, 0, Button4, shiftview, {.i = -1}},
    {ClkTagBar, 0, Button5, shiftview, {.i = 1}},
};
