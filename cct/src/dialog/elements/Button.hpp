/**
* Created by univrsal on 14.07.2017.
* This file is part of reloded which is licensed
* under the MOZILLA PUBLIC LICENSE 2.0 - mozilla.org/en-US/MPL/2.0/
* github.com/univrsal/reloded
*/

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "GuiElement.hpp"
#include "../Dialog.hpp"
#include "../../util/SDL_helper.hpp"

class Dialog;

class Button : public GuiElement
{
public:
	Button(int8_t id, int x, int y, const char *text, Dialog *parent);

	Button(int8_t id, int x, int y, int w, const char *text, Dialog *parent);

	~Button();

	bool can_select(void);

	void select_state(bool state);

	void draw_background(void);

	void draw_foreground(void);

	bool handle_events(SDL_Event *event);

	void close(void);

	void resize(void);

protected:
	bool m_pressed = false;
	bool m_hovered = false;
	bool m_tab_focused = false;

	std::string m_text;
	SDL_Point m_text_pos;
};

#endif
