import signal
import subprocess
import time
from python_tools.herminia_config import *
from python_tools.boardgenerator import generate_board_from_image

def retrieve_screenshot():
    cmd = ['monkeyrunner', 'python_tools/get_screenshot.py']
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    process.wait()

def solve_board():
    cmd = ['./herminia', '--dict', DICT_FILE, '--board', BOARD_TEXT_FILE, '--outwords', WORDS_FILE, '--outcoords', COORDINATES_FILE]
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    process.wait()

def play_game():
    cmd = ['monkeyrunner', 'python_tools/play_game.py']
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    process.wait()


def main():
    # Retrieve screenshot. This method captures the screen and pauses the game
    print('Retrieving screenshot...')
    retrieve_screenshot()
    print('Screenshot retrieved!')

    # Detect the letters from the board and generate a suitable board for the
    # C++ solving tool
    print('Generating text board...')
    generate_board_from_image(SCREENSHOT_FILE, BOARD_TEXT_FILE)
    print('Board generated!')

    # Use the C++ tool to find all the possible words and save the coordinates
    # to COORDINATES_FILE
    print('Finding all possible words...')
    solve_board()
    print('All words found!')

    # Resume the game and play it
    print('Now let\'s kick some asses')
    play_game()

if __name__ == "__main__":
    main()
