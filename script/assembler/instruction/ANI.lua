function init ()

end

function on_key_up ()
	wl_select_menu_entity ("ANA")
end

function on_key_down ()
   wl_select_menu_entity ("CALL")
end

function on_key_left ()
	wl_select_menu_entity ("SBB")
end

function on_key_right ()
	wl_select_menu_entity ("HLT")
end

function on_key_a ()
	wl_mnemonic_set_base (0xE6)
	wl_create_menu ("byte_a", 176, 8, "frame.png")
	wl_create_entity (2, 176, 8, "0", "script/assembler/byte/NA0.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 16, "1", "script/assembler/byte/NA1.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 24, "2", "script/assembler/byte/NA2.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 32, "3", "script/assembler/byte/NA3.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 40, "4", "script/assembler/byte/NA4.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 48, "5", "script/assembler/byte/NA5.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 56, "6", "script/assembler/byte/NA6.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 64, "7", "script/assembler/byte/NA7.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 72, "8", "script/assembler/byte/NA8.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 80, "9", "script/assembler/byte/NA9.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 88, "A", "script/assembler/byte/NAA.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 96, "B", "script/assembler/byte/NAB.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 104, "C", "script/assembler/byte/NAC.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 112, "D", "script/assembler/byte/NAD.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 120, "E", "script/assembler/byte/NAE.lua", "white", "red", "yellow", "gray")
	wl_create_entity (2, 176, 128, "F", "script/assembler/byte/NAF.lua", "white", "red", "yellow", "gray")
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
      "AND IMMEDIATE WITH ACCUMULATOR" ..
      "                              " ..
      "ANI performs a logical AND op-" ..
      "eration using the contents of " ..
      "the of the Instruction's 2nd  " ..
      "byte with the accumulator. The" ..
      "result is placed In the accu- " ..
      "mulator. ANI also resets the  " ..
      "carry flag to zero. The       " ..
      "operand must specify the data " ..
      "to be used in the operation.  " ..
      "This data may be in the form  " ..
      "of a number, an ASCII constant" ..
      "or the label of some          " ..
      "previously defined value, or  " ..
      "an expression. The data may   " ..
      "not exceed one byte. The      " ..
      "assembler's relocation feature" ..
      "treats all external and relo- " ..
      "catable symbols as 16-bit     " ..
      "addresses. When one of these  " ..
      "symbols appears in the operand" ..
      "expression of an immediate    " ..
      "instruction, it must be       " ..
      "preceded by either the HIGH or" ..
      "LOW operator to specify which " ..
      "byte of the address is to be  " ..
      "used in the evaluation of the " ..
      "expression. When neither      " ..
      "operator is present, the      " ..
      "assembler assumes the LOW     " ..
      "operator and issues an error  " ..
      "message.",
      "frame.png"
   )
end

function on_key_start ()
	
end

function on_key_back ()
	
end

init ()
