textures = 
   {
      {"hero.png", "image/sprite/hero.png"},
      {"bert.png", "image/sprite/bert.png"},
      --{"gold.png", "image/tileset/gold.png"},
      {"gold.png", "image/tileset/wanderlust.png"},
      {"dawn.png", "image/tileset/dawn.png"}, 
      {"transition.fade.in.png", "image/transition/transition.fade.in.png"},
      {"transition.fade.out.png", "image/transition/transition.fade.out.png"},
      {"transition.stripes.in.png" ,"image/transition/transition.stripes.in.png"},
      {"transition.stripes.out.png","image/transition/transition.stripes.out.png"},
      {"transition.cube.in.png","image/transition/transition.cube.in.png"},
      {"transition.cube.out.png","image/transition/transition.cube.out.png"}
   }

function on_key_start ()

end

function on_key_back ()

end

function init ()   
   for i = 1, #textures do
      wl_load_image (textures [i][1], textures [i][2])
   end

   wl_rundown_task ()

   --wl_load_map ("map/demo/gold.map", "gold.png", "script/map/demo/gold.lua")
   wl_load_map ("map/demo/gold.map", "gold.png", "script/map/demo/gold.lua")
   wl_select_map("gold.map")
   
   wl_create_entity (0, "hero", "hero.png", "script/hero.lua")
	wl_fetch_state ()
   
   wl_warp_map (17, 16)	
   wl_rundown_task ()

end

init ()
