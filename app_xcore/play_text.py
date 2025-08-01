import numpy as np
import sounddevice as sd
from scipy.io import wavfile

def read_int16_text_file(file_path):
    valid_samples = []
    with open(file_path, 'r') as f:
        for line_num, line in enumerate(f, 1):
            try:
                value = int(line.strip())
                if -32768 <= value <= 32767:
                    valid_samples.append(value)
                else:
                    print(f"Line {line_num}: Value out of int16 range, skipped.")
            except ValueError:
                print(f"Line {line_num}: Not a valid integer, skipped.")
    return np.array(valid_samples, dtype=np.int16)

def play_audio(samples, sample_rate=44100):
    if len(samples) == 0:
        print("No valid audio samples to play.")
        return
    float_samples = samples.astype(np.float32) / 32768.0
    sd.play(float_samples, samplerate=sample_rate)
    sd.wait()

def save_wav(samples, sample_rate, output_path):
    wavfile.write(output_path, sample_rate, samples)
    print(f"Saved WAV file to: {output_path}")

if __name__ == "__main__":
    import argparse
    import os

    parser = argparse.ArgumentParser(description="Play and save audio from a text file of 16-bit signed integers (LF-separated).")
    parser.add_argument("file", help="Path to the text file")
    parser.add_argument("--rate", type=int, default=44100, help="Sample rate (default: 44100)")
    parser.add_argument("--out", type=str, default=None, help="Optional output WAV file name")

    args = parser.parse_args()

    samples = read_int16_text_file(args.file)

    if args.out:
        save_wav(samples, args.rate, args.out)
    else:
        # Default: same name as input file but with .wav
        base = os.path.splitext(os.path.basename(args.file))[0]
        default_out = f"{base}.wav"
        save_wav(samples, args.rate, default_out)

    play_audio(samples, sample_rate=args.rate)
