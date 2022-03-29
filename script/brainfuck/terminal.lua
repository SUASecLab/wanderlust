function init (x, y)
	wl_create_menu ("bfterminal", x+8, y+8, "frame.png")
   

	wl_create_entity (2, x + 8, y + 8, "0", "script/brainfuck/button.0.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 16, y + 8, "1", "script/brainfuck/button.1.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 24, y + 8, "2", "script/brainfuck/button.2.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 32, y + 8, "3", "script/brainfuck/button.3.lua", "white", "red", "yellow", "gray")
	
	wl_create_entity (2, x + 40, y + 8, "4", "script/brainfuck/button.4.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 48, y + 8, "5", "script/brainfuck/button.5.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 56, y + 8, "6", "script/brainfuck/button.6.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 64, y + 8, "7", "script/brainfuck/button.7.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, x + 72, y + 8, "8", "script/brainfuck/button.8.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 80, y + 8, "9", "script/brainfuck/button.9.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 88, y + 8, "A", "script/brainfuck/button.a.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 96, y + 8, "B", "script/brainfuck/button.b.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, x + 104, y + 8, "C", "script/brainfuck/button.c.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 112, y + 8, "D", "script/brainfuck/button.d.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 120, y + 8, "E", "script/brainfuck/button.e.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 128, y + 8, "F", "script/brainfuck/button.f.lua", "white", "red", "yellow", "gray")
   
	wl_create_entity (2, x + 8, y + 16, "<", "script/brainfuck/button.decrement.pointer.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 16, y + 16, ">", "script/brainfuck/button.increment.pointer.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 24, y + 16, "-", "script/brainfuck/button.decrement.cell.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 32, y + 16, "+", "script/brainfuck/button.increment.cell.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, x + 40, y + 16, ".", "script/brainfuck/button.output.cell.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 48, y + 16, ",", "script/brainfuck/button.input.cell.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 56, y + 16, "[", "script/brainfuck/button.loop.begin.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 64, y + 16, "]", "script/brainfuck/button.loop.end.lua", "white", "red", "yellow", "gray")

	wl_create_entity (2, x + 72, y + 16, "\\", "script/brainfuck/button.push.lua", "white", "red", "yellow", "gray")	
	wl_create_entity (2, x + 80, y + 16, "|", "script/brainfuck/button.peek.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 88, y + 16, "/", "script/brainfuck/button.pop.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 96, y + 16, "!", "script/brainfuck/button.halt.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, x + 104, y + 16, "Menu", "script/brainfuck/button.menu.lua", "white", "red", "yellow", "gray")

	wl_append_menu ()

	wl_rundown_task ()

	wl_select_menu_entity ("<")
end