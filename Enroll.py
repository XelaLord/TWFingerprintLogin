import serial, gspread, time, random
from oauth2client.service_account import ServiceAccountCredentials
from Common import *
"twfingerprintlogin@twfingerprintlogin.iam.gserviceaccount.com"

def InitSpreadsheet():
    global IDs, available, row
    print("[STATUS] Importing data from spreadsheet")
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
    
    IDs = {int(sheet.cell(i,1).value):sheet.cell(i,2).value for i in range(row+2,sheet.row_count+1)}
    available = [i for i in range(162) if i not in IDs.keys()]
#----------------------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------------------
def PrintStatusMessage(string):
    global flag

    if not flag:
        p = ' '
        while (p[-1] != ';'):
            p += str(ser.read())[2:-1]
        p = int(p[:-1])
        print(string + ": " + StatusMessage(p))
        if p != 0:
            flag = True

def ReplaceFinger():
    if not flag:
        print("Remove finger")
        while (str(ser.read())[-2] != ';'):
            pass
        print()
        print("Place same finger again")
                
def StatusMessage(p):
    if p == 0:
        return "OK"
    elif p == 1:
        return "Communication Error"
    elif p == 3:
        return "Imaging error"
    elif p == 6:
        return "Image too messy"
    elif p == 7 or p == 21:
        return "Failed to find features"
    elif p == 8:
        return "Fingerprint already enrolled somewhere else"
    elif p == 9:
        return "Fingerprint not found"
    elif p == 10:
        return "Fingerprints don't match"
    elif p == 11:
        return "Can't store fingerprint in that location"
    elif p == 24:
        return "Error with flash storage"
    else:
        return "Unknown error"

ser = Connect()
InitScanner('E')
sheet = InitGoogleAPI()
InitSpreadsheet()
print("[NOTICE] Program started!")

while True:
    print('\n' * 100)
    while True:
        name = input("[INPUT] Enter your name: ")
        if input("You've entered \"%s\". This okay? (y/n) " % name).lower() == 'y':
            break
    while ser.in_waiting > 0:
        ser.read()
    fingerid = random.choice(available)
    ser.write(bytes(str(fingerid),'utf-8'))

    print()
    print("Please place your finger on the scanner, do your best to make sure the scanner and your finger are clean")
    flag = False
    
    PrintStatusMessage("Image")
    PrintStatusMessage("Conversion")
    PrintStatusMessage("Unique")

    ReplaceFinger()

    PrintStatusMessage("Image")
    PrintStatusMessage("Conversion")
    PrintStatusMessage("Unique")
    PrintStatusMessage("Model")
    PrintStatusMessage("Storage")

    ReplaceFinger()
    
    PrintStatusMessage("Image")
    PrintStatusMessage("Conversion")
    PrintStatusMessage("Confirmation")

    #Send confirmation of success (or failure)
    if flag:
        print("[ERROR] Something has gone wrong, fingerprint not enrolled. Please try again.")
    else:
        print("[NOTICE] Fingerprints all match")
        print("[STATUS] Uploading fingerprint to spreadsheet")
        try:
            sheet.append_row([fingerid,name] + ['' for i in range(sheet.col_count-2)])
            available.pop(0)
            ser.write(bytes("1;",'utf-8'))
            print("[NOTICE] Fingerprint succesfully enrolled. You're done! :)")
        except:
            ser.write(bytes("0;",'utf-8'))
            print("[ERROR] There was an error while updating the spreadsheet. Please try again")
    time.sleep(3)
