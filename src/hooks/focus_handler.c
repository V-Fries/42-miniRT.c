#include "engine.h"
#include "mlx_wrapper.h"



#include <stdio.h>
int	focus_in_handler(t_engine *engine)
{
	printf("focus_in_handler\n");
	if (engine->camera.lock == false)
		mouse_hide(engine);
	engine->window.is_focused = true;
	return (0);
}

int	focus_out_handler(t_engine *engine)
{
	printf("focus_out_handler\n");
	if (engine->camera.lock == false)
		mouse_show(engine);
	engine->window.is_focused = false;
	return (0);
}
