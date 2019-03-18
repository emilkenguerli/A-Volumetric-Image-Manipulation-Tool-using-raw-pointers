#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "tool.h"

using namespace std;

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)>\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-d,--difference \tSpecify difference map between\n"
              << "\t\t\t\timages i and j, and write out to file.\n"
              << "\t\t\t\tEnter numbers i and j followed by file name.\n"
              << "\t-x,--extract \t\tExtract and write the slice with\n"
              << "\t\t\t\tnumber i and write out to file.\n"
              << "\t\t\t\tEnter number i and file name.\n"
              << std::endl;
}

static int parse_args(int argc, char* argv[], int& option_chosen, 
    std::string& image_base, int& number1, int& number2, std::string& output_file)
{
    std::string difference_args;
    std::string extract_args;
    std::string dif_output_file;
    std::string slice;
    std::string ext_output_file;
    int first_image_number;
    int second_image_number;
    int slice_number;
    option_chosen = 0;


    if (argc < 2 || argc > 6 || argc == 3 || argc == 4)
    {
        show_usage(argv[0]);
        return 1;
    }

    if (argc == 2) 
    {
        option_chosen = 1; // do your default things here
        image_base = argv[1]; // Get imageBase - the prefix for the file sequence
        if (image_base[0] == '-') {
            cout << "Invalid image_base file name \n" << image_base << endl; 
            show_usage(argv[0]);        
        }
    }
    int i;
    for (option_chosen == 0, i = 2; i < argc; ++i) 
    {
        if (argc == 2) option_chosen = 1; // do your default things here
        image_base = argv[1]; // Get imageBase - the prefix for the file sequence
        if (image_base[0] == '-') {
            cout << "Invalid image_base file name \n" << image_base << endl; 
            show_usage(argv[0]);        
        }

        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help"))  
        {
            show_usage(argv[0]);
            return 0;
        } 
        else if ((arg == "-d") || (arg == "--difference")) 
        {
            if (i + 3 < argc && argc < 7) 
            { // Make sure we have enough difference argiments
                difference_args = argv[i++]; 
                std::istringstream convert1(argv[i++]); 
                if ( !(convert1 >> number1) )  
                {
                    number1 = 0; 
                    std::cerr << "non-numeric first image number\n" << std::endl;
                    show_usage(argv[0]);
                    return 1;
                } 
 
                std::istringstream convert2(argv[i++]); 
                if ( !(convert2 >> number2) ) 
                {
                    number2 = 0; 
                    std::cerr << "non-numeric second image number\n" << std::endl;
                    show_usage(argv[0]);
                    return 1;
                } 

                output_file = argv[i++];
                option_chosen = 2;
            } 
            else 
            { // Not enough arguments to the difference option.
                std::cerr << "-d option requires 3 arguments." << std::endl;
                show_usage(argv[0]);
                return 1;
            }  
        } 
        else if ((arg == "-x") || (arg == "--extract")) 
        {
            if (i + 2 < argc && argc < 6) 
            { // Make sure we have enough extract argiments
                extract_args = argv[i++];
                slice = argv[i++];
                std::istringstream convert(slice); 
                if ( !(convert >> number1) ) 
                {
                    number1 = 0; 
                    std::cerr << "non-numeric slice number\n" << std::endl;
                    show_usage(argv[0]);
                    return 1;
                } 

                output_file = argv[i++];
                option_chosen = 3;
            } 
            else 
            { // Not enough arguments to the extract option.
                std::cerr << "-x option requires 2 arguments." << std::endl;
                show_usage(argv[0]);
                return 1;
            }  
        } 
        else 
        {
             std::cerr << "No Option chosen" << std::endl;
             show_usage(argv[0]);
             return 1;
        }
    }
}

using namespace KNGEMI002;
int main(int argc, char* argv[])
{
    int option = 0;
    std::string image = "";
    std::string output_file = "";
    int number1 = 0;
    int number2 = 0;
 
    std::string difference_output_file;
    std::string exttract_output_file;
    int first_image_number;
    int second_image_number;
    int slice_number;

	KNGEMI002::VolImage vi;

    parse_args(argc, argv, option, image, number1, number2, output_file);
    cout << "option = " << option << endl;
	cout << "Image_base = " << image << endl;
	
    if (option == 1) // no arguments passed
    {   
		 cout << "Number of images: " << KNGEMI002::getNumberofImages(image) << std::endl;
		 cout << "Number of bytes required: " << vi.volImageSize() << std::endl;
		 if(vi.readImages(image)){
			cout << "Images read: True\n";
		 }
		 else{
			cout << "Images read: False\n";
		 }
		 
    }
    else if (option == 2) // -d chosen
    {
        first_image_number = number1;
        second_image_number = number2;
        difference_output_file = output_file;
        cout << "Image_base = " << image << endl;
        cout << "First image number: " << first_image_number << endl;
        cout << "Second image number: " << second_image_number << endl;
        cout << "Difference output filename: " << output_file << endl;

		KNGEMI002::getNumberofImages(image);
		vi.diffmap(first_image_number, second_image_number, output_file);

    } 
    else if (option == 3) // -x chosen
    {
        slice_number = number1;
        cout << "Image_base = " << image << endl;
        cout << "Slice number: " << slice_number << endl;
        cout << "Extract output filename: " << output_file << endl;

		KNGEMI002::getNumberofImages(image);
		vi.extract(slice_number, output_file);
    } 
    else
    {
        cout << "No option chosen" << endl;
    }

    return 0;
}
