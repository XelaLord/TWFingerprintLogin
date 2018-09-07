import serial, gspread, time
from oauth2client.service_account import ServiceAccountCredentials
from Common import *
"twfingerprintlogin@twfingerprintlogin.iam.gserviceaccount.com"
    
ser = Connect()
InitScanner('R')

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
