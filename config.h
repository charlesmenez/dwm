#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

static unsigned int borderpx = 2;
static unsigned int snap = 0;
static unsigned int gappih = 20;
static unsigned int gappiv = 20;
static unsigned int gappoh = 20;
static unsigned int gappov = 20;
static int swallowfloating = 0;
static int smartgaps = 1;
static int showbar = 1;
static int topbar = 1;
static char *fonts[] = {
    "SauceCodePro Nerd Font:size=9"};
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#770000";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
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
    {"spterm", spcmd1},
    {"spcalc", spcmd2},
};

static const char *tags[] = { "\uf30d", "\uf489", "\uf26b", "\ueaf7", "\uf36f", "\uf338", "\uf306", "\uf337", "\uf1d3" };

static const Rule rules[] = {
    {TERMCLASS, NULL, NULL, 0, 0, 1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1},
  	{ "firefox",  NULL,       NULL,       1 << 2,       0,           -1 },
  	{ TERMCLASS,  NULL,       NULL,       1 << 1,       0,           -1 },
  	{ "firedragon",  NULL,       NULL,       1 << 2,       0,           -1 },
    {TERMCLASS, "bg", NULL, 1 << 7, 0, 1, 0, -1},
    {TERMCLASS, "spterm", NULL, SPTAG(0), 1, 1, 0, -1},
    {TERMCLASS, "spcalc", NULL, SPTAG(1), 1, 1, 0, -1},
};

static float mfact = 0.55;
static int nmaster = 2;
static int resizehints = 0;
#define FORCE_VSPLIT 1
#include "vanitygaps.c"
static const Layout layouts[] = {
    {"[@]", spiral},   {"|M|", centeredmaster}, {"[]=", tile},
    {"[\\]", dwindle}, {"><>", NULL},           {NULL, NULL},
};

#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD, XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
      {MOD, XK_k, ACTION##stack, {.i = INC(-1)}},                              \
      {MOD, XK_v, ACTION##stack, {.i = 0}},

#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

static const char *termcmd[] = {TERMINAL, NULL};

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
    STACKKEYS(MODKEY, focus) STACKKEYS(MODKEY | ShiftMask, push){
        MODKEY, XK_e, spawn, {.v = (const char *[]){"dmenu-emoji", NULL}}},
    {MODKEY, XK_q, spawn, {.v = (const char *[]){"pkill", "dwm", NULL}}},
    {MODKEY | ControlMask,
     XK_w,
     spawn,
     {.v = (const char *[]){"dwm-colors", NULL}}},

    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

    {MODKEY,
     XK_BackSpace,
     spawn,
     {.v = (const char *[]){"sysact", NULL}}},
    {MODKEY | ShiftMask,
     XK_BackSpace,
     spawn,
     {.v = (const char *[]){"cpufreq-dmenu", NULL}}},

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

    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY | ShiftMask, XK_t, setlayout, {.v = &layouts[1]}},
    {MODKEY | ShiftMask, XK_u, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_u, setlayout, {.v = &layouts[4]}},

    {MODKEY, XK_o, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_o, incnmaster, {.i = -1}},

    {MODKEY, XK_a, togglegaps, {0}},
    {MODKEY | ShiftMask, XK_a, defaultgaps, {0}},
    {MODKEY, XK_s, togglesticky, {0}},
    {MODKEY, XK_d, spawn, {.v = (const char *[]){"dmenu_run", NULL}}},
    {MODKEY | ShiftMask,
     XK_d,
     spawn,
     {.v = (const char *[]){"rofi", "-show", "drun", NULL}}},
    {MODKEY, XK_f, togglefullscr, {0}},
    {MODKEY | ShiftMask, XK_f, setlayout, {.v = &layouts[8]}},
    {MODKEY, XK_g, shiftview, {.i = -1}},
    {MODKEY | ShiftMask, XK_g, shifttag, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
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
    {MODKEY, XK_c, spawn, SHCMD(TERMINAL " -e nvim ~/dwm/config.h")},

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
     {.v = (const char *[]){"telegram-desktop", NULL}}},

    {MODKEY, XK_m, spawn, {.v = (const char *[]){"ytfzf", "-D", "-m", NULL}}},

    {MODKEY,
     XK_F11,
     spawn,
     {.v = (const char *[]){"backlight-decrease", NULL}}},
    {MODKEY,
     XK_F12,
     spawn,
     {.v = (const char *[]){"backlight-increase", NULL}}},

    {MODKEY | ControlMask, XK_j, focusmon, {.i = -1}},
    {MODKEY | ControlMask, XK_k, focusmon, {.i = -1}},
    {MODKEY | ControlMask, XK_h, tagmon, {.i = -1}},
    {MODKEY | ControlMask, XK_l, tagmon, {.i = +1}},
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
    {MODKEY, XK_p, spawn, SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png")},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_space, zoom, {0}},

};

static Button buttons[] = {
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
