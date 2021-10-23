#!/usr/bin/env python3
import sys
import mido
import argparse
import bisect
import os
import subprocess
from collections import namedtuple

MixEvent = namedtuple("MixEvent", ['instrument', 'freq', 'volume', 'channel', 'start'], defaults=[0,0,0,0,0])
Instrument = namedtuple("Instrument", ['path', 'freq', 'name'])

# Note these mapping match the names used in Logic X
# If you look up a midi note matching table like on this website
# https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
# You will see the the octaves are all offset by 1
note_map = {
        "D2" : 146.83,
        "C2" : 130.81,
        "C3" : 261.63,
        "C4" : 523.25,
        "C5" : 1046.50,
        "F#2": 185.00,
        "F#3": 369.99,
        "F#4": 739.99,
        "C#3": 277.18,
        "D3" : 293.66,
        "D#3": 311.13,
        "D#2": 155.56,
        "E3" : 329.63,
        "F3" : 349.23,
        "E2" : 164.81,
        }

class TrackMsg:
    def __init__(self, time, inst, msg):
        self.time = time
        self.inst = inst
        self.msg = msg

    def __lt__(self, other):
        return self.time < other.time

def calc_freq(n):
    return 440*2**((n-69)/12)

def find_empty_slot(music_slots):
    for i in range(len(music_slots)):
        if music_slots[i] == None:
            return i

    raise Exception("Could not find empty slot")
    return None

def find_slot(music_slots, note):
    for i in range(len(music_slots)):
        if music_slots[i] != None and music_slots[i].inst == note.inst and music_slots[i].msg.note == note.msg.note:
            return i

    raise Exception("Could not find matching note")
    return None

def process_instrument(inst_name, samples):
    inst_table = []
    for root, dirs, files in os.walk(os.path.abspath("{}/{}".format(samples, inst_name))):
        for file in files:
            sample_name = file.replace("#", "Sharp").split(".")[0]
            # Get the note name
            note = file.split(".")[0].split("_")[-1]
            inst_table.append(Instrument(os.path.join(root,file), note_map[note], sample_name))

    return inst_table

def find_closest_sample(samples, freq):
    min_diff = 1000000000.0
    best_sample = None
    for sample in samples:
        diff = abs(sample.freq-freq)
        if diff < min_diff:
            min_diff = diff
            best_sample = sample

    return best_sample

def combine_tracks(tracks):
    out_track = []
    inst_name = ""
    tempo = 0
    current_time = 0
    for track in tracks:
        current_time = 0
        for msg in track:
            if msg.type == "instrument_name":
                inst_name = msg.name 
            elif msg.type == "channel_prefix":
                current_time = msg.time - 17280
            elif msg.type == "note_on" or msg.type == "note_off" or msg.type == "set_tempo":
                current_time += msg.time
                value = TrackMsg(current_time, inst_name, msg)
                bisect.insort_left(out_track, value)

    return out_track

def main():
    parser = argparse.ArgumentParser("n64_midi_parse")
    parser.add_argument("input", type=str, help="MIDI input file") 
    parser.add_argument("output", type=str, help="output file name")
    parser.add_argument("--samples", type=str, help="Sample Path")
    args = parser.parse_args()

    freq_table = []
    for i in range(128):
        freq_table.append(calc_freq(i))

    mid = mido.MidiFile(args.input)
    ticks_per_beat = mid.ticks_per_beat
    tempo = 576923
    music_slots = [None]*16
    mix_stream = [[0]]
    insts = {}
    sorted_tracks = combine_tracks(mid.tracks)
    sample_list = []
    for index, element in enumerate(sorted_tracks):
        prev_time = 0 if index == 0 else sorted_tracks[index-1].time
        time = element.time
        inst_name = element.inst
        msg = element.msg

        offset_time = time - prev_time

        if msg.type == "set_tempo":
            tempo = msg.tempo
        elif msg.type == "note_on":
            if inst_name not in insts:
                insts[inst_name] = process_instrument(inst_name, args.samples)

            note_time = mido.tick2second(offset_time, ticks_per_beat, tempo)

            slot = find_empty_slot(music_slots)
            music_slots[slot] = element

            note_freq = freq_table[msg.note]
            sample = find_closest_sample(insts[inst_name], note_freq)

            if sample not in sample_list:
                sample_list.append(sample)

            freq_factor = note_freq / sample.freq
            note_freq = 44100 * freq_factor
            note_volume = msg.velocity / 128.0

            mixev = MixEvent("&"+sample.name, note_freq, note_volume, slot, "true")

            # If this mix event starts at a new timestamp create a new entry
            # in the mix stream list
            if note_time != 0:
                mix_stream.append([note_time])

            # Append mix stream event to end of last element of mix stream list
            mix_stream[-1].append(mixev)

        elif msg.type == "note_off":
            note_time = mido.tick2second(offset_time, ticks_per_beat, tempo)

            slot = find_slot(music_slots, element)
            music_slots[slot] = None

            mixev = MixEvent("NULL", 0, 0, slot, "false")

            if note_time != 0:
                mix_stream.append([note_time])

            mix_stream[-1].append(mixev)

    stream_string = ""
    event_string = ""
    event_counter = 0
    song_name = "test_song"
    for event in mix_stream:
        stream_string += "{{.delta_time={}, .events={}_events+{}, .num_events={}}},".format(
                         event[0], song_name, event_counter, len(event)-1)
        event_counter += len(event)-1

        for e in event[1:]:
            event_string += "{{.sample={}, .freq={}, .volume={}, .channel={}, .start={}}},".format(
                             e.instrument, e.freq, e.volume, e.channel, e.start)

    samples_names_str = ""
    samples_load_str = ""
    paths_to_add = ""
    for sample in sample_list:
        file = os.path.basename(sample.path)
        file = file.split(".")[0] + ".wav64"
        file = file.replace("#", "Sharp")
        samples_names_str += "wav64_t {};\n".format(sample.name)
        samples_load_str += "wav64_open(&{}, \"{}\");\n".format(sample.name, file)

        # Convert sample to wav
        wav_file = file.split(".")[0] + ".wav"
        output_path_tmp = "/tmp/{}".format(wav_file)
        subprocess.run(["ffmpeg", "-y", "-i", sample.path, output_path_tmp])

        # Convert wav to wav64
        subprocess.run(["audioconv64", "-o", "filesystem", output_path_tmp])

        paths_to_add += "\tfilesystem/{} \\\n".format(file)


    print("\n\nAdd following lines to makefile")
    print(paths_to_add)
    out_string = """
#include <libdragon.h>

{sample_defs}

MixEvent {song_name}_events[] = 
{{
{events}
}};

MixStream {song_name}_stream[] = 
{{
{stream}
}};

void {song_name}_load()
{{
{sample_load}
}}
""".format(song_name=song_name, events=event_string, stream=stream_string, sample_defs=samples_names_str, sample_load=samples_load_str)

    f = open(args.output, "w")
    f.write(out_string)
    f.close()

    return 0

if __name__ == "__main__":
    sys.exit(main())
