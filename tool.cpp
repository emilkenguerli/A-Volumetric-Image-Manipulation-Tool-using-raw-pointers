#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "tool.h"

using namespace std;

int imageWidth;
int imageHeight;
std::string filename;
int numOfImages;

KNGEMI002::VolImage::VolImage(){
	width = 0;
	height = 0;
}
	
KNGEMI002::VolImage::~VolImage(){
            vector<unsigned char**>::iterator it;
            for(it=slices.begin();it!=slices.end();it++)
            {
                delete **it;            
            }
            slices.clear();
}

bool KNGEMI002::VolImage::readImages(std::string baseName){
        for (int j = 0; j < numOfImages; j++)
        {
            ifstream image_base_file;
            unsigned char ** bytes = new unsigned char*[imageWidth]; //raw file goes here
            // 429 col, 303 row
            for  (int l = 0; l < imageWidth; l++)
            {
                bytes[l] = new unsigned char[imageHeight];
            }
            string s = to_string(j);
            string ext = ".raw";
            string file_name = baseName+s+ext;
            image_base_file.open(file_name,std::ios_base::binary);
            if (!image_base_file) 
            {
                std::cerr << "Could not open the file!" << std::endl;
            }
            else
            {
                int byte_count = 0;
                for (int k = 0; k < imageWidth; k++)
                {
                    for  (int m = 0; m < imageHeight; m++)
                    {
                        unsigned char byte;

                        image_base_file >> byte; //byte by byte

                        if (image_base_file.fail())
                        {
                            // end of file
                            break;
                        }

                        bytes[k][m] = byte;
                        byte_count++;
                     }
                 }
            }
            slices.push_back(bytes); //pushing into the vector of slices
        }
		return true;
}

void KNGEMI002::VolImage::diffmap(int sliceI, int sliceJ, std::string output_prefix){
	    int j = 0;
        for (int i = 0; i < 2; i++)
        {
            if (i == 0) j = sliceI;
            if (i == 1) j = sliceJ;
            ifstream image_base_file;
            // allocate memory
		cout << "imageWidth = " << imageWidth << endl; 
		cout << "imageHeight = " << imageHeight << endl; 
            unsigned char ** bytes = new unsigned char*[imageWidth]; //raw file goes here
            for  (int l = 0; l < imageWidth; l++)
            {
                bytes[l] = new unsigned char[imageHeight];
            }
            string s = to_string(j);
            string ext = ".raw";
            string file_name = "MRI"+s+ext;
            cout << file_name << endl;
            image_base_file.open(file_name,std::ios_base::binary);
            if (!image_base_file) 
            {
                std::cerr << "Could not open the file!" << std::endl;
            }
            else
            {
                int byte_count = 0;
                for (int k = 0; k < imageWidth; k++)
                {
                    for  (int m = 0; m < imageHeight; m++)
                    {
                        unsigned char byte;

                        image_base_file >> byte; //byte by byte

                        if (image_base_file.fail())
                        {
                            // end of file
                            break;
                        }

                        bytes[k][m] = byte;
                        byte_count++;
                     }
                 }
                 cout << "Number of bytes = " << byte_count << endl;
            }
            slices.push_back(bytes); //pushing into the vector of slices
        }
        std::fstream output_file_stream;
		output_prefix = output_prefix + ".raw";
        output_file_stream.open(output_prefix, std::ios::app | std::ios::binary);
  
        unsigned char output_pixel;
        int number_of_pixels = 0;
		for (int p = 0; p < imageWidth; p++)
        {
            for  (int m = 0; m < imageHeight; m++)
            {
                output_pixel = (unsigned char)(abs((float)slices.at(0)[p][m] - (float)slices.at(1)[p][m])/2);
                output_file_stream.write(reinterpret_cast<char*>(&output_pixel), sizeof(output_pixel));
                number_of_pixels++;
          }
        }
        output_file_stream.close();
        cout << "Number of pixels = " << number_of_pixels << endl;    
    

}

void KNGEMI002::VolImage::extract(int sliceId, std::string output_prefix){
			ifstream image_base_file;
            // allocate memory
            unsigned char ** bytes = new unsigned char*[imageWidth]; //raw file goes here
            for  (int l = 0; l < imageWidth; l++)
            {
                bytes[l] = new unsigned char[imageHeight];
            }
            string s = to_string(sliceId);
            string ext = ".raw";
            string file_name = "MRI"+s+ext;
            image_base_file.open(file_name,std::ios_base::binary);
			cout << "imageWidth = " << imageWidth << endl;
			cout << "imageHeight = " << imageHeight << endl;

            if (!image_base_file) 
            {
                std::cerr << "Could not open the file!" << std::endl;
            }
            else
            {
                int byte_count = 0;
                for (int k = 0; k < imageWidth; k++)
                {
                    for  (int m = 0; m < imageHeight; m++)
                    {
                        unsigned char byte;

                        image_base_file >> byte; //byte by byte

                        if (image_base_file.fail())
                        {
                            // end of file
                            break;
                        }

                        bytes[k][m] = byte;
                        byte_count++;
                     }
                 }
            }
        slices.push_back(bytes); //pushing into the vector of slices

        file_name = output_prefix+".dat";
        ofstream header_file_stream (file_name);
        if (header_file_stream.is_open())
        {
            header_file_stream << imageWidth << ", " << imageHeight << ", " << numOfImages << std::endl;
            header_file_stream.close();
        }
        else cout << "Unable to open file";

        std::fstream output_file_stream;
		output_prefix = output_prefix +".raw";
        output_file_stream.open(output_prefix, std::ios::app | std::ios::binary);
  
        unsigned char output_byte;
        int number_of_bytes = 0;

        for (int k = 0; k < imageWidth; k++)
        {
            for  (int m = 0; m < imageHeight; m++)
            {
                output_byte = (unsigned char)slices.at(0)[k][m];
                output_file_stream.write(reinterpret_cast<char*>(&output_byte), sizeof(output_byte));
                number_of_bytes++;
            }
        }
        output_file_stream.close();
        cout << "Number of bytes written= " << number_of_bytes << endl;  

}

int KNGEMI002::VolImage::volImageSize(void){
    int size;
	int byte_count = 0;
	for (int j = 0; j < numOfImages; j++)
        {
            ifstream image_base_file;
            string s = to_string(j);
            string ext = ".raw";
            string file_name = filename+s+ext;
            image_base_file.open(file_name,std::ios_base::binary);
            if (!image_base_file) 
            {
                std::cerr << "Could not open the file!" << std::endl;
            }
            else
            {               
                for (int k = 0; k < imageWidth; k++)
                {
                    for  (int m = 0; m < imageHeight; m++)
                    {
                        unsigned char byte;

                        image_base_file >> byte; //byte by byte

                        if (image_base_file.fail())
                        {
                            // end of file
                            break;
                        }

                        byte_count++;
                     }
                 }
            }
        }

	int bytes = sizeof(int);
	size = byte_count + (bytes*numOfImages);

	return size;
}

int KNGEMI002::getNumberofImages(std::string s){
	std::ifstream file;
	std::string fileName = s + ".data";
	filename = s;
    file.open(fileName); 

	if(!file){ 
		cout << "File is not a .data file but a .dat" << endl;
		fileName = s + ".dat";
		file.open(fileName);
	}

    int data=0;
	std::vector<std::string> v;
	std::vector<std::string> v2;
	std::string line;

	if(!file){ 
		cout << "Couldn't open file" << endl; 
	}
	else{		
		while(std::getline(file, line)){
			v.push_back(line);			
		}
		std::string word;
		std::istringstream iss(v[0]);

		while(std::getline(iss, word, ' ')){
			v2.push_back(word);				
		}
	}

	imageWidth = stoi(v2[0]);
	imageHeight = stoi(v2[1]);
	int size = stoi(v2[2]);
    file.close();
	numOfImages = size;

	return size;
}