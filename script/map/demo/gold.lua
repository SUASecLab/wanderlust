warp = 
   {
      {  8, 11, 37,128,3}, {  9, 11, 38,128,3}, { 37,128,  8, 11,1}, { 38,128,  9, 11,1},
      { 37,119, 37,128,3}, { 38,119, 38,128,3}, { 15, 31, 16,120,0}, { 17,120, 16, 31,2},
      { 17,123,164, 42,2}, {163, 42, 16,123,0}, { 17,126, 11, 39,2}, { 10, 39, 16,126,0},
      {164, 12, 37,128,3}, {165, 12, 38,128,3}, {231, 85, 42,153,0}, { 42,135,232, 85,2},
      { 42,136,232, 85,2}, {227, 85, 35,153,2}, {227, 86, 35,154,2}, { 43,153,226, 85,0},
      { 43,154,226, 86,0}, { 29, 37, 23,151,0}, { 24,151, 30, 37,2}, { 24,150, 30, 37,2},
      { 14,158, 49, 46,2}, { 48, 46, 13,158,2}, { 39, 78, 15,182,0}, { 39, 79, 15,183,0},
      { 16,182, 40, 78,2}, { 16,183, 40, 79,2}, {  8,182,126, 46,0}, {  8,183,126, 47,0},
      {126, 46,  9,182,2}, {126, 47,  9,183,2},

      {120, 41,120, 52,3}, {121, 41,121, 52,3}, {115, 46,126, 46,0}, {115, 47,126, 47,0},
      {120, 52,120, 18,1}, {121, 52,121, 18,1}, {120, 29,120, 18,3}, {121, 29,121, 18,3},
      {126, 23,115, 23,2}, {126, 24,115, 24,2}, {115, 23,104, 46,0}, {115, 24,104, 47,0},
      {120, 18,120, 29,3}, {121, 18,121, 29,3},

      { 93, 46,104, 46,0}, { 93, 47,104, 47,0}, { 98, 52, 98, 41,1}, { 99, 52, 99, 41,1},
      {104, 46, 93, 46,2}, {104, 47, 93, 47,2}, { 98, 41, 98, 29,3}, { 99, 41, 99, 29,3},

      {104, 23, 93, 23,2}, {104, 24, 93, 24,2}, { 98, 18, 98, 29,3}, { 99, 18, 99, 29,3},
      { 98, 29, 98, 18,1}, { 99, 29, 99, 18,1}, { 93, 23, 69, 19,0}, { 93, 24, 69, 20,0},
      { 69, 19,115, 46,2}, { 69, 20,115, 47,2}, { 52, 16, 13,150,0}, { 14,150, 53, 16,2}
   }

function enter_door (hash)
   --   c = wl_get_property ("PLAYER_COLUMN")
   --   r = wl_get_property ("PLAYER_ROW")
   
   if hash == nil then
      return 0
   end
   
   r = wl_get_state_entity_by_hash_row (hash)
   c = wl_get_state_entity_by_hash_column (hash)

   --   io.write ("hash (lua): ", hash, "\n")
   
   if c == nil or r == nil then
      return 0
   end
   
   for i = 1, #warp
   do
      if c == warp [i][2] and r == warp [i][1] then
	 wl_warp_map_by_transition (warp[i][3], warp[i][4], warp[i][5], hash, "transition.fade.out.png", 16)
	 wl_create_transition ("transition.fade.in.png", 16)
	 return 1
      end
   end

   --   wl_warp_map_by_transition (0, 0, "transition.fade.out.png", 16)
   --   wl_create_transition ("transition.fade.in.png", 16)
   
   return 0
end

read = 
   {
      {256,256, "\n\nYou can not read the letters\nfrom this position!"},
      { 17,  9, "Did you know!? That Wanderlust is a strong desire for or impulse to wander or travel and explore the world.  Feel free to explore this adventerous environment. You'll be likely to find some well known places."},
      { 21, 26, "Fractal Pond\n\nA calm place for creatures of\nall kind."},
      { 27, 36, "For those who enter the woods:\nwest, north, east, north..."},
      {120, 15, "Visiting some old Friends!\n\nAren't you?"},
      {123, 15, "Be remeniscent of a simpler\nTime.\n\nOr is it just a deja vu?"},
      {126, 15, "This way leads to a forest,\nthat is not sacred at all."}      
   }

function read_sign ()
   c = wl_get_property ("PLAYER_COLUMN")
   r = wl_get_property ("PLAYER_ROW")
   d = wl_get_property ("PLAYER_DIRECTION")
   
   if c == nil or r == nil or d == nil then
      return 0
   end
   
   for i = 2, #read do
      -- player faces sign in n direction, subract 1 to express position
      if c == read [i][1] and r - 1 == read [i][2] and d == 0 then
	 wl_create_textbox ("white", read [i][3], "frame.png")
	 return 1
      end
   end
   
   wl_create_textbox ("white", read [1][3], "frame.png")
   
   return 0
end

textures =
   {
      {"tree.png", "image/sprite/tree.png"}
   }

function init ()
   for i = 1, #textures do
      wl_load_image (textures [i][1], textures [i][2])
   end
   
   wl_rundown_task ()
   
   wl_create_entity (1, "bert", "bert.png", 10, 22, "script/npc.lua")
   -- wl_create_entity (1, "bart", "hero.png", 46, 59, "script/npc.lua")
   wl_create_entity (3, "tree00", "tree.png", 21, 163, 0, 0, 16, 16, "script/tree.lua", "interact")
   wl_create_entity (3, "tree01", "tree.png", 33, 168, 0, 0, 16, 16, "script/tree.lua", "interact")
   wl_create_entity (3, "tree02", "tree.png", 36, 182, 0, 0, 16, 16, "script/tree.lua", "interact")
   wl_create_entity (3, "tree03", "tree.png", 88, 239, 0, 0, 16, 16, "script/tree.lua", "interact")
   wl_create_entity (3, "tree04", "tree.png", 79, 241, 0, 0, 16, 16, "script/tree.lua", "interact")
--   wl_create_entity (3, "tree05", "tree.png", 21, 11, 0, 0, 16, 16, "script/tree.lua", "interact")
   wl_create_entity (3, "door", "tree.png", 123, 16, 0, 0, 16, 16, "script/door.lua", "interact")

   wl_rundown_task ()

   wl_fetch_state ()
   
   wl_attach_event (0, "enter_door")
   wl_attach_event (1, "enter_door")
   wl_attach_event (2, "enter_door")
   wl_attach_event (3, "enter_door")
   wl_attach_event (5, "read_sign")
end

init ()
