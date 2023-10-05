import mido

midi_to_freq=[
    16,
    17,
    18,
    19,
    20,
    21,
    23,
    24,
    25,
    27,
    29,
    30,
    32,
    34,
    36,
    38,
    41,
    43,
    46,
    49,
    51,
    55,
    58,
    61,
    65,
    69,
    73,
    77,
    82,
    87,
    92,
    98,
    103,
    110,
    116,
    123,
    130,
    138,
    146,
    155,
    164,
    174,
    185,
    196,
    207,
    220,
    233,
    246,
    261,
    277,
    293,
    311,
    329,
    349,
    369,
    392,
    415,
    440,
    466,
    493,
    523,
    554,
    587,
    622,
    659,
    698,
    739,
    783,
    830,
    880,
    932,
    987,
    1046,
    1108,
    1174,
    1244,
    1318,
    1396,
    1479,
    1567,
    1661,
    1760,
    1864,
    1975,
    2093,
    2217,
    2349,
    2489,
    2637,
    2793,
    2959,
    3135,
    3322,
    3520,
    3729,
    3951,
    4186,
    4434,
    4698,
    4978,
    5274,
    5587,
    5919,
    6271,
    6644,
    7040,
    7458,
    7902,
    8372,
    8869,
    9397,
    9956,
    10548,
    11175,
    11839,
    12543,
    13289,
    14080,
    14917,
    15804,
    16744,
    17739,
    18794,
    19912,
    21096,
    22350,
    23679,
    25087]
filename = input("请输入文件名")
mid = mido.MidiFile(filename)
channel = 0

tempo = 500000
"""
music_desc = {
    'freq': 0,              #频率，0为休止符
    'time': 0,              #毫秒
}
"""
current_pressing = 0
current_second = 0
music = []
for msg in mid.tracks[1]:
    # Convert message time from absolute time
    # in ticks to relative time in seconds.
    if msg.time > 0:
        delta = mido.tick2second(msg.time, mid.ticks_per_beat, tempo)
    else:
        delta = 0
    
    current_second += delta
    if msg.type == 'set_tempo':
        tempo = msg.tempo
    elif msg.type == 'note_on' and msg.channel == channel:
        if(delta >= 0.08):
            # 上个音符已完成
            music.append({'freq': current_pressing, 'time': round(current_second * 1000)})
            current_second = 0
        current_pressing = midi_to_freq[msg.note]
    elif msg.type == 'note_off' and msg.channel == channel:
        if msg.note == current_pressing:
            current_pressing = 0
print(music)
#下面保存
import struct
"""
文件格式：
小端
2字节：频率
2字节：时间
"""
with open(filename.replace(".mid", ".buz").replace(".midi", ".buz"), 'wb') as f:
    for i in range(len(music)):
        if i == 0 and music[i]['freq'] == 0:
            continue
        f.write(struct.pack('<H', music[i]['freq']))
        f.write(struct.pack('<H', music[i]['time']))
