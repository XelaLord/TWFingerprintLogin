import gspread, time
from oauth2client.service_account import ServiceAccountCredentials
"twfingerprintlogin@twfingerprintlogin.iam.gserviceaccount.com"

#Verifing credentials from ServiceAccountCredentials.json file
try:
    creds = ServiceAccountCredentials.from_json_keyfile_name('ServiceAccountCredentials.json', ['https://spreadsheets.google.com/feeds'])
    client = gspread.authorize(creds)
    print('[NOTICE] Credentials Accepted')
except Exception as e:
    print(e)
    print('[ERROR] Credentials Denied')
    print('[ERROR] Couldn\'t access Service Account')
    print('[ERROR] Ending script early')
    quit()

sheets = client.openall()
print()
[print("Name: %s\tID: %s" % (i.title,i.id)) for i in sheets]
print()
f = open("List_of_Spreadsheets.txt",'w')
[f.write("Name: %s\tID: %s\n" % (i.title,i.id)) for i in sheets]
f.close()

print("[NOTICE] List outputed to \"List_of_Spreadsheets.txt\"")
input('[INPUT] Press enter to close script')
quit()
