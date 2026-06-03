import os
import tempfile
import numpy as np
import cv2 as cv
import soundfile as sf
import pytest

from project import (
    check_file_type,
    check_file_exists,
    pixel_brightness_to_ascii,
    image_to_ascii,
    change_pitch,
    add_robotic_effect,
    ASCII_RAMP,
)

def test_check_file_type():
    assert check_file_type("photo.jpg") == "image"
    assert check_file_type("photo.jpeg") == "image"
    assert check_file_type("photo.png") == "image"

    assert check_file_type("song.wav") == "audio"
    assert check_file_type("song.mp3") == "audio"
    assert check_file_type("song.m4a") == "audio"

    assert check_file_type("video.mp4") == False
    assert check_file_type("doc.pdf") == False

    assert check_file_type("Photo.JPG") == "image"
    assert check_file_type("Song.MP3") == "audio"

def test_check_file_exists():
    # create a temp file that definitely exists
    with tempfile.NamedTemporaryFile(delete=False) as file:
        path = file.name
    assert check_file_exists(path) == True
    os.unlink(path)
    assert check_file_exists("non_existent_file.txt") == False


def test_pixel_brightness_to_ascii():
    assert pixel_brightness_to_ascii(0) == ASCII_RAMP[0]  # darkest
    assert pixel_brightness_to_ascii(255) == ASCII_RAMP[-1]  # brightest
    assert pixel_brightness_to_ascii(128) == ASCII_RAMP[len(ASCII_RAMP) // 2]  # middle brightness
    assert pixel_brightness_to_ascii(64) == ASCII_RAMP[len(ASCII_RAMP) // 4]  # quarter brightness


def test_image_to_ascii():
    black_img = np.zeros((100, 100, 3), dtype=np.uint8)
    white_img = np.full((100, 100, 3), 255, dtype=np.uint8)

    result = image_to_ascii(black_img)

    assert isinstance(result, list)
    assert all(isinstance(line, str) for line in result)
    assert len(result[0]) == 80
    assert len(result) == int(80 * (100 / 100) * 0.55)
    assert all(ch == ASCII_RAMP[0] for line in result for ch in line)

    result = image_to_ascii(white_img)
    assert all(ch == ASCII_RAMP[-1] for line in result for ch in line)

def test_change_pitch():
    sr = 22050
    t = np.linspace(0, 1, sr, endpoint=False)
    audio = np.sin(2 * np.pi * 440 * t).astype(np.float64)

    shifted_down = change_pitch(audio, sr, semitones=-4)
    shifted_up = change_pitch(audio, sr, semitones=4)
    shifted_zero = change_pitch(audio, sr, semitones=0)

    # all should return non-empty numpy arrays
    assert isinstance(shifted_down, np.ndarray)
    assert isinstance(shifted_up, np.ndarray)
    assert isinstance(shifted_zero, np.ndarray)
    assert len(shifted_down) > 0
    assert len(shifted_up) > 0
    assert len(shifted_zero) > 0


def test_add_robotic_effect():
    sr = 22050
    t = np.linspace(0, 1, sr, endpoint=False)
    audio = np.sin(2 * np.pi * 440 * t).astype(np.float64)

    result = add_robotic_effect(audio, sr, freq=40)
    assert len(result) == len(audio)
    assert result.min() >= -1.0
    assert result.max() <= 1.0

    result_20 = add_robotic_effect(audio, sr, freq=20)
    result_80 = add_robotic_effect(audio, sr, freq=80)
    assert not np.allclose(result_20, result_80)

    loud = np.ones(sr)
    clipped = add_robotic_effect(loud, sr, freq=40)
    assert clipped.min() >= -1.0
    assert clipped.max() <= 1.0







