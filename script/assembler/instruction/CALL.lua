function init ()

end

function on_key_up ()
	wl_select_menu_entity ("ANI")
end

function on_key_down ()
   wl_select_menu_entity ("CC")
end

function on_key_left ()
	wl_select_menu_entity ("SBI")
end

function on_key_right ()
	wl_select_menu_entity ("IN")
end

function on_key_a ()
	wl_mnemonic_set_base (0xCD)
	wl_create_menu ("byte_a", 176, 24, "frame.png")
	wl_create_entity (2, 176, 24, "0", "script/assembler/word/a/NA0.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "1", "script/assembler/word/a/NA1.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "2", "script/assembler/word/a/NA2.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "3", "script/assembler/word/a/NA3.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 56, "4", "script/assembler/word/a/NA4.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 64, "5", "script/assembler/word/a/NA5.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 72, "6", "script/assembler/word/a/NA6.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 80, "7", "script/assembler/word/a/NA7.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 88, "8", "script/assembler/word/a/NA8.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 96, "9", "script/assembler/word/a/NA9.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 104, "A", "script/assembler/word/a/NAA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 112, "B", "script/assembler/word/a/NAB.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 120, "C", "script/assembler/word/a/NAC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 128, "D", "script/assembler/word/a/NAD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 136, "E", "script/assembler/word/a/NAE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 144, "F", "script/assembler/word/a/NAF.lua", "white", "red", "yellow", "gray")
	wl_append_menu ()
	wl_rundown_task ()
	wl_select_menu_entity ("0")
end

function on_key_b ()
      wl_repeal_menu ()
end

function on_key_x ()
	
end

function on_key_y ()
	wl_create_textbox (
      "white", 
      "CALL                          " ..
      "                              " ..
  		"The CALL Irstructlon combines " ..
  		"functions of the PUSH and POP " ..
  		"Instructions. CALL pushes the " ..
  		"content of the program counter" ..
  		"(the sequential instruction's " ..
  		"next address) onto the stack  " ..
  		"and then jumps to the address " ..
		"specified in the CALL.        " ..
		"Each CALL Instruction or one  " ..
		"of It's variants implies the  " ..
		"use of a subsequent RET       " ..
		"(return) Instruction. When a  " ..
		"call has no corresponding RET." ..
		"excess addresses are built up " ..
		"in the stack. The address may " ..
		"be specified as a number, a   " ..
		"label, or an expression.      " ..
		"(The label is most common) The" ..
		"assembler inverts the high and" ..
		"low address bytes when it     " ..
		"assembles the instruction. The" ..
		"larger the code segment to be " ..
		"repeated and the greater the  " ..
		"number of repetitions, the    " ..
		"greater the potential memory  " ..
		"savings of using a subroutine." ..
		"Thus, a Routine requires one  " ..
		"hundred bytes, coding it In-  " ..
		"line would require three      " ..
		"hundred bytes. Coded as a sub-" ..
		"routine, it requires one      " ..
		"hundred bytes plus 9 bytes for" ..
		"the 3 CALL instructions.",
      "frame.png"
   )
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
