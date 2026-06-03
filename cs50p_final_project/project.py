# import the necessary libraries
import os
import sys
import cv2 as cv
import numpy as np
from moviepy import AudioFileClip
import librosa
import soundfile as sf

# define main function
def main():

    input_file = input("Press 1 for Image to ASCII, Press 2 for Audio to Robotic: ")
    if input_file == "1":
        file_path = input("Enter the path to the image file: ")
        if check_file_exists(file_path):
            if check_file_type(file_path) == "image":
                    img = cv.imread(file_path)
                    if img is None:
                        sys.exit("Could not read the image file.")
                    ascii_lines = image_to_ascii(img)
                    print("\n".join(ascii_lines))
            elif check_file_type(file_path) == "audio":
                sys.exit("Audio files are not supported for ASCII conversion. Please provide an image file.")
            else:
                sys.exit("Unsupported file type. Please provide an image or audio file.")
        else:
            sys.exit("File does not exist. Please provide a valid file path.")

    elif input_file == "2":
        file_path = input("Enter the path to the audio file: ")
        if check_file_exists(file_path):
            if check_file_type(file_path) == "audio":
                clip = AudioFileClip(file_path)
                audio = clip.to_soundarray()
                sr = clip.fps
                clip.close()
                if audio.ndim == 2: # Convert stereo to mono by averaging the 2 channels
                    audio = audio.mean(axis=1)
                deep_pitch_audio = change_pitch(audio, sr, semitones=-4)
                robotic_audio = add_robotic_effect(deep_pitch_audio, sr, freq=40)
                sf.write("changed.wav", robotic_audio.astype(np.float32), sr)
            elif check_file_type(file_path) == "image":
                sys.exit("Image files are not supported for robotic audio effect. Please provide an audio file.")
            else:
                sys.exit("Unsupported file type. Please provide an image or audio file.")
        else:
            sys.exit("File does not exist. Please provide a valid file path.")
    else:
        sys.exit("Invalid choice. Please enter 1 or 2.")

# define the functions that takes video file as input, check if it is valid and exists 
def check_file_type(input_file):
    audio_extensions = (".wav", ".mp3", ".m4a")
    image_extensions = (".jpg", ".jpeg", ".png")
    if input_file.lower().endswith(image_extensions):
        return "image"
    elif input_file.lower().endswith(audio_extensions):
        return "audio"
    else:
        return False
    
def check_file_exists(file_path):
    if os.path.isfile(file_path):
        return True
    else:
        return False

# define the function that converts the pixel brightness to ASCII characters
ASCII_RAMP = " .',:;clxkO0KXNW@"

def pixel_brightness_to_ascii(pixel_brightness):
    ramp_length = len(ASCII_RAMP)
    index = round(pixel_brightness / 255 * (ramp_length - 1)) # Scale the pixel brightness to the range of the ASCII ramp
    return ASCII_RAMP[index]

def image_to_ascii(frame):
    new_width = 80
    gray_frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    height, width = gray_frame.shape
    new_height = int(new_width * (height / width) * 0.55)
    resized_frame = cv.resize(gray_frame, (new_width, new_height), interpolation=cv.INTER_AREA)

    lines = []
    for row in resized_frame:
        lines.append("".join(pixel_brightness_to_ascii(int(pixel)) for pixel in row))
    return lines

# define the function that changes the pitch of the audio as per the desire of the user
def change_pitch(audio, sr, semitones):
    alpha = 2 ** (semitones / 12) # Calculate the time-stretching factor
    streched_length = librosa.effects.time_stretch(audio, rate=alpha)
    target_sr = int(sr * alpha) # Calculate the new sampling rate after time-stretching
    resampled_audio = librosa.resample(streched_length, orig_sr=target_sr, target_sr=sr) # Resample the audio
    return resampled_audio

# define the function that adds the robotic effect to the audio
def add_robotic_effect(audio, sr, freq=40): # this adds two sidebands to the audio at a desired frequency, creating a robotic effect
    t = np.arange(len(audio)) / sr
    carrier = np.sin(2 * np.pi * freq * t)
    modulated_audio = audio * carrier
    return np.clip(modulated_audio, -1, 1)


if __name__ == "__main__":
    main()