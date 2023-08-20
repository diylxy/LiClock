import time
import gzip
import traceback
import sys

dropped_file = sys.argv[1]
print(dropped_file)

try:
    with gzip.open(dropped_file, mode='rb', compresslevel=9) as f:
        data = f.read()

    out_array = "";
    
    iter = 0
    for i in range(len(data)):
        hex_item = hex(data[i])
        if len(hex_item) < 4:
            hex_item = hex_item[0]+hex_item[1]+"0"+hex_item[2]

        out_array += hex_item
        out_array += ", "
        
        iter+=1
        if iter == 150:
            iter = 0
            out_array += "\n  "
            
    out_array = out_array[:-2]
    out_array = out_array.upper()
    out_array = out_array.replace('X','x')
    
    out_array = "uint8_t music[] = {\n  " + out_array + "\n};"
    
    print(out_array)
    
    with open("vgm_arduino.txt.","w+") as f:
        f.write(out_array)
    
    print("\n------------------------------------------------------");
    print(  "Conversion complete, output stored in vgm_arduino.txt!")
    print(  "------------------------------------------------------");
    
except:
    traceback.print_exc()
    time.sleep(10)