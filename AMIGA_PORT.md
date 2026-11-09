Critical Mass 1.0.2 for AmigaOS 3.x
===================================

Critical Mass, also called Critter (Frank Becker,
http://criticalmass.sourceforge.net), the shoot-'em-up: your ship at the
bottom of the screen, waves of alien bugs swooping down, weapon and shield
power-ups, and a boss. 3D models and particle effects drawn with OpenGL,
music by Lunar Gravity. For AmigaOS 3.x on a 68040 or 68060 with an FPU
and an RTG screen, on OpenGPU's SDL 1.2 (sdl12-compat) and OpenGL.

Base: criticalmass_1.0.2.orig.tar.bz2 from Debian (sha256
846db2d30d39e559612328594084a7aef980169948b2cfaee9fddec8ffdef724), with
Debian's patches (1.0.2-5) as the second commit. Branch
team2/criticalmass-amiga.

Status: plays in the lab (Instance-45 on GFOpenUp 0.8.19, 6 November
2026): the menu over its animated ship and starfield, New Game, the first
wave (the ship moving and firing, an enemy shot down, the score), game
over, Config, and Quit back to the Shell, the high scores and settings
written in user/.critter. In the lab the game ran slowly and answered keys
a few seconds late; a key has to be held a moment to be seen. The lab is
muted, so the music and sounds were not heard there.


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab), and a fast one: the game's OpenGL drawing is heavy.
- GFOpenUp 0.8.18 or later: LIBS:OpenGPU/SDL2.module and GL.module.


How to play
-----------

Open the CriticalMass icon, or run CriticalMass from a Shell. It opens a
640x480 full screen. In the menu the cursor keys choose and Return picks;
in the game the cursor keys (or the mouse) move the ship, Space (or the
left mouse button) fires, left Alt and left Ctrl the other weapons, P
pauses, Escape is the menu. Readme.html in the drawer is upstream's
guide. The game's data is unpacked in data/; the settings and high scores
are kept in user/.critter.



Standard keys
-------------

Every AmiCradle game has the same keys on Right Amiga (GFOpenUp's
SDL2.module, after 0.8.26):

- Right Amiga + F: window or full screen, remembered for next time
- Right Amiga + P: pause (Right Amiga + P again goes on)
- Right Amiga + Q: quit
- Right Amiga + M: free the mouse pointer, or let the game take it again
- Right Amiga + S: a screenshot in the drawer's Screenshots
- Help: this game's keys (the drawer's Controls file)
- Right Amiga + F1, F2: F11, F12; Right Amiga + cursor keys: Page Up,
  Page Down, Home, End

The FULLSCREEN tooltype on the game's icon (take the brackets off) starts
it full screen.


Building
--------

    make -f amiga/Makefile.amigaos -j2 dist

with GFOpenAmigaGCC 16.2.0-open4, the OpenNDK headers, the OpenGPU kit and
the Team's static libpng and zlib (see the Makefile's head). The drawer is
dist/CriticalMass (and dist/CriticalMass.info).


What was changed
----------------

- game/main.cpp: main() first moves into the drawer (amiga/amiga_start.c),
  where the game reads its data/ unpacked (no resource.dat). $HOME is the
  drawer's user/ (amiga/amiga_env.c, -Wl,--wrap=getenv, from the GNU Robbo
  port). DATA_DIR is PROGDIR:data/, where the music is.
- amiga/Makefile.amigaos builds the Makefile.am sources of tinyxml, utils
  (without LZMA), utilssdl, utilsgl and game, with amiga/defines.h for
  configure's.
- No SDL_image or SDL_mixer 1.2 for OpenGPU's SDL 1.2: the Team's
  amiga/sdl_image_lite.c (PNG on libpng, now reading the game's own RWops
  too) and amiga/sdl_mixer_lite.c (WAV, now from the game's RWops too,
  with libmikmod 3.3.16 for the music: amiga/libmikmod, its player, XM
  loader and no-sound driver).
- libcurl, for the game's online update check, is a stub with no network
  (amiga/include/curl): the check fails at once and the game goes on, as
  it does offline.
- amiga/amiga_prefix.hpp: the C++ streams first in every file (the
  toolchain's pthread.h wraps read() and write() in macros once
  <unistd.h> is in). amiga/stack.c: 1 MB of stack.


Licences
--------

Critical Mass, its pictures, models, sounds and music: GPL 2 or later
(COPYING.txt; the music is Lunar Gravity's remix of Burkey's "Critical",
data/Readme.txt). libmikmod: LGPL 2 or later (COPYING-libmikmod.txt).
The Amiga files (amiga/): the AmiCradle Team; the mixer comes from the
Team's Wizznic port and is GPL 3 or later, so the program as built here is
GPL 3 or later.
