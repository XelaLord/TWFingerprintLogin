Hello!
Disclaimer: If you're reading this, go do something more valuable with your
time.

SECTION 1: Overview
This system uses an arduino and a fingerprint scanner to automate the
student meeting sign in process. It has the capability to enroll (register)
students fingerprints, to identify a students fingerprint, automatically
update a google attendance spreadsheet to say that student has signed in or
out, and to clear all fingerprints off of the scanner at the end of a season.
This involves python scripts, arduino scripts, and interfacing with the
Google Drive API. 

SECTION 2: Scripts
There are 5 important scripts
Enroll.py
SignIn.py
Reset.py
ListSpreadsheets.py
Scanner.ino

Firstly, ListSpreadsheets.py does pretty much what it says on the tin, it
finds all the spreadsheets the Google Service account has access to,
displays the names and other relevant info, and saves that data to the
List_of_Spreadsheets.txt file. This is designed make it easier to access
the desired spreadsheet. (See section 4)

Now that that is out of the way, Let’s dive into the other 4 scripts.
There is one script uploaded onto the arduino, and 3 python scripts each
with their own unique functionality. 
Enroll.py is used to enroll new students fingerprints onto the scanner
SignIn.py is the script that logs students in as they scan their fingers
and updates the google spreadsheet
Reset.py is used to delete all stored fingerprints off of the scanner, to
be used at the end of the season.
Scanner.ino is the script that is uploaded onto the arduino. It serves as a
middle man between the physical scanner and any of the python scripts,
communicating with the python script to determine the required action,
interfacing and managing the physical scanner, and then passing any
relevant information from the scanner to the python script. While there
are 3 python scripts, there is only 1 arduino script. The arduino script
capable of interfacing with all 3 python scripts and performing all 3 tasks.

IMPORTANT: An important thing to realise is that while there is ONLY ONE
script that is uploaded onto the Arduino, this is not how the system was
originally designed. It was originally developed with 3 separate arduino
scripts, each corresponding to a python script, but it those 3 scripts were
combined into 1 to improve the ease of use and user experience. Now I’m
pretty sure that this redesign didn’t break anything, but the legacy folder
contains the original 3 arduino scripts and matching python scripts, just
in case.

SECTION 3: Running the system
To run this system you need Python (Version 3), and IDLE and the Arduino 
editor are HIGHLY recommended, the system is technically capable of running
without them, but only technically.

The Arduino script should already be uploaded onto the Arduino but if it’s
not, do that, I guess.
You can run the python scripts through the windows terminal, or through
IDLE, that’s up to you.
Run the script corresponding to your desired action and follow the prompts.
I’ve tried to make it as easy as possible.

SECTION 4: Google Drive API
In order for the system to update a google attendance spreadsheet a couple
things need to be set up first, but like before I’ve tried to make it as
easy as possible. 
We need 4 things
The desired spreadsheet to be shared with the google service account
The desired workbook to be referenced in the SpreadsheetData.txt file
The correct spreadsheet to referenced in the SpreadsheetData.txt file
An internet connection. (Duh)

IMPORTANT: A workbook and and a spreadsheet are 2 different things the
difference between them is crucial. If you don’t know, look it up.

Step one is as easy as sharing the spreadsheet with any other user. The
email address to share it with is
“twfingerprintlogin@twfingerprintlogin.iam.gserviceaccount.com”
which can also be found in the ServiceAccountCredential.json file, or at
the top of all of the python scripts.

Steps 2 and 3 are a bit harder, but if you use the ListSpreadsheets.py
script it should be very easy. Run the python script, copy the name and ID
of the desired workbook, paste that info, and/or the workbook’s URL into
the SpreadsheetData.txt file and format as it specifies, making sure it
looks something like this.

	title="Workbook name"
	url="https://docs.google.com/spreadsheets/d/[some code]”
	key="[some code]”

That's the workbook done. The spreadsheet is easier. If you only have one
spreadsheet in the workbook, or you just want it to use the first
spreadsheet, you can leave the second section blank. If you have multiple
spreadsheets and the one that is used matters, then simply input the name
and/or index of the desired sheet into the SpreadsheetData.txt file and
format as it specifies, making sure it looks something like this.

IMPORTANT: This is programing, we start counting at 0. The first sheet has
index 0.

	title="February"
	index="1"

SECTION 5: Wrap-up
That should be it. If any problems arise that cannot be solved by the
programming team, or if you just have questions, you can contact me at
alex.m.boyer@gmail.com.

FIN

P.S. If you're running this on linux, you're probalby having trouble with the serial connection. Linux stores Serial port data in the /dev directory, in files called tty[some code]. This files are permission limited and by defualt a normal user doesn't have access. All you have to do is add your user to the 'dialout' group, and that should fix it.
