///////////////////////////////////////////////////////////
//  RACKMAKER, C++ SOURCE.
//  THIS PROGRAM WAS CREATED BY: AARON J. SKARLUPKA
//  July 2015, This version: AUGUST 23th 2016
//
//  RACKMAKER is a program designed to create
//  rackdrawings as .PDF through simple user input.
//
//  RACKMAKER uses .php for a web interface, LaTeX (.TEX)
//  for drawing generation, C++ for .TEX generation and
//  the linux command line.
//
//  Pseudo Code:
//  1) User creates an input.txt file by hand or website.
//  2) This file is placed in the same directory as RackMaker.
//  3) RackMaker is run using ./RackMaker (Linux).
//  4) RackMaker Generates a .TEX file and support files.
//  5) RackMaker compiles the .TEX file into a .PDF file.
//  6) RackMaker cleans up the extra files. (There are many!)
//  7) RackMaker moves/renames the .PDF to its appropriate
//     location. (optional).
//
//  It is recommended that RackMaker not be allowed to run
//  wild.  A timeout command should ALWAYS BE APPLIED!
//  An appropriate example of this would be:
//  user:~$ timeout 2.5 ./RackMaker
///////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <boost/algorithm/string/replace.hpp>
using namespace std;

///////////////////////////////////////////////////
//
// Get data from file or user
//
///////////////////////////////////////////////////
string get_data(ifstream& fin, string& height)
{
    string in;
    string junk;
    getline(fin, junk, ':');
    getline(fin, junk, ' ');
    getline(fin, height, ' ');
    getline(fin, in);
    return in;
}

///////////////////////////////////////////////////
//
// Get data from file or user
//
///////////////////////////////////////////////////
string get_type_number (ifstream& fin)
{
    string junk;
    string in;
    getline(fin, junk, ':');
    getline(fin, junk, ' ');
    getline(fin, in);
    return in;
}

///////////////////////////////////////////////////
//
// Get data from file or user
//
///////////////////////////////////////////////////
string get_units(ifstream& fin, string& height)
{
    string in;
    getline(fin, height, ' ');
    getline(fin, in);
    return in;
}
///////////////////////////////////////////////////
//
// Get details (author and date) from file or user
//
///////////////////////////////////////////////////
string get_details(ifstream& fin)
{
    string in;
    string junk;
    getline(fin, junk, ':');
    getline(fin, junk, ' ');
    getline(fin, in);
    return in;
}

///////////////////////////////////////////////////
//
// Get details (author and date) from file or user
//
///////////////////////////////////////////////////
void get_revisions(ifstream& fin, string& date, string& by, string& desc)
{
    getline (fin, date, ' ');
    getline (fin, by, ' ');
    getline (fin, desc);
}

///////////////////////////////////////////////////
//
// Get details (author and date) from file or user
//
///////////////////////////////////////////////////
string replace_special_characters(string& test_string)
{
  boost::replace_all(test_string, "&","\\&");
  return test_string;
}

///////////////////////////////////////////////////
//
// Print the header for the .TEX file
//
///////////////////////////////////////////////////
void print_header(int height, string number)
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex");
    myfile << "\\documentclass{minimal}\n";
    myfile << "\\usepackage[paperwidth=8.5in, paperheight=11in, textwidth=7.5in, textheight=10in]{geometry}\n";
    myfile << "\\usepackage{tikz}\n";
    myfile << "\\usetikzlibrary{trees}\n";
    myfile << "\\begin{document}\n";
    myfile << "\\begin{center}\n";
    myfile << "\\begin{tikzpicture}[scale=0.4]\n";
    myfile << "\\draw (0,0) rectangle (10,-1); % Print Top\n";
    myfile << "\\node at (5,-0.5) {";
    myfile << "RR";
    myfile << number;
    myfile << "};\n";
    myfile << "\\draw (0,-1) rectangle (1,-";
    myfile << height + 1;
    myfile << "); % Print Left Side\n";
    myfile << "\\draw (9,-1) rectangle (10,-";
    myfile << height + 1;
    myfile << "); % Right Side\n";
    myfile.close();
}

///////////////////////////////////////////////////
//
// Print the ru count for the .TEX file
//
///////////////////////////////////////////////////
void print_ru_count(int count)
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex", ios::app);
    myfile << "\\draw (-1,-";
    myfile << count + 2 << ")--(0,-";
    myfile << count + 2 << ");\n";
    myfile << "\\node at (-0.5,-";
    myfile << count + 2 << ") [above] {";
    myfile << count + 1;
    myfile << "};\n";
    myfile.close();

}

///////////////////////////////////////////////////
//
// Print the notes for the .TEX file
//
///////////////////////////////////////////////////
int print_notes(int count, string name, int notes_LeftRight, int heightKeeper)
{
    float temp;
    temp = count - heightKeeper/2.0 - 0.125;
    if ( notes_LeftRight%2 == 1)
        {
            ofstream myfile;
            myfile.open ("rackdrawing.tex", ios::app);
            myfile << "\\node at (15, -";
            myfile << temp;
            myfile << ") [text width=5cm, align=left, right] {";
            myfile << name;
            myfile << "};\n";
            myfile << "\\path[->] (10.25,-";
            myfile << temp;
            myfile << ") edge (15,-";
            myfile << temp;
            myfile << ");\n";
            myfile.close();
        }
    else
        {
            ofstream myfile;
            myfile.open ("rackdrawing.tex", ios::app);
            myfile << "\\node at (-5, -";
            myfile << temp;
            myfile << ")[text width=5cm, align=left, left]{";
            myfile << name;
            myfile << "};\n";
            myfile << "\\path[->] (-1.25,-";
            myfile << temp;
            myfile << ") edge (-5,-";
            myfile << temp;
            myfile << ");\n";
            myfile.close();
        }
  notes_LeftRight++;
  return notes_LeftRight;
}

///////////////////////////////////////////////////
//
// Print the units for the .TEX file
//
///////////////////////////////////////////////////
int print_units(string name, int height, int count, int& notes_LeftRight, int& heightKeeper, string temp_unit_height)
{
  if (temp_unit_height != "Notes" && temp_unit_height != "notes" && temp_unit_height != "NOTES" && temp_unit_height != "note" && temp_unit_height != "Note" && temp_unit_height != "NOTE" && temp_unit_height != "nt" && temp_unit_height != "NT" && temp_unit_height != "Nt" && temp_unit_height != "nT")
    {
    if (name != "space" && name != "sp" && name != "Space" && name != "SPACE")
    {
        ofstream myfile;
        myfile.open ("rackdrawing.tex", ios::app);
        myfile << "\\filldraw[fill=black!10!white, draw=black] (0.75,-";
        myfile << count;
        myfile << ".125) rectangle (9.25,-";
        myfile << count + height;
        myfile << ");\n";
        myfile << "\\node at (5,-";
        myfile << count + height/2.0 + 0.125;
        myfile << ") {";
        myfile << name;
        myfile << "};\n";
        myfile.close();
    }
    heightKeeper = height;
    count += height;
    }
    else
        {
            notes_LeftRight = print_notes(count, name, notes_LeftRight,heightKeeper);
        }
    return count;
}

///////////////////////////////////////////////////
//
// Print the footer for the .TEX file
//
///////////////////////////////////////////////////
void print_footer(int height)
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex", ios::app);
    myfile << "\\draw (0,-";
    myfile << height +1;
    myfile << ") rectangle (10,-";
    myfile << height + 1.5;
    myfile << "); % Bottom\n";
    myfile << "\\draw (-1,-";
    myfile << height + 1.5;
    myfile << ") rectangle (11,-";
    myfile << height + 2;
    myfile << "); % Bottom Edge\n";
    myfile.close();
}

///////////////////////////////////////////////////
//
// Print the Docuemnt info section for the .TEX file
//
///////////////////////////////////////////////////
void print_box(string name, string type, string number)
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex", ios::app);
    myfile << "\\draw (-15,-53) -- (25,-53) -- (25,-63) -- (-15,-63) -- cycle;\n";
    myfile << "\\filldraw[fill=black!10!white, draw=black] (-15,-53) rectangle (7,-54); \n";
    myfile << "\\draw (0,-53) -- (0,-63);\n";
    myfile << "\\draw (5,-53) -- (5,-63);\n";
    myfile << "\\draw (7,-53) -- (7,-63);\n";

    myfile << "\\draw (-15,-55) -- (7,-55); \n";
    myfile << "\\draw (-15,-56) -- (7,-56); \n";
    myfile << "\\draw (-15,-57) -- (7,-57); \n";
    myfile << "\\draw (-15,-58) -- (7,-58); \n";
    myfile << "\\draw (-15,-59) -- (7,-59); \n";
    myfile << "\\draw (-15,-60) -- (7,-60); \n";
    myfile << "\\draw (-15,-61) -- (7,-61); \n";
    myfile << "\\draw (-15,-62) -- (7,-62); \n";

    myfile << "\\filldraw[fill=black!10!white, draw=black] (7,-53) rectangle (25,-58);\n";
    myfile << "\\draw (7,-56) -- (25,-56);\n";
    myfile << "\\draw (7, -60) -- (25,-60);\n";
    myfile << "\\draw (12,-58) -- (12,-62);\n";
    myfile << "\\filldraw[fill=black!10!white, draw=black] (7,-62) rectangle (25,-63);\n";

    myfile << "\\node at (-7.5,-53.5) {Description of Work};\n";
    myfile << "\\node at (2.5,-53.5) {Date};\n";
    myfile << "\\node at (6,-53.5) {By};\n";
    myfile << "\\node at (16, -54) {YOUR COMPANY};\n";
    myfile << "\\node at (16, -55) {System Engineering Department};\n";
    myfile << "\\node at (16,-57) {";
    myfile << name;
    myfile << "-";
    myfile << type;
    myfile << "-RR";
    myfile << number;
    myfile << "};\n";
    myfile << "\\node at (9.5, -59) {Author};\n";
    myfile << "\\node at (9.5, -61) {Date};\n";
    myfile << "\\node at (16, -62.5) {Created by RackMaker};\n";
    myfile.close();
}

void print_drawing_details(string author, string date)
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex", ios::app);
    myfile << "\\node at (18.5,-59) {";
    myfile << author;
    myfile << "};\n";
    myfile << "\\node at (18.5,-61) {";
    myfile << date;
    myfile << "};\n";
    myfile.close();
}

void print_drawing_revision_details(string desc, string date, string by, int count)
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex", ios::app);
    myfile << "\\node at (-7.5,";
    myfile << -54.5 - count;
    myfile << ") {";
    myfile << desc;
    myfile << "};\n";
    myfile << "\\node at (2.5,";
    myfile << -54.5 - count;
    myfile << ") {";
    myfile << date;
    myfile << "};\n";
    myfile << "\\node at (6,";
    myfile << -54.5 - count;
    myfile << ") {";
    myfile << by;
    myfile << "};\n";
    myfile.close();
}

void print_file_close()
{
    ofstream myfile;
    myfile.open ("rackdrawing.tex", ios::app);
    myfile << "\\end{tikzpicture}\n";
    myfile << "\\end{center}\n";
    myfile << "\\end{document}\n";
    myfile.close();
}

///////////////////////////////////////////////////
//
//
// Main Funciton of Program
//
//
///////////////////////////////////////////////////
int main()
{
    ///////////////////////////////////////////////////
    // Set up Varialbes for the Main Program
    ///////////////////////////////////////////////////

    int rack_height;
    int unit_height;
    int count;
    int notes_LeftRight;
    int heightKeeper;

    string author;
    string date;
    string desc;
    string drawing_name;
    string by;

    string unit_name;
    string rack_name;

    string temp_rack_height;
    string temp_unit_height;

    string rack_type;
    string rack_number;

    string file_name;
    string file_name_pdf;
    string file_name_txt;
    string file_date;

    string rack_name_prompt;
    string rack_height_prompt;
    string unit_height_prompt;
    string unit_name_prompt;

    string copyFile;
    string copyInput;

    std::stringstream convert;
  //  std::stringstream move_file_pdf;
  //  std::stringstream move_file_txt;

    count = 1;
    notes_LeftRight = 0;

    ///////////////////////////////////////////////////
    // Main Program Function Calls
    ///////////////////////////////////////////////////
    std::ifstream fin;
    fin.open("input.txt");
    if (fin.is_open())
    {
    file_name = get_type_number (fin);
    rack_name = get_data (fin, temp_rack_height);
    convert << temp_rack_height;
    rack_height = atoi(temp_rack_height.c_str());

    rack_type = get_type_number (fin);
    rack_number = get_type_number (fin);

    print_header(rack_height, rack_number);                               //Print the .TEX file header

    for (int i =0; i < rack_height; i++)
    {
        print_ru_count(i);
    }

    unit_name = get_units (fin, temp_unit_height);                            //Get the units name to be placed on the block as a description
    unit_name = replace_special_characters(unit_name);
    unit_height = atoi(temp_unit_height.c_str());


    while (unit_name != "end")                       //Waits for end of file (prints all units in design file)
    {
        count = print_units(unit_name,unit_height,count,notes_LeftRight,heightKeeper,temp_unit_height);               //Print the .TEX file units
        unit_name = get_units (fin, temp_unit_height);
        unit_name = replace_special_characters(unit_name);
        unit_height = atoi(temp_unit_height.c_str());
    }
    }
    print_footer(rack_height);                                          //Print the .TEX file footer
    print_box(rack_name, rack_type, rack_number);
    date = get_details(fin);
    author = get_details(fin);
    print_drawing_details(author, date);
    file_date = date;

    for (int i = 0; i < 9; i++)
    {
        get_revisions(fin, date, by, desc);
        print_drawing_revision_details(desc, date, by, i);
        desc = "";
        date = "";
        by = "";
    }

    print_file_close();
    fin.close();
    file_name_pdf = file_name + ".pdf";
    file_name_txt = file_name + ".txt";
  //  move_file_pdf << "cp rackdrawing.pdf " << "/tmp/rackmaker/" << file_name << ".pdf";
  //  move_file_txt << "cp input.txt " << "/tmp/rackmaker/" << file_name << ".txt";

    system("pdflatex rackdrawing.tex");
    system("rm rackdrawing.tex");
    system("rm rackdrawing.log");
    system("rm rackdrawing.aux");
    //system( "mv rackdrawing.pdf" move_file_pdf.str().c_str() );
    //system( move_file_txt.str().c_str() );
    rename( "rackdrawing.pdf", file_name_pdf.c_str() );
    rename( "input.txt", file_name_txt.c_str() );
    return 0;
}
