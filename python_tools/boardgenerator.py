#!/usr/bin/env python
# -*- coding: utf-8 -*-

try:
    import Image
except ImportError:
    from PIL import Image
from scipy.misc import fromimage
from scipy.misc import imread
from scipy.signal.signaltools import correlate2d
import numpy
import pytesseract
import scipy
import scipy.cluster
import scipy.misc
import sys
from herminia_config import *


# Returns a dictionary containing the values of the letters in a certain 
# language
def get_values(language):
    if language != 'esp':
        raise ValueError('Invalid language ' + language)

    letter_values = {}
    letter_values['A'] = 1
    letter_values['B'] = 3
    letter_values['C'] = 3
    letter_values['D'] = 2
    letter_values['E'] = 1
    letter_values['F'] = 4
    letter_values['G'] = 2
    letter_values['H'] = 4
    letter_values['I'] = 1
    letter_values['J'] = 8
    letter_values['K'] = 4
    letter_values['L'] = 1
    letter_values['M'] = 3
    letter_values['N'] = 1
    letter_values['Ñ'] = 8
    letter_values['O'] = 1
    letter_values['P'] = 3
    letter_values['Q'] = 5
    letter_values['R'] = 1
    letter_values['S'] = 1
    letter_values['T'] = 1
    letter_values['U'] = 1
    letter_values['V'] = 4
    letter_values['W'] = 8
    letter_values['X'] = 8
    letter_values['Y'] = 4
    letter_values['Z'] = 10
    return letter_values

def prepare_image_for_correlation(im):
    letterArray = fromimage(im.convert('RGB'))
    # Black and white
    letterArray = scipy.inner(letterArray, [299, 587, 114]) / 1000.0
    # Normalize
    letterArray = (letterArray - letterArray.mean()) / letterArray.std()
    return letterArray


# If a letter could not be properly detected or it was an N, invoke this method.
# It will compare the letter with pre-recorded screenshots of the letters which
# most commonly fail recognition and determine which one it is by correlation
def detect_irregular_letter(letter):
    letter = prepare_image_for_correlation(letter)

    letter_i = prepare_image_for_correlation(Image.open(SAVED_LETTERS_DIR+'i.png'))
    letter_q = prepare_image_for_correlation(Image.open(SAVED_LETTERS_DIR+'q.png'))
    letter_n = prepare_image_for_correlation(Image.open(SAVED_LETTERS_DIR+'n.png'))
    letter_enie = prepare_image_for_correlation(Image.open(SAVED_LETTERS_DIR+'enie.png'))

    candidates = {}
    candidates['I'] = letter_i
    candidates['Q'] = letter_q
    candidates['N'] = letter_n
    candidates['Ñ'] = letter_enie

    current_candidate = None
    max_correlation = 0

    for candidate_letter,candidate_letter_array in candidates.iteritems():
        correlation = correlate2d(letter, candidate_letter_array, mode='same').max()
        if correlation > max_correlation:
            max_correlation = correlation
            current_candidate = candidate_letter

    return current_candidate


# Returns a numpy array representing the RGB components of the most predominant 
# color
def get_predominant_color(image):
    NUM_CLUSTERS = 5
    im = image.convert('RGB')

    # Convert to numpy array
    ar = scipy.misc.fromimage(im)

    # Get dimensions
    shape = ar.shape

    # Convert to bidimensional array of width x height rows and 3 columns (RGB)
    ar = ar.reshape(scipy.product(shape[:2]), shape[2])

    # Find cluster centers and their distortions
    # codes contains the RGB value of the centers
    codes, dist = scipy.cluster.vq.kmeans(ar.astype(float), NUM_CLUSTERS)

    # Maps all the pixels in the image to their respective centers
    vecs, dist = scipy.cluster.vq.vq(ar, codes)

    # Counts the occurances of each color (NUM_CLUSTER different colors after the mapping)
    counts, bins = scipy.histogram(vecs, len(codes))

    # Find most frequent color
    index_max = scipy.argmax(counts)
    peak = codes[index_max]

    return peak.astype(int)

# Finds how different two colors are (see http://www.compuphase.com/cmetric.htm)
# The lower the value, the more similar the colors
def color_difference(color1, color2):
    # Mean of red
    red_mean = (color1[0] + color2[0])/2
    red = color1[0] - color2[0]
    green = color1[1] - color2[1]
    blue = color1[2] - color2[2]
    return (((512+red_mean)*red*red)/256) + 4*green*green + (((767-red_mean)*blue*blue)/256)

# Determine whether the letter has a word or letter modifier. It does this by
# finding the predominant color in the top left corner of the tile.
# Beige is no modifier; blue is DL; green is TL; orange is DW; and red is TW
# It cannot be done using OCR because the letters are too small
def get_modifier(letter):
    modifiers = {}
    modifiers[''] = numpy.array([239, 237, 221])
    modifiers['DL'] = numpy.array([49, 86, 229])
    modifiers['TL'] = numpy.array([115, 165, 56])
    modifiers['DW'] = numpy.array([246, 134, 33])
    modifiers['TW'] = numpy.array([251, 61, 49])

    # Crop the top left corner
    region_of_interest = letter.crop((MODIFIER_IN_TILE_LEFT, MODIFIER_IN_TILE_TOP, MODIFIER_IN_TILE_RIGHT, MODIFIER_IN_TILE_BOTTOM))

    predominant = get_predominant_color(region_of_interest)

    max_similarity = None
    current_modif = None

    for modif,color in modifiers.iteritems():
        similarity = color_difference(color, predominant)
        if max_similarity == None or similarity < max_similarity:
            max_similarity = similarity
            current_modif = modif

    return current_modif



def generate_board_from_image(board_filename, output_filename):
    image = Image.open(board_filename)
    letter_values = get_values(LANGUAGE)
    with open(output_filename, 'wb') as output:
        for row in range(0,VERTICAL_TILES):
            for col in range(0,HORIZONTAL_TILES):
                top = TOPLEFT_TILE_Y_COORD + row*DISTANCE_BETWEEN_TILES_IN_BOTH_DIRECTIONS
                bottom = top + TILE_DIMENSION
                left = TOPLEFT_TILE_X_COORD + col*DISTANCE_BETWEEN_TILES_IN_BOTH_DIRECTIONS
                right = left + TILE_DIMENSION

                # Crop this tile
                tile = image.crop((left, top, right, bottom))

                # Extract the letter in this tile
                letter = tile.crop((LETTER_IN_TILE_LEFT, LETTER_IN_TILE_TOP, LETTER_IN_TILE_RIGHT, LETTER_IN_TILE_BOTTOM))
                # OCR the cropped letter
                ocr_result = pytesseract.image_to_string(letter,config='-psm 10')

                # If the letter was not rightly recognised (or if we are using Spanish
                # and this was an enye) try to use correlation to find the right letter
                if len(ocr_result) != 1 or (ocr_result == 'N' and LANGUAGE == 'esp'):
                    ocr_result = detect_irregular_letter(letter)

                # Get the letter's modifier
                modifier = get_modifier(tile)

                output.write(ocr_result.upper() + str(letter_values[ocr_result.upper()]) + modifier + ' ')
            output.write('\n')

