'''
This script does nothing but house any functions that are common between scripts
'''
import serial, gspread, time, sys, glob
from oauth2client.service_account import ServiceAccountCredentials

def Connect(): #Receive, parse, and display startup data
    global ser
    
    ports = serial_ports() #["COM%s" % str(i+1) for i in range(256)]
    for port in ports:
        try:
            ser = serial.Serial(port,9600)
            print("[NOTICE] Connected through %s" % port)
            break
        except Exception as e:
            print(e)
    try:
        return ser
    except:
        print('[ERROR] Could not connect to Arduino')
        print('[ERROR] Check the USB connection and close any other programs accessing the Arduino')
        input('[INPUT] Press enter to close script')
        quit()

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')
    
    return ports

def InitScanner(iden):
    stream = ''
    while (stream[-1:] != ';'):
        stream += str(ser.read())[2:-1]
    init = stream[:-1]

    if init[0] == '1':
        ser.write(bytes(iden + ';','utf-8'))
        print("[NOTICE] Sensor configured successfully")
    else:
        print("[ERROR] Sensor configure failed")
        input("[ERROR] Press enter to close")
        exit()

def InitGoogleAPI(): #Sets up Google Sheets' API and finds the spreadsheet
    global sheet

    #Verifing credentials from ServiceAccountCredentials.json file
    try:
        creds = ServiceAccountCredentials.from_json_keyfile_name('ServiceAccountCredentials.json', ['https://spreadsheets.google.com/feeds'])
        client = gspread.authorize(creds)
        print('[NOTICE] Credentials Accepted')
    except Exception as e:
        print(e)
        print('[ERROR] Credentials Denied, couldn\'t access Service Account')
        input('[INPUT] Press enter to close script')
        quit()

    #Accessing the indicated spreadsheet
    f = open("SpreadsheetData.txt",'r')
    data = [i.split('=')[1][1:-1] for i in f.read().split('\n') if ('=' in i)]
    f.close()

    if data[:3] == ['','','']:
        print("[ERROR] No information about spreadsheet provided")
        print("[ERROR] Plese see \"SpreadsheetData.txt\" and enter relevant data")
        input('[INPUT] Press enter to close script')
        quit()

    sheet = 0
    flag = False
    if (data[0] != ''):
        try:
            sheet = client.open(data[0])
        except:
            flag = True
            
    if (data[1] != '' and sheet == 0):
        try:
            sheet = client.open_by_url(data[1])
        except:
            flag = True
            
    if (data[2] != '' and sheet == 0):
        try:
            sheet = client.open_by_key(data[2])
        except:
            flag = True

    if sheet != 0:
        flag = False
        if len(sheet.worksheets()) == 1:
            sheet = sheet.sheet1
        else:
            if (data[3] != ''):
                try:
                    sheet = sheet.worksheet(data[3])
                except:
                    flag = True
                    
            if (data[4] != '' and type(sheet) != "<class 'gspread.models.Worksheet'>"):
                try:
                    sheet = sheet.get_worksheet(int(data[4]))
                except:
                    flag = True
            else:
                try:
                    sheet = sheet.sheet1
                except:
                    flag = True

    if not flag:
        print('[NOTICE] Spreadsheet found')
    else:
        print('[ERROR] Couldn\'t access designated spreadsheet')
        input('[INPUT] Press enter to close script')
        quit()

    return sheet
