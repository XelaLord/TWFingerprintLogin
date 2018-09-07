import serial, gspread, time
from oauth2client.service_account import ServiceAccountCredentials
from Common import *
"twfingerprintlogin@twfingerprintlogin.iam.gserviceaccount.com"
        
def InitScanner():
    stream = ''
    while (stream[-1:] != ';'):
        stream += str(ser.read())[2:-1]
    init = stream[:-1]

    if init[0] == 'R':
        if init[1] == '1':
            ser.write(bytes('1;','utf-8'))
            print("[NOTICE] Sensor configured successfully")
        else:
            ser.write(bytes('0;','utf-8'))
            print("[ERROR] Sensor configure failed")
            input("[ERROR] Press enter to close")
            exit()
    else:
        ser.write(bytes('0;','utf-8'))
        print("[ERROR] Wrong script loaded onto Arduino, please upload \"Reset.ino\" to the Arduino")
        input('[INPUT] Press enter to close script')
        quit()
#----------------------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------------------
ser = Connect()
InitScanner()

print("\n")
time.sleep(1)
print("[NOTICE] This program will erase all fingerprints off of the scanner")
time.sleep(3)
print("[NOTICE] THERE IS NO COMING BACK FROM THIS")
time.sleep(4)
if (input("[INPUT] Are you sure? (y/n) ").lower() == 'y'):
    print("[INPUT] Please press the button on the scanner to confirm")
    ser.read()
    print("[NOTICE] Emptying database")
    ser.write(bytes(str(1),'utf-8'))
    if (int(str(ser.read())[2:-1]) == 1):
        print("[NOTICE] Database emptied")
        input("[INPUT] Press enter to exit script")
        exit()
else:
    exit()
