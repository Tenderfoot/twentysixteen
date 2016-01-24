
#include "base_user_interface.h"

void TextWidget::draw()
{
	Paintbrush::draw_text(text, 512, 200, 300, 300);
}

void BaseUserInterface::draw()
{
	int i;
	for (i = 0; i < widgets.size(); i++)
		widgets.at(i)->draw();
}

void BaseUserInterface::add_widget(UIWidget *new_widget)
{
	widgets.push_back(new_widget);
}