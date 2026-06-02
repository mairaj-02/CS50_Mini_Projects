# Video-to-ASCII & Audio Manipulation

A single-file Python tool that takes a short video clip (.mov or .mp4, ≤15 s), converts each frame to ASCII art, and manipulates the audio track (pitch, volume, speed). The output is a playable ASCII-rendered video with the modified audio.

---

## What It Does

1. **ASCII Art Video** — extracts frames, downsamples, maps pixel brightness to ASCII characters, and plays the result in the terminal (or saves as text).
2. **Audio Manipulation** — extracts the audio track, applies pitch shifting, volume scaling, and speed changes, then recombines with the ASCII video.

---

## Project Files

```
project.py           ← everything: I/O, ASCII conversion, audio ops, CLI
test_project.py      ← all tests
```

That's it. No packages, no submodules, no registry.

---

## Project Structure Inside `project.py`

The file is organised into four sections:

| Section | Functions | What it does |
|---|---|---|
| **Video I/O** | `extract_frames()`, `get_video_info()`, `save_ascii_video()` | Reads .mov/.mp4 via OpenCV, returns frames as numpy arrays; writes ASCII frames to text file |
| **ASCII Conversion** | `frame_to_ascii()`, `brightness_to_char()`, `play_ascii_video()` | Downsamples frame, converts grayscale brightness to ASCII character set, plays in terminal |
| **Audio Manipulation** | `extract_audio()`, `shift_pitch()`, `change_volume()`, `change_speed()`, `save_audio()` | Extracts audio via moviepy, modifies pitch/volume/speed via librosa & numpy, saves via soundfile |
| **CLI** | `main()` | argparse entry point — ties it all together |

---

## Mathematical Background

### ASCII Art Conversion

Each video frame is a 3-channel BGR image of shape (H, W, 3) with values in [0, 255]. Conversion to ASCII art follows three steps:

**1. Grayscale conversion**

A single luminance value per pixel is computed as a weighted sum of the colour channels, matching human brightness perception:

L = 0.299·R + 0.587·G + 0.114·B

OpenCV's `cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)` applies this internally.

**2. Downscaling**

A frame of size (H, W) is resized to (new_h, new_w) where `new_w` determines the ASCII art width and `new_h = new_w * (H/W) * 0.55` — the 0.55 factor compensates for the fact that monospace terminal characters are roughly twice as tall as they are wide. Without this correction, ASCII output appears vertically stretched. Resizing uses `cv2.resize()` with `INTER_AREA` interpolation, which averages pixel neighbourhoods and is optimal for downscaling (anti-aliasing).

**3. Brightness-to-character mapping**

The 8-bit grayscale value [0, 255] is mapped to a character from a ramp ordered by visual density:

```
ASCII_RAMP = " .:-=+*#%@"
```

Index = round(L / 255 × (len(RAMP) - 1)). A pixel at L=0 maps to space (black/background), L=255 maps to `@` (densest character). The ramp length determines tonal resolution — 10 characters gives ~25 levels of distinguishable brightness on a terminal, which is sufficient for small ASCII art.

### Pitch Shifting

Pitch shifting raises or lowers the perceived frequency of the audio without changing its duration. The time-stretch + resample method works as follows:

1. **Time-stretch** the signal by a factor α = 2^(n_steps/12), where n_steps is the shift in semitones. This changes duration by α but preserves frequency relationships. librosa's `time_stretch` uses a phase vocoder: it computes the STFT, interpolates the phase between frames to maintain coherence at the new hop rate, then inverse-transforms.

2. **Resample** back to the original length using `librosa.resample()`, which applies a polyphase FIR anti-aliasing filter. This restores the original duration while the frequency content has been scaled by α.

The net effect: a signal at frequency f₀ is shifted to f₀ × α. For n_steps = +12 (one octave up), α = 2, so all frequencies double.

### Volume Scaling

Amplitude scaling is a direct multiplication: y_out[n] = G · y_in[n], where G is the gain factor. A gain of 2.0 doubles the amplitude (+6 dB); 0.5 halves it (-6 dB). The relationship between linear gain and decibels is:

G_lin = 10^(G_dB / 20)

After scaling, the signal is clipped to [-1.0, 1.0] to prevent wraparound distortion when saving to 16-bit PCM.

### Speed Change

Speed change is a resampling operation: to play the audio at k× speed, the signal is resampled from sr to sr/k. This shortens the duration by factor k and shifts all frequencies up by factor k (the "chipmunk" effect). Unlike pitch shifting, speed change alters both tempo and pitch simultaneously.

`librosa.resample(y, orig_sr=sr, target_sr=int(sr / k))` applies a polyphase FIR filter for anti-aliasing before decimation, preventing spectral images from folding into the audible range.

---

## CLI Usage

```bash
# ASCII art video only (no audio changes)
python project.py input.mp4 --ascii --width 80

# Pitch shift up 5 semitones
python project.py input.mp4 --pitch 5

# Double the volume
python project.py input.mp4 --volume 2.0

# Play at 1.5× speed
python project.py input.mp4 --speed 1.5

# All combined: ASCII + pitch + volume
python project.py input.mp4 --ascii --width 100 --pitch -3 --volume 1.5

# Save ASCII frames to file instead of playing
python project.py input.mp4 --ascii --width 80 --save-ascii output.txt
```

---

## Dependencies

```
numpy>
opencv-python
moviepy
librosa
soundfile
```

Install all at once:

```bash
pip install -r requirements.txt
```

---

## Testing

```bash
python -m pytest test_project.py -v
```

`test_project.py` generates a tiny synthetic video fixture programmatically (no external files needed) and tests:

| Test | What it verifies |
|---|---|
| `test_grayscale_conversion` | BGR frame → grayscale produces correct shape and value range |
| `test_ascii_output_dimensions` | ASCII frame has the requested width and proportionally correct height |
| `test_brightness_mapping` | Black pixel → space, white pixel → densest character |
| `test_volume_scaling` | Gain=2.0 doubles amplitude (within clipping) |
| `test_pitch_shift_preserves_length` | Pitch-shifted audio has same number of samples as input |
| `test_speed_change_length` | 2× speed produces half the samples |
| `test_frame_extraction` | Extracted frame count matches video frame count |
| `test_ascii_play_no_crash` | ASCII playback runs without error on a 3-frame video |