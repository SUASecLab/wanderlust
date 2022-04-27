function init ()

end

function on_key_up ()
	wl_select_menu_entity ("JP")
end

function on_key_down ()
   wl_select_menu_entity ("JPO")
end

function on_key_left ()
	wl_select_menu_entity ("CPE")
end

function on_key_right ()
	wl_select_menu_entity ("RLC")
end

function on_key_a ()
	wl_mnemonic_set_base (0xEA)
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
      "JUMP IF PARITY EVEN           " ..
      "                              " ..
      "Parity is even if the byte in " ..
      "the accumulator has an even   " ..
      "number of one bits. The parity" ..
      "flag IS set to one to in-     " ..
      "dicate this condition. The JPE" ..
      "Instruction tests the setting " ..
      "of the parity flag. If the    " ..
      "parity flag is set to one,    " ..
      "program execution resumes at  " ..
      "the address specified in the  " ..
      "JPE instruction. If the flag  " ..
      "is reset to zero, execution   " ..
      "continues with the next seq-  " ..
      "quential in instruction.      " ..
      "The address may be specified  " ..
      "as a number, a label, or an   " ..
      "expression. The assembler in- " ..
      "verts the high and low address" .. 
      "byte, when it assembles the   " ..
      "instruction.                  " ..
      "The JPE and JPO (jump if par- " ..
      "rity odd) instructions are    " ..
      "especially useful for testing " ..
      "the parity of input data.     " ..
		"However, the IN instruction   " ..
		"does not set any of the con-  " ..
		"dition flags. The flags can be" ..
		"set by adding OOH to the con- " ..
		"tents of the accumulator.",
      "frame.png"
   )
end

function on_key_start ()

end

function on_key_back ()

end

init ()
