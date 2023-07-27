#include "events.h"
#include "engine.h"
#include "hooks.h"

int	key_press_handler(int key_code, t_engine *engine)
{
	int	i;

	if (key_code == KEY_ESC)
		close_engine(engine);
	else if (key_code == KEY_H)
		toggle_gui(&engine->gui);
	if (key_code != KEY_W && key_code != KEY_S && key_code != KEY_A
		&& key_code != KEY_D && key_code != KEY_SPACE && key_code != KEY_L_SHIFT
		&& key_code != KEY_Q && key_code != KEY_E)
		return (0);
	i = -1;
	while (++i < engine->pressed_keys_index)
		if (key_code == engine->pressed_keys[i])
			return (0);
	engine->pressed_keys[engine->pressed_keys_index++] = key_code;
	return (0);
}
