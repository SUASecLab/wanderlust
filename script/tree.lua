function callback_tree_00 ()
   io.write ("tree: 00")
end

function callback_tree_01 ()
   io.write ("tree: 01")
end

function callback_tree_02 ()
   io.write ("tree: 02")
end

function callback_tree_03 ()
   io.write ("tree: 03")
end

function callback_tree_04 ()
   io.write ("tree: 04")
end

function callback_tree_05 ()
   io.write ("tree: 05")
end

tree =
   {
      {"tree00", callback_tree00},
      {"tree01", callback_tree01},
      {"tree02", callback_tree02},
      {"tree03", callback_tree03},
      {"tree04", callback_tree04},
      {"tree05", callback_tree05}
   }

function interact (name)
   for i = 1, #tree do
      if name == tree [i][1] then
	     wl_expire_state_entity (name)
      end
   end
end

