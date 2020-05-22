#pragma once

#include "fow_player.h"

class FOWEditor : public FOWPlayer
{
public:
	FOWEditor();

	void take_input(boundinput input, bool type);
	void update();

	int blobtype;
	bool blob_droppin;	// I like to have fun sometimes
};