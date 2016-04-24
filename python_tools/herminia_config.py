# Modify this file to adjust the behaviour of Herminia

#####################################################
#####################################################
################# General settings ##################
#####################################################
#####################################################

HORIZONTAL_TILES = 4
VERTICAL_TILES = 4
LANGUAGE = 'esp'
# Play dumb so that we are not too suspicious
HUMAN_MODE = 1

#####################################################
#####################################################
################## Time constants ###################
#####################################################
#####################################################

# Seconds to wait before pressing and releasing a button
WAIT_TIME=0.1
# Seconds between submitting a word and starting tapping on the next
TIME_BETWEEN_WORDS=0.5

#####################################################
#####################################################
################ File name constants ################
#####################################################
#####################################################

SCREENSHOT_EXTENSION = 'png'
SCREENSHOT_NAME = 'screenshot.' + SCREENSHOT_EXTENSION
# Location of the screenshot in this machine
SCREENSHOT_FILE = '/home/ic/.tmp/'+SCREENSHOT_NAME

# File containing the dictionary
DICT_FILE = '/home/ic/projects/WordCrackHerminia/dict/esp.txt'
# File for the text-based board
BOARD_TEXT_FILE = '/home/ic/.tmp/board.txt'
# Output file with words and their scores
WORDS_FILE = '/home/ic/.tmp/results.txt'
# Output file with the coordinates of the words
COORDINATES_FILE = '/home/ic/.tmp/coords.txt'

# Directory with the letters for correlation
SAVED_LETTERS_DIR = '/home/ic/projects/WordCrackHerminia/python_tools/'


#####################################################
#####################################################
################## Screen settings ##################
#####################################################
#####################################################

# The X coordinate of the top left tile
TOPLEFT_TILE_X_COORD = 22
# The Y coordinate of the top left tile
TOPLEFT_TILE_Y_COORD = 355
# The dimension of the whole tile (modifier, letter and score included)
TILE_DIMENSION = 154
# How far apart the different tiles are in both directions in pixels
DISTANCE_BETWEEN_TILES_IN_BOTH_DIRECTIONS = 175

# The Y coordinate of the upper limit of letter inside the cropped tile
LETTER_IN_TILE_TOP = 22
# The X coordinate of the left limit of letter inside the cropped tile
LETTER_IN_TILE_LEFT = 38
# The Y coordinate of the bottom limit of letter inside the cropped tile
LETTER_IN_TILE_BOTTOM = 128
# The X coordinate of the right limit of letter inside the cropped tile
LETTER_IN_TILE_RIGHT = 115

# The Y coordinate of the upper limit of modifier box inside the cropped tile
MODIFIER_IN_TILE_TOP = 0
# The X coordinate of the left limit of modifier box inside the cropped tile
MODIFIER_IN_TILE_LEFT = 0
# The Y coordinate of the bottom limit of modifier box inside the cropped tile
MODIFIER_IN_TILE_BOTTOM = 33
# The X coordinate of the right limit of modifier box inside the cropped tile
MODIFIER_IN_TILE_RIGHT = 33

# Buttons coordinates in the screen
PAUSE_BUTTON = (669, 104)
RESUME_BUTTON = (376, 506)
CONFIRM_WORD_BUTTON = (647, 297)

