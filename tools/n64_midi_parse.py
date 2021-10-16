#!/usr/bin/env python3
import sys
import mido
from collections import namedtuple

MixEvent = namedtuple("MixEvent", ['instrument', 'freq', 'volume', 'channel', 'start'], defaults=[0,0,0,0,0])

def calc_freq(n):
    return 440*2**((n-69)/12)

def find_empty_slot(music_slots):
    for i in range(len(music_slots)):
        if music_slots[i][0] == -1:
            return i

    return None

def find_slot(music_slots, note):
    for i in range(len(music_slots)):
        if music_slots[i][0] == note[0]:
            return i

    return None

def main():
    freq_table = []
    for i in range(128):
        freq_table.append(calc_freq(i))

    default_freq = freq_table[60]
    mid = mido.MidiFile('assets/Midi_Data_Test_3.mid')
    ticks_per_beat = mid.ticks_per_beat
    tempo = 0
    music_slots = [(-1, 0)]*16
    mix_stream = [[0]]
    for track in mid.tracks:
        inst_name = ""
        # Skip the kicker track lmms includes
        if track.name == "Kicker":
            continue
        for msg in track:
            if msg.type == 'set_tempo':
                tempo = msg.tempo
            elif msg.type == "instrument_name":
                inst_name = msg.name 
                inst_name = "Casio_SA_76_Piano1"
            elif msg.type == 'note_on':
                note_time = mido.tick2second(msg.time, ticks_per_beat, tempo)

                slot = find_empty_slot(music_slots)
                music_slots[slot] = (msg.note, note_time)

                # calculate freq we need for channel
                note_freq = freq_table[msg.note]
                freq_factor = note_freq / default_freq
                note_freq = 44100 * freq_factor
                note_volume = msg.velocity / 128.0

                mixev = MixEvent("&"+inst_name, note_freq, note_volume, slot, "true")

                if note_time != 0:
                    mix_stream.append([note_time])

                mix_stream[-1].append(mixev)


            elif msg.type == 'note_off':
                note_time = mido.tick2second(msg.time, ticks_per_beat, tempo)
                note = (msg.note, note_time)
                slot = find_slot(music_slots, note)
                music_slots[slot] = (-1, 0)

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

    out_string = """
MixEvent {song_name}_events[] = 
{{
{events}
}};

MixStream {song_name}_stream[] = 
{{
{stream}
}};
""".format(song_name=song_name, events=event_string, stream=stream_string)


    f = open("src/out.hpp", "w")
    f.write(out_string)
    f.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
