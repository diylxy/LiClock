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
filename = input("请输入文件名：")
mode=input("请输入模式（1:最高音优先/2:最接近上个音符优先）：")
mid = mido.MidiFile(filename)
max_len = 0
def process(track, id):
    global max_len
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
    last_pressing = -1
    music = []
    for msg in track:
        # Convert message time from absolute time
        # in ticks to relative time in seconds.
        if msg.time > 0:
            delta = mido.tick2second(msg.time, mid.ticks_per_beat, tempo)
        else:
            delta = 0
        
        current_second += delta
        if msg.type == 'set_tempo':
            tempo = msg.tempo
        elif msg.type == 'note_on':
            if(current_second == 0):
                if mode == "2":
                    if(last_pressing != -1):
                        if abs(current_pressing - last_pressing) < abs(midi_to_freq[msg.note] - last_pressing):
                            continue
                else:
                    if current_pressing > midi_to_freq[msg.note]:
                        continue
            if(current_second >= 0.08):
                # 上个音符已完成
                music.append({'freq': current_pressing, 'time': round(current_second * 1000)})
                current_second = 0
                last_pressing = current_pressing
            current_pressing = midi_to_freq[msg.note]
        elif msg.type == 'note_off':
            if msg.note == current_pressing:
                current_pressing = 0
    print(len(music))
    #print(music)
    if(len(music) == 0):
        return
    if(len(music) < max_len and len(music) < 100):
        return
    if len(music) > max_len:
        max_len = len(music)
    #下面保存
    import struct
    """
    文件格式：
    小端
    2字节：频率
    2字节：时间
    """
    with open(filename.replace(".mid", str(id) + ".buz").replace(".midi",  str(id) + ".buz"), 'wb') as f:
        for i in range(len(music)):
            if i == 0 and music[i]['freq'] == 0:
                continue
            f.write(struct.pack('<H', music[i]['freq']))
            f.write(struct.pack('<H', min(music[i]['time'], 30000)))
idx = 0
for t in mid.tracks:
    process(t, idx)
    idx += 1
