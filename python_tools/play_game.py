from __future__ import with_statement
from com.android.monkeyrunner import MonkeyRunner, MonkeyDevice

import random
import subprocess
import time

from herminia_config import *

def tap_button(device, button):
    device.touch(int(button[0]), int(button[1]), MonkeyDevice.DOWN)
    time.sleep(WAIT_TIME)
    device.touch(int(button[0]), int(button[1]), MonkeyDevice.UP)

def read_words_from_file():
    with open(COORDINATES_FILE) as f:
        content = f.readlines()

    words = []
    for line in content:
        separate_coords = line.split()
        word = []
        for x,y in zip(separate_coords[0::2], separate_coords[1::2]):
            word.append((x,y))
        words.append(word)

    return words

def play_word(device, word):
    for letter in word:
        # Touch the screen
        tap_button(device, letter)
        time.sleep(WAIT_TIME)

    # Confirm word
    tap_button(device, CONFIRM_WORD_BUTTON)


def main():
    device = MonkeyRunner.waitForConnection()

    # Read the coordinates of the words from COORDINATES_FILE
    words = read_words_from_file()

    # Resume game
    tap_button(device, RESUME_BUTTON)
    time.sleep(TIME_BETWEEN_WORDS)

    end_time = time.time() + 120

    if HUMAN_MODE == 0:
        # Play best words first
        for word in words:
            play_word(device, word)
            time.sleep(TIME_BETWEEN_WORDS)

            # Stop playing if the game is over
            if (time.time() > end_time):
                break
    else:
        # Try not to look suspicious by playing words in random order and taking
        # longer between words
        while (len(words) != 0 and time.time() < end_time):
            word = random.choice(words)
            # Do not play words twice
            words.remove(word)
            play_word(device, word)
            time.sleep(TIME_BETWEEN_WORDS*2)

if __name__ == "__main__":
    main()
