import time, gspread
import serial
from oauth2client.service_account import ServiceAccountCredentials
from Common import *
"twfingerprintlogin@twfingerprintlogin.iam.gserviceaccount.com"
#----------------------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------------------
def InitScanner():
    stream = ''
    while (stream[-1:] != ';'):
        stream += str(ser.read())[2:-1]
    init = stream[:-1]

    if init[0] == 'L':
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
        print("[ERROR] Wrong script loaded onto Arduino, please upload \"SignIn.ino\" to the Arduino")
        input('[INPUT] Press enter to close script')
        quit()

def InitSpreadsheet():
    global date, IDs, column, row

    print("[STATUS] Calibrating to spreadsheet")
    date = '%s/%s' % (time.localtime().tm_mon,time.localtime().tm_mday)
    row = sheet.col_values(1).index("ID")
    try:
        a = sheet.col_values(1)[row:].index('')+row
    except:
        a = sheet.row_count
    try:
        b = sheet.row_values(row+1).index('')
    except:
        b = sheet.col_count
    sheet.resize(a,b)
    IDs = {int(sheet.cell(i,1).value):i for i in range(row+2,sheet.row_count+1)}
    
    try:
        column = sheet.row_values(row).index(date) + 1
    except:
        print("[ERROR] Today's date not found in spreadsheet")
        if (input("[INPUT] Insert columns for today? (y/n): ").lower() == 'y'):
            column = sheet.col_count + 1
            sheet.add_cols(2)
            sheet.update_cell(row,column,date)
            sheet.update_cell(row+1,column,"Time-in")
            sheet.update_cell(row+1,column+1, "Time-out")
#---------------------------------------------------------------------------------------------------- 


#----------------------------------------------------------------------------------------------------
ser = Connect()
InitScanner()
sheet = InitGoogleAPI()
InitSpreadsheet()
print()

#Main loop
while True:
    stream = ' '
    while (stream[-1] != ';'):
        stream += str(ser.read())[2:-1]
    i = int(stream[1:-1])

    if i in IDs.keys():
        if (sheet.cell(IDs[i], column).value == ''):
            sheet.update_cell(IDs[i], column, time.strftime("%H:%M"))
            print("%s signed in" % sheet.cell(IDs[i],2).value)
        else:
            sheet.update_cell(IDs[i], column+1, time.strftime("%H:%M"))
            print("%s signed out" % sheet.cell(IDs[i],2).value)
        ser.write(bytes("1;","utf-8"))
        
    else:
        print("[ERROR] Fingerprint ID not in attendance spreadsheet")
        ser.write(bytes("0;","utf-8"))
    time.sleep(2)
