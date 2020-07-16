Volumetric Image Tool

Usage
volimage <imageBase> [-d i j output_file_name] [-x i output_file_name]

where 
'volimage' is the name of the executable 
<imageBase> is the prefix for the file sequence (eg "MRI") 
Two optional command line arguments (indicated by the [] brackets):
-d i j output file name: compute a dierence map between images i and j, and write
this out to file
-x i output file name: extract and write the slice with number i and write this out
to file

Description of files

tool.h:
This is the header file that declares the VolImage class and getNumberofImages method

tool.cpp:
This file defines the VolImage and getNumberofImages method

OWN METHODS

getNumberofImages takes in the name of the file of the base image and returns the number of images (int) in the header file "xxx.data" or "xxx.dat"

Driver.cpp:

Contains the main and is where the operations are called. It displays all arguments inserted in the command line and if there are missing arguments it invokes a detailed explaination of the problem and possible solutions such as use -h to show that explaination.

