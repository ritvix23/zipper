# zipper
Text file compressor

Before proceeding - 
1. make sure you have the 'tkinter' module for python3 installed
2. you are using latest c++ compiler version ( should support c++11 standard) 
3. You can choose any text(.txt) file to compress.
A sample text file('random.txt') is attached.


To use GUI - 
	
1. Run the python3 script GUI.py

	In linux terminal -> python3 GUI.py

2. A file browser window will open up.

3. Navigate & open the file to be compressed( file has to be .txt format)



To execute C++ file directly (without GUI) -

1. Make the directory in which zipper.cpp is present as working directory.

2. (This step can be skipped as the compiled executable(for linux systems) is provided in the submission files) 
   Compile the file zipper.cpp and name executable file as 'zipper'

	in linux terminal -> g++ -std=c++11 zipper.cpp -o zipper
 
3. Run the compiled executable with the filename(or absolute/relative address of the file to be compressed) as argument
	
	for eg if 'random.txt' has to compressed and is present in working directory 
	In terminal -> ./zipper random.txt



NOTE - 
* The results of various coding schemes will appear at the same terminal instance(command prompt for windows)

* Compressed binary files will be saved in the working directory for various applied coding schemes
	(huff_comp.dat -> compressed file from huffman coding, 
	 shannon_comp.dat -> compressed file from shannon coding, 
	 shanfano_comp.dat -> compressed file from shannon-fano coding)

* Encoded bitstrings corresponding to various coding schemes will be saved in working directory.
	
