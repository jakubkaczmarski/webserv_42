#!/usr/bin/env python3

import os


file = open("/workspaces/webserv_42/database/default_index_.html", "r")			#oppening the html for reading


replaced_content = ""															#initialising an empty string 
intra = os.environ["QUERY_STRING"].split('=')[1]								#getting the intra login
new_path = "/workspaces/webserv_42/database/intraPictures/" + intra + '.jpg'	#prepraing the path


for line in file:																#line by line reading
    new_line = line.replace("/database/UFF.png", new_path)						#replacing the text
    replaced_content = replaced_content	+ new_line								#adding the new line

file.close()																	#close the file

# print("Content-Type: text/html")
print(replaced_content)															#print result