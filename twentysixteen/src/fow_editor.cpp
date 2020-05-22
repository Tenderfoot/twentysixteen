
#include "techdemos/fow_character.h"
#include "fow_building.h"
#include "fow_editor.h"
#include "base_user_interface.h"
#include "audio_controller.h"

FOWEditor::FOWEditor()
{
	queue_add_toggle = false;
	gold = 0;
	green_box = new GreenBox();
	camera_distance = 100.0f;
	camera_pos.x = 15;
	camera_pos.z = 10;
	blobtype = 0;
}

void FOWEditor::update()
{
	if (blob_droppin)
	{
		grid_manager->dropblob(grid_manager->mouse_x, grid_manager->mouse_y, blobtype);
		grid_manager->cull_orphans();
		grid_manager->calc_all_tiles();
	}
	FOWPlayer::update();
}

void FOWEditor::take_input(boundinput input, bool type)
{

	if (input == LMOUSE && type == true)
	{
		green_box->x = *mousex;
		green_box->y = *mousey;
		green_box->mouse_in_space = mouse_in_space;
		//green_box->visible = true;
		blob_droppin = true;
	}

	if (input == LMOUSE && type == false)
	{
		green_box->visible = false;
		blob_droppin = false;
		get_selection(grid_manager->convert_mouse_coords(green_box->mouse_in_space), grid_manager->convert_mouse_coords(mouse_in_space));
	}

	if (input == RIGHT && type == true)
	{
		camera_pos.x++;
	}

	if (input == LEFT && type == true)
	{
		camera_pos.x--;
	}

	if (input == UP && type == true)
	{
		camera_pos.z--;
	}

	if (input == DOWN && type == true)
	{
		camera_pos.z++;
	}

	if (input == EDITOR_T && type == true)
	{
		grid_manager->use_tex = !grid_manager->use_tex;
	}

	if (input == USE && type == true)
	{
		grid_manager->randomize_map();
	}

	if (input == NEXT && type == true)
	{
		blobtype++;
		blobtype = blobtype % 5;
	}

	if (input == MWHEELUP)
	{
		if (camera_distance > 0)
			camera_distance-=10;
	}

	if (input == MWHEELDOWN)
	{
			camera_distance+=10;
	}

}