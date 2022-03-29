function interact (name)
   ready = wl_get_property ("challenge")
   if ready == nil then
      wl_create_textbox ("white", "I've updated the rom in your microcontroller. Open the Terminal to read it's content. It will contain an error. So try to fix it and i will let you pass.", "frame.png")
      wl_set_property (1, "challenge", "excepted")
      wl_assembler_rom_load ("rom/wanderlust.rom")
   else
      -- set property door to the output ports of the cpu
      wl_core_get_output ("door")
      -- get the string representation of these ports
      data = wl_get_property ("door")
      -- proof
      if data == "hallo!" then
         wl_create_textbox ("white", "ACCESS GRANTED, you may enter...", "frame.png")
         wl_expire_state_entity (name)
      else
         wl_create_textbox ("white", "To enter the set output to \"hallo!\"", "frame.png")
      end
   end

end
