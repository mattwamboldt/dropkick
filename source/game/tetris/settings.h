#ifndef TETRIS_SETTINGS_H
#define TETRIS_SETTINGS_H

/////////////////////////////////////////////////////////
// Settings that separate different versions of tetris
/////////////////////////////////////////////////////////

namespace Tetris
{
	class Settings
	{
	public:
		Settings();

		bool holdEnabled;
		bool previewEnabled;
		bool ghostEnabled;

		//Gravity http://tetrisconcept.net/wiki/Drop
		bool hardDropEnabled;
		bool hardDropLocks;// Whether or not harddrop skips lockdelay
		bool softDropLocks; // Whether or not softdrop skips lockdelay 

		//Soft drop http://tetrisconcept.net/wiki/Drop#Soft_drop
		int softDropFrames;
		int softDropDistance;

		//Lock Delay http://tetrisconcept.net/wiki/Lock_delay
		//How long the pieces wait before locking to the field
		int lockDelayFrames;
	};
}

#endif