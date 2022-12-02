#!/usr/bin/env python3

import os

#oppening the html for reading
# file = open("/workspaces/webserv_42/database/default_index_.html", "r")
file = open("/home/kis619/webserv_42/database/default_index_.html", "r")

# #initialising an empty string, getting the intra login, prepraing the path
replaced_content = ""
intra = os.environ["QUERY_STRING"].split('=')[1]
# print(intra)
# new_path = "/workspaces/webserv_42/database/intraPictures" + intra + '.jpg'
new_path = "/home/kis619/webserv_42/database/intraPictures/" + intra + ".jpg"

#looping through the file
for line in file:

    #replacing the texts
    new_line = line.replace("/database/UFF.png", new_path)

    #concatenate the new string and add an end-line break
    replaced_content = replaced_content + new_line

    
#close the file
file.close()

# print(os.environ)
# print("Content-Type: text/html")
print(replaced_content)