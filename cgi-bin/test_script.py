#!/usr/bin/python3
# The line "Content-type: text/html\n\n" is special piece of text that must be the first thing sent to the browser by any CGI script.
import os
import socket
#create an env variable before entering the script
os.environ['INTRA_NAME'] = "kmilchev"
print(os.environ['GIT_EDITOR'])

# open file in read mode
file = open("default_index_.html", "r")
replaced_content = ""
intra = os.environ['INTRA_NAME']

#looping through the file
for line in file:
    #replacing the texts
    new_line = line

    if "database" in line:
        print("UES")
    new_line = line.replace('<img src= "/database/UFF.png"></img>', '<img src= "/database/' + intra + '.png"></img>')

    #concatenate the new string and add an end-line break
    replaced_content = replaced_content + new_line + "\n"

    
#close the file
file.close()

#Open file in write mode
write_file = open("responsehtml.html", "w")

#overwriting the old file contents with the new/replaced content
write_file.write(replaced_content)

#close the file
write_file.close()

