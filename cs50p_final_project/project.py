# import the necessary libraries
import os
import sys
import cv2 as cv
import numpy as np

# video class with image and audio parts and methds to extract them from the video file
class Video:
    def __init__(self, video_file):
        self._video_file = video_file
        self._image_part = None
        self._audio_part = None

    def extract_image_part(self):
        # code to extract image part from the video file
        cap = cv.VideoCapture(self._video_file)
        if not cap.isOpened():
            print("Error opening video file")
            return
        
        self.fps = cap.get(cv.CAP_PROP_FPS)
        self.frames = []
        while True:
            ret, frame = cap.read()
            if not ret:
                break
            self.frames.append(frame)

        cap.release()
        self._image_part = self.frames
        # print(f"Extracted {len(self.frames)} frames at {self.fps} frames per second.")

    def extract_audio_part(self):
        # code to extract audio part from the video file
        pass

# define main function
def main():
    input_video = input("Enter the path of the video file: ")
    if check_file_type(input_video):
        if check_if_video_file_exists(input_video):
            video = Video(input_video)
            video.extract_image_part()
            video.extract_audio_part()
            ascii_frames = change_frames_to_ascii(video._image_part)
            changed_audio = change_audio(video._audio_part)
            combine_audio_and_frames(ascii_frames, changed_audio)
            
        else:
            sys.exit("File doesn't exist on the system. Please check again.")
    else:
        sys.exit("Invalid video file. Please enter a valid video file with extensions .mp4, or .mov")

# define the functions that takes video file as input, check if it is valid and exists 
def check_file_type(video_file):
    if video_file.lower().endswith(('.mp4', '.mov')):
        return True
    else:
        return False
    
def check_if_video_file_exists(video_file):
    if os.path.isfile(video_file):
        return True
    else:
        return False


# define the functions that changes the frames of the video to ASCII art
def change_frames_to_ascii(frames):
    # code to change the frames of the video to ASCII art
    pass


# define the functions that changes the audio of the video as per the desire of the user
def change_audio(audio):
    # code to change the audio of the video as per the desire of the user
    pass

# define a function that combines the changed audio and the ASCII frames to create a new video file
def combine_audio_and_frames(ascii_frames, changed_audio):
    # code to combine the changed audio and the ASCII frames to create a new video file
    pass


if __name__ == "__main__":
    main()