## About

Herminia is an automatic solver for Mezcladitos (WordCrack) boards.

WordCrack is a multi-platform mobile phone game (see http://www.mezcladitos.com/)

The idea is to find all the possible words which can be formed with adjacent 
letters in two minutes.

Herminia is composed by different subtools: a main script, an image-processing 
tool and a C++ solving tool.

The main script (run_herminia.py) is to be launched before a game starts. Once 
the game has started, press Enter.
The script will take a screenshot of the board and pause the game. It will then
invoke the image-processing tool and generate a txt file representing the board.

Herminia's C++ solver takes a text file representing the board to solve. This 
file must be formatted like:
* Four lines, each line composed of four tokens separated by a blank space.
* Each token represents a tile in the board.
* A tile is represented as <letter><value><multiplier_if_any>

See exampleboard.png and exampleboard.txt for an example.

Once this board has been automatically generated, the solving tool uses it to
generate two files. One contains the words and their score and the other one 
contains the coordinates of the tiles of the letters forming each word in
pixels. These values are, obviously, device dependent, and at the moment are set
up for a Moto G. It can easily be adapted by changing the values inside 
src/ScreenDimensions.h

The main script then takes the output coordinates and uses it to automatically
play the game for us, playing the most valuable words first.

## Building Herminia

The python scripts need no building, so just run "make" for the C++ tool

## Using Herminia

There two uses for Herminia.

The first one is to simply find the words, using the
C++ solver. In that case, run:
    ./herminia--dict <dictionary> --board <board> --outwords <words> --outcoords <coords>

The second one is to have Herminia also play for you. In that case, change the
values inside herminia_config.py for your setup (this includes screen-related 
values, so make sure you change them if using something different than a MotoG),
make sure that the C++ tool is adapted to your device's screen by setting the 
values inside src/ScreenDimensions.h, rebuild Herminia and run:
    python run_herminia.py

It is necessary to have mokeyrunner in the PATH, as well as having several 
Python modules installed.


## About MonkeyRunner and Jython

monkeyrunner runs on Jython, which is based on Python 2.5. That means that 
getting a regular Python script to work with it can be tedious (having to install
modules for 2.5, importing stuff from __future__, etc.). Therefore, for this 
project I decided to do something very hacky: run all the code in Python, and
create scripts for the monkeyrunner parts which are run as a subprocess.

This way, code which doesn't need monkeyrunner can use all the new Python 
features and there is no need to install modules for Python 2.5.
