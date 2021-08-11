Author - Ritwik Sahani 
- EE18BTECH11038



Before proceeding, a few notes - 
1. Preferrably use linux OS(with gcc) for compilation because I have written the code in linux and the code might spawn errors in other OS
 as compilers change from OS to OS.
2. make sure you have the 'tkinter' module for python3 installed
3. you are using latest c++ compiler version ( should support c++11 standard) 
4. The code is not limited to compress a particular file, you can choose file of your wish to compress(it has to .txt file though). 
A sample text file('random.txt') has been attached with the submitted files. The problems files are also attached.


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
	
	for ex if 'random.txt' has to compressed and is present in working directory 
	In terminal -> ./zipper random.txt




* The results of various coding schemes will appear at the same terminal instance(command prompt for windows)

* Compressed binary files will be saved in the working directory for various applied coding schemes
	(huff_comp.dat -> compressed file from huffman coding, 
	 shannon_comp.dat -> compressed file from shannon coding, 
	 shanfano_comp.dat -> compressed file from shannon-fano coding)

* Encoded bitstrings corresponding to various coding schemes will be saved in working directory.
	
