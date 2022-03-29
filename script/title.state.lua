textures = 
   {
   {"transition.fade.in.png", "image/transition/transition.fade.in.png"},
   {"transition.fade.out.png", "image/transition/transition.fade.out.png"},
   {"transition.stripes.in.png" ,"image/transition/transition.stripes.in.png"},
   {"transition.stripes.out.png","image/transition/transition.stripes.out.png"},
	{"transition.cube.in.png","image/transition/transition.cube.in.png"},
   {"transition.cube.out.png","image/transition/transition.cube.out.png"},
   {"frame.png", "image/frame/frame.png"},
   {"title.png", "image/background/title.png"},
   {"smaragd.png", "image/background/smaragd.png"},
   {"title_a.png", "image/background/title_a.png"},
   {"title_b.png", "image/background/title_b.png"}
   }

function init ()   
   for i = 1, #textures do
      wl_load_image (textures [i][1], textures[i][2])
   end

   wl_create_font ("red",    0xFF0000FF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("green",  0x00FF00FF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("blue",   0x0080FFFF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("cyan",   0x00FFFFFF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("white",  0xFFFFFFFF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("yellow", 0xFFFF00FF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("gray",   0x404040FF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("lightgray",   0xE0E0E0FF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)
   wl_create_font ("mediumgray",   0xB0B0B0FF, 0x00000000, 0xFF000000, 0xFF0000, 0xFF00, 0xFF)

   wl_rundown_task ()

   --wl_create_entity (2, 8,8, "New Game", "script/menu.main/menu.button.new_game.lua", "white", "blue", "yellow", "gray")
   --wl_create_entity (2, 8,16, "Continue", "script/menu.main/menu.button.continue.lua", "white", "blue", "yellow", "gray")
   --wl_create_entity (2, 8,24, "Options", "script/menu.main/menu.button.options.lua", "white", "blue", "yellow", "gray")
   --wl_create_entity (2, 8,32, "Terminal", "script/menu.main/menu.button.terminal.lua", "white", "blue", "yellow", "gray")
   --wl_create_entity (2, 8,40, "Assembler", "script/menu.main/menu.button.assembler.lua", "white", "blue", "yellow", "gray")
   wl_create_entity (2, 112,148, "Press Start", "script/start_button.lua", "white", "blue", "yellow", "gray")

   wl_fetch_state ()

   wl_set_background ("title_a.png")

   wl_rundown_task ()

   wl_select_state_entity ("Press Start")

end

init ()
