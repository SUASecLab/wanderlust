path = 
{
	 	{10,22,29,14}, {29,14,30,22}, {30,22,29,22}, {29,22, 14, 27}, {14,27,10,27}, {10,27,10,22}
}

function next (r, c)
	if r == nil or c == nil then
		return 0
	end
	for i = 1, #path do
		if path [i][2] == c and path [i][1] == r then
			wl_modify_state_entity ("bert", 5, path [i][3]) -- row
			wl_modify_state_entity ("bert", 6, path [i][4]) -- column
			return 1
		end
	end
	return 0
end

function talk ()
	wl_create_textbox ("white", "I'm sorry, but i can not help\nyou right now. Try talking to\nthe old man in the house...", "frame.png")
end

function init ()

end

init ()
