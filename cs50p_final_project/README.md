# Audio & Image Processor

#### Video Demo:  <URL https://youtu.be/je-IpqPN6Bg>

A Python CLI tool that does two things:

1. **Image → ASCII Art** — converts any image to ASCII characters and prints it to the terminal
2. **Audio → Robotic Voice** — lowers the pitch and adds a ring modulation effect to any audio file

---

## How to Run

```bash
python project.py
```

You'll see:

```
Press 1 for Image to ASCII, Press 2 for Audio to Robotic:
```

### Option 1: Image to ASCII

Enter the path to a `.jpg`, `.jpeg`, or `.png` file. The program converts it to ASCII art and prints it in the terminal.

### Option 2: Audio to Robotic

Enter the path to a `.wav`, `.mp3`, or `.m4a` file. The program lowers the pitch by 4 semitones, adds a 40 Hz ring modulation effect, and saves the result as `changed.wav`.

---

## Supported Formats

| Type | Extensions |
|---|---|
| Image | `.jpg`, `.jpeg`, `.png` |
| Audio | `.wav`, `.mp3`, `.m4a` |

---

## Dependencies

Go to `requirements.txt` to see the names of libraries needed, for installing them run the following command:

```bash
pip install -r requirements
```

---

## How It Works

### Image to ASCII

1. Convert the image to grayscale
2. Downscale to 80 characters wide (with 0.55 height correction for terminal aspect ratio)
3. Map each pixel's brightness [0–255] to a character from the ramp `" .',:;clxkO0KXNW@"`

### Audio to Robotic

1. **Pitch shift** — time-stretch the audio by α = 2^(-4/12), then resample back
2. **Ring modulation** — multiply the audio by a 40 Hz sinusoid, creating sidebands at f₀ ± 40 Hz around every frequency component. This produces the metallic/robotic character

---

## Testing

```bash
pip install pytest
python -m pytest test_project.py -v
```

---

## Project Files

```
project.py         ← all code
test_project.py    ← all tests
```