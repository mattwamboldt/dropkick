#include "settings.h"

namespace Tetris
{
	Settings::Settings()
	{
		// By default we have tetris guideline settings http://tetrisconcept.net/wiki/Tetris_Guideline
		holdEnabled = true;
		previewEnabled = true;
		ghostEnabled = true;
		hardDropEnabled = true;
		hardDropLocks = true;
		softDropLocks = false;
		softDropFrames = 2;
		softDropDistance = 1;
		lockDelayFrames = 30;
	}
}