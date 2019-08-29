// "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-=!@#$%^&*()_+[]{};'\\:\"|,./<>? "

const char FONT[][7][6] = {{
		"#####",
		"#   #",
		"#  ##",
		"# # #",
		"##  #",
		"#   #",
		"#####"
	}, {
		"   # ",
		"  ## ",
		" # # ",
		"   # ",
		"   # ",
		"   # ",
		"   # "
	}, {
		" ### ",
		"#   #",
		"   # ",
		"  #  ",
		" #   ",
		"#    ",
		"#####"
	}, {
		" ### ",
		"#   #",
		"    #",
		" ### ",
		"    #",
		"#   #",
		" ### ",
	}, {
		"   # ",
		"  ## ",
		" # # ",
		"#  # ",
		"#####",
		"   # ",
		"   # "
	}, {
		"#####",
		"#    ",
		"#    ",
		"#### ",
		"    #",
		"#   #",
		" ### ",
	}, {
		" ### ",
		"#   #",
		"#    ",
		"#### ",
		"#   #",
		"#   #",
		" ### "
	}, {
		"#####",
		"    #",
		"    #",
		"   # ",
		"  #  ",
		"  #  ",
		"  #  "
	}, {
		" ### ",
		"#   #",
		"#   #",
		" ### ",
		"#   #",
		"#   #",
		" ### "
	}, {
		" ### ",
		"#   #",
		"#   #",
		" ####",
		"    #",
		"#   #",
		" ### "
	}, {
		"  #  ",
		" # # ",
		"#   #", 
		"#   #",
		"#####",
		"#   #",
		"#   #",
	}, {
		"#### ",
		"#   #",
		"#   #",
		"#### ",
		"#   #",
		"#   #",
		"#### "
	}, {
		" ### ",
		"#   #",
		"#    ",
		"#    ",
		"#    ",
		"#   #",
		" ### "
	}, {
		"#### ",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"#### "
	}, {
		"#####",
		"#    ",
		"#    ",
		"#####",
		"#    ",
		"#    ",
		"#####"
	}, {
		"#####",
		"#    ",
		"#    ",
		"#####",
		"#    ",
		"#    ",
		"#    "
	}, {
		" ### ",
		"#   #",
		"#    ",
		"#  ##",
		"#   #",
		"#   #",
		" ### "
	}, {
		"#   #",
		"#   #",
		"#   #",
		"#####",
		"#   #",
		"#   #",
		"#   #",
	}, {
		"#####",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"#####"
	}, {
		"#####",
		"    #",
		"    #",
		"    #",
		"    #",
		"#   #",
		" ### "
	}, {
		"#   #",
		"#  # ",
		"# #  ",
		"##   ",
		"# #  ",
		"#  # ",
		"#   #"
	}, {
		"#    ",
		"#    ",
		"#    ",
		"#    ",
		"#    ",
		"#    ",
		"#####"
	}, {
		"#   #",
		"## ##",
		"## ##",
		"# # #",
		"#   #",
		"#   #",
		"#   #"
	}, {
		"#   #",
		"##  #",
		"##  #",
		"# # #",
		"#  ##",
		"#  ##",
		"#   #"
	}, {
		" ### ",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		" ### "
	}, {
		"#### ",
		"#   #",
		"#   #",
		"#### ",
		"#    ",
		"#    ",
		"#    "
	}, {
		" ### ",
		"#   #",
		"#   #",
		"#   #",
		"# # #",
		"#  ##",
		" ####"
	}, {
		"#### ",
		"#   #",
		"#   #",
		"#### ",
		"# #  ",
		"#  # ",
		"#   #"
	}, {
		" ### ",
		"#   #",
		"#    ",
		" ### ",
		"    #",
		"#   #",
		" ### "
	}, {
		"#####",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  "
	}, {
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		" ### "
	}, {
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		" # # ",
		"  #  "
	}, {
		"#   #",
		"#   #",
		"#   #",
		"#   #",
		"# # #",
		"# # #",
		" # # "
	}, {
		"#   #",
		"#   #",
		" # # ",
		"  #  ",
		" # # ",
		"#   #",
		"#   #"
	}, {
		"#   #",
		"#   #",
		" # # ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  "
	}, {
		"#####",
		"    #",
		"   # ",
		"  #  ",
		" #   ",
		"#    ",
		"#####"
	}, {
		"     ",
		"     ",
		"     ",
		"#####",
		"     ",
		"     ",
		"     ",
	}, {
		"     ",
		"     ",
		"#####",
		"     ",
		"#####",
		"     ",
		"     "
	}, {
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"     ",
		"  #  "
	}, {
		" ### ",
		"#   #",
		"# ###",
		"# # #",
		"# ###",
		"#    ",
		" ####"
	}, {
		" # # ",
		" # # ",
		"#####",
		" # # ",
		"#####",
		" # # ",
		" # # "
	}, {
		" ### ",
		"# # #",
		"# #  ",
		" ### ",
		"  # #",
		"# # #",
		" ### "
	}, {
		"###  ",
		"# # #",
		"#### ",
		"  #  ",
		" ####",
		"# # #",
		"  ###"
	}, {
		"  #  ",
		" # # ",
		"#   #",
		"     ",
		"     ",
		"     ",
		"     "
	}, {
		" ### ",
		"#   #",
		"#    ",
		" #   ",
		" ## #",
		"#  # ",
		" ## #"
	}, {
		"  #  ",
		"#####",
		" # # ",
		"     ",
		"     ",
		"     ",
		"     "
	}, {
		"   # ",
		"  #  ",
		" #   ",
		" #   ",
		" #   ",
		"  #  ",
		"   # "
	}, {
		" #   ",
		"  #  ",
		"   # ",
		"   # ",
		"   # ",
		"  #  ",
		" #   "
	}, {
		"     ",
		"     ",
		"     ",
		"     ",
		"     ",
		"     ",
		"#####"
	}, {
		"     ",
		"  #  ",
		"  #  ",
		"#####",
		"  #  ",
		"  #  ",
		"     "
	}, {
		" ### ",
		" #   ",
		" #   ",
		" #   ",
		" #   ",
		" #   ",
		" ### "
	}, {
		" ### ",
		"   # ",
		"   # ",
		"   # ",
		"   # ",
		"   # ",
		" ### "
	}, {
		"  ## ",
		" #   ",
		" #   ",
		"#    ",
		" #   ",
		" #   ",
		"  ## "
	}, {
		" ##  ",
		"   # ",
		"   # ",
		"    #",
		"   # ",
		"   # ",
		" ##  "
	}, {
		"     ",
		"     ",
		"  #  ",
		"     ",
		"  #  ",
		"  #  ",
		" #   "
	}, {
		"  #  ",
		"  #  ",
		"     ",
		"     ",
		"     ",
		"     ",
		"     "
	}, {
		"#    ",
		"#    ",
		" #   ",
		"  #  ",
		"   # ",
		"    #",
		"    #"
	}, {
		"     ",
		"     ",
		"  #  ",
		"     ",
		"  #  ",
		"     ",
		"     "
	}, {
		" # # ",
		" # # ",
		"     ",
		"     ",
		"     ",
		"     ",
		"     "
	}, {
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  ",
		"  #  "
	}, {
		"     ",
		"     ",
		"     ",
		"     ",
		"  #  ",
		"  #  ",
		" #   "
	}, {
		"     ",
		"     ",
		"     ",
		"     ",
		"  #  ",
		"     ",
		"     "
	}, {
		"    #",
		"    #",
		"   # ",
		"  #  ",
		" #   ",
		"#    ",
		"#    "
	}, {
		"   # ",
		"  #  ",
		" #   ",
		"#    ",
		" #   ",
		"  #  ",
		"   # "
	}, {
		" #   ",
		"  #  ",
		"   # ",
		"    #",
		"   # ",
		"  #  ",
		" #   "
	}, {
		" ### ",
		"#   #",
		"    #",
		"   # ",
		"  #  ",
		"     ",
		"  #  "
	}, {
    "     ",
    "     ",
    "     ",
    "     ",
    "     ",
    "     ",
    "     "
	}
};

// "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-=!@#$%^&*()_+[]{};'\\:\"|,./<>? "
