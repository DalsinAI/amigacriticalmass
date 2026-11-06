/* Critical Mass for AmigaOS: what main() does first. With no resource.dat
 * the game reads its data/ from the current directory, so the program
 * moves into its drawer, as Workbench does. From The Legend of Edgar's and
 * Freegish's ports. The AmiCradle Team. GPL 2 or later, like Critical Mass. */
#include <stdlib.h>
#include <proto/dos.h>

static BPTR old_dir, prog_dir;

static void amiga_start_end(void)
{
	if (prog_dir) {
		CurrentDir(old_dir);
		UnLock(prog_dir);
		prog_dir = 0;
	}
}

void amiga_start(void)
{
	BPTR here = GetProgramDir();
	if (here && (prog_dir = DupLock(here)) != 0) {
		old_dir = CurrentDir(prog_dir);
		atexit(amiga_start_end);
	}
}
