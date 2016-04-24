from com.android.monkeyrunner import MonkeyRunner, MonkeyDevice

import time
import subprocess

from herminia_config import SCREENSHOT_FILE, PAUSE_BUTTON, SCREENSHOT_EXTENSION, WAIT_TIME

def tap_button(device, button):
    device.touch(int(button[0]), int(button[1]), MonkeyDevice.DOWN)
    time.sleep(WAIT_TIME)
    device.touch(int(button[0]), int(button[1]), MonkeyDevice.UP)

def main():
    device = MonkeyRunner.waitForConnection()

    MonkeyRunner.input('Press enter as soon as the game starts')

    # Capture screen, pause game and save the screenshot
    board = device.takeSnapshot()
    tap_button(device, PAUSE_BUTTON)
    board.writeToFile(SCREENSHOT_FILE, SCREENSHOT_EXTENSION)

if __name__ == "__main__":
    main()
