#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

const int MAX_VALUE=255;

class pixel /*class for represent a one pixel of the image*/
{
public:
	int red,green,blue;
};

class ppmImage
{
public:
	ppmImage(const string filename); //reader constructor
	ppmImage(const int nrow,const int ncolumn);	//all image white constructor
	ppmImage(){}; 						//default const.
	int getRow(); //accesor for row
	int getColumn();// accesor for column
	void setRow(const int nrow);//change value of row
	void setColumn(const int ncolumn);//change value of column
	bool save_image(const string filename);//save image, if it succeeds, return true otherwise false
	bool read_image(const string filename);//same above but read image
	void print_dimensions();//prints row and column value
	int color_value(const int nrow,const int ncolumn,int color);//return relevant value of specified color
	void change_inpixel(const int nrow,const int ncolumn,const int color,const int newnumber);// change value of one color in a pixel
	bool check_validity();//basically checks if input is valid
	bool checker;//constructors does not return. So function has to know if relevant part is checked or not
private:
	vector<vector<pixel>> image; //full image
	int row,column; //total row and column value.
};

int ppmImage::getRow() {
	return row;
}

int ppmImage::getColumn() {
	return column;
}

void ppmImage::setRow(const int nrow){
	row=nrow;
	return;
}

void ppmImage::setColumn(const int ncolumn){
	column=ncolumn;
	return;
}

bool ppmImage::save_image(const string filename) {
	int i,j;
	ofstream outFile;
	outFile.open(filename);
	if(outFile.is_open()){
			outFile<<"P3"<<endl;
		outFile<<row<<" "<<column<<endl;
		outFile<<MAX_VALUE<<endl;
		for(i=0;i<row;i++){
			for(j=0;j<column;j++){
				outFile<<image[i][j].red<<" ";
				outFile<<image[i][j].green<<" ";
				if(j+1==column){
					outFile<<image[i][j].blue;
					break;
				}
				outFile<<image[i][j].blue<<" ";

			}
			outFile<<endl;
		}
		outFile.close();
	}
	else
	return false;
	return true;
}

bool ppmImage::read_image(const string filename) {
	ifstream inFile;
	inFile.open(filename);
	if(!inFile.is_open())
		return false;
	string str;
	int ascii;
	inFile>>str;
	inFile>>row>>column;
	inFile>>ascii;
	int i,j;
	pixel a;
	for(i=0;i<row;i++) {
		vector<pixel> v;
		for(j=0;j<column;j++){
			inFile>>a.red;
			inFile>>a.green;
			inFile>>a.blue;
			v.push_back(a);
		}
		image.push_back(v);
	}
	inFile.close();
	return true;
}

void ppmImage::print_dimensions() {
	cout<<row<<" "<<column<<endl;
}

void ppmImage::change_inpixel(const int nrow,const int ncolumn,const int color,const int newnumber) {
	if(color==0) //red
	image[nrow][ncolumn].red=newnumber;
	else if(color==1) //green
	image[nrow][ncolumn].green=newnumber;
	else if(color==2) //blue
	image[nrow][ncolumn].blue=newnumber;
	else
	exit(1);
}

int ppmImage::color_value(const int nrow,const int ncolumn,int color) {
	//vector<pixel> v;
	//v.push_back(image[nrow]);
	if(color==0)
		return image[nrow][ncolumn].red;
	if(color==1)
		return image[nrow][ncolumn].green;
	if(color==2)
		return image[nrow][ncolumn].blue;
	else
		exit(1);
}



bool ppmImage::check_validity() {
	for(unsigned int i=0;i<row;i++) {
		for(unsigned int j=0;j<column;j++){
			if(image[i][j].red<0 || image[i][j].red>MAX_VALUE)
				return false;
			if(image[i][j].green<0 || image[i][j].green>MAX_VALUE)
				return false;
			if(image[i][j].blue<0 || image[i][j].blue>MAX_VALUE)
				return false;
		}
	}
	return true;
}

ppmImage::ppmImage(const string filename) {
	checker=read_image(filename);
	if(!check_validity()){
		exit(1);
	}
}

ppmImage::ppmImage(const int nrow,const int ncolumn) {
	setRow(nrow);
	setColumn(ncolumn);
	for(unsigned int i=0;i<nrow;i++) {
		for(unsigned int j=0;j<ncolumn;j++) {
			image[i][j].red=MAX_VALUE;
			image[i][j].green=MAX_VALUE;
			image[i][j].blue=MAX_VALUE;
		}
	}
}

// returns 1 if the operation is successful. otherwise, returns 0.
// reads ppm data from file named as source_ppm_file_name. stores data in destination_object
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object) {
	destination_object=ppmImage(source_ppm_file_name);//call object constructor about read file

	if(destination_object.read_image(source_ppm_file_name)){
		if(!destination_object.check_validity()){//exit if it's not valid
			exit(1);
				}
		return 1;
	}
		return 0;
}

// returns 1 if the operation is successful. otherwise, returns 0.
// writes ppm data from source_object to the file named destination_ppm_file_name.
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object){
	ppmImage newone;
	newone=source_object;
	if(newone.save_image(destination_ppm_file_name))
		return 1;

	return 0;
}

// this function swaps the color values of every pixel in a given ppm image.
// this function does not create a new object but modifies the given one.
// if swap_choice is 1: swaps red and green
// if swap_choice is 2: swaps red and blue
// if swap_choice is 3: swaps green and blue
// if swap_choice is not 1, 2 or 3: no swaps (this does not mean that the operation is not
// successful. the function should return 1 in this case if everything is normal)
// returns 1 if the operation is successful. otherwise, returns 0.
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice){
	if(swap_choice==1) {//red and green
		int i,j,temp;
		int row=image_object_to_be_modified.getRow();
		int column=image_object_to_be_modified.getColumn();
		for(i=0;i<row;i++){
			for(j=0;j<column;j++) {
				temp=image_object_to_be_modified.color_value(i,j,0); //get red value of object as temp
				image_object_to_be_modified.change_inpixel(i,j,0,image_object_to_be_modified.color_value(i,j,1));
				image_object_to_be_modified.change_inpixel(i,j,1,temp);
			}
		}
		return 1;
	}
	else if(swap_choice==2) {//red and blue
		int i,j,temp;
		int row=image_object_to_be_modified.getRow();
		int column=image_object_to_be_modified.getColumn();
		for(i=0;i<row;i++){
			for(j=0;j<column;j++) {
				temp=image_object_to_be_modified.color_value(i,j,0); //get red value of object as temp
				image_object_to_be_modified.change_inpixel(i,j,0,image_object_to_be_modified.color_value(i,j,2));
				image_object_to_be_modified.change_inpixel(i,j,2,temp);
			}
		}
		return 1;
	}

	else if(swap_choice==3) {//green and blue
		int i,j,temp;
		int row=image_object_to_be_modified.getRow();
		int column=image_object_to_be_modified.getColumn();
		for(i=0;i<row;i++){
			for(j=0;j<column;j++) {
				temp=image_object_to_be_modified.color_value(i,j,1); //get green value of object as temp
				image_object_to_be_modified.change_inpixel(i,j,1,image_object_to_be_modified.color_value(i,j,2));
				image_object_to_be_modified.change_inpixel(i,j,2,temp);
			}
		}
		return 1;
	}
	else
		exit(1);

	return 0;
}

// creates and returns a copy of a new ppmImage object which stores only one color at each
//pixel. This simply takes the source and copies only one color information and stores it in
//a new object. The other color channels are simply going to be zeros.
//if color_choice is 1: red channel is preserved
//if color_choice is 2: green channel is preserved
//if color_choice is 3: blue channel is preserved
ppmImage single_color(const ppmImage& source, int color_choice) {
	ppmImage newone;
	newone=source;
	if(color_choice==1){//red preserve
		int i,j;
		int row,column;
		row=newone.getRow();
		column=newone.getColumn();
		for(i=0;i<row;i++) {
			for(j=0;j<column;j++) {// assign other values as 0
				newone.change_inpixel(i,j,1,0);
				newone.change_inpixel(i,j,2,0);
			}
		}
	}
	else if(color_choice==2){//green preserve
		int i,j;
		int row,column;
		row=newone.getRow();
		column=newone.getColumn();
		for(i=0;i<row;i++) {
			for(j=0;j<column;j++) {// assign other values as 0
				newone.change_inpixel(i,j,0,0);
				newone.change_inpixel(i,j,2,0);
			}
		}
	}
	else if(color_choice==3){//blue preserve
		int i,j;
		int row,column;
		row=newone.getRow();
		column=newone.getColumn();
		for(i=0;i<row;i++) {
			for(j=0;j<column;j++) {// assign other values as 0
				newone.change_inpixel(i,j,0,0); 
				newone.change_inpixel(i,j,1,0);
			}
		}
	}
	else exit(1);
	return newone;
}

int main(int argc, char** argv)
{
// check for number of command line arguments
// the first argument is going to be choice number
// the second argument is going to be a ppm_file_name
	if(argc!=3)
		exit(1);
	string str1=argv[1];
	int choice_number=stoi(str1);
	string ppm_file_name=argv[2];
// if choice number is 1
// read ppm_file_name using function read_ppm
// write the same data without changing anything to a file named "o1.ppm". use write_ppm
// function.
if(choice_number==1) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	write_ppm("o1.ppm",ppm_object);
}

// if choice number is 2
// read ppm_file_name using function read_ppm
// swap red and green channels
// write the updated data to a file named "o2.ppm". use write_ppm function.
else if(choice_number==2) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	swap_channels(ppm_object,1);
	write_ppm("o2.ppm",ppm_object);
}

// if choice number is 3
// read ppm_file_name using function read_ppm
// swap red and blue channels
// write the updated data to a file named "o3.ppm". use write_ppm function.
else if(choice_number==3) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	swap_channels(ppm_object,2);
	write_ppm("o3.ppm",ppm_object);
}

// if choice number is 4
// read ppm_file_name using function read_ppm
// swap green and blue channels. use swap_channels function
// write the updated data to a file named "o4.ppm". use write_ppm function.
else if(choice_number==4) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	swap_channels(ppm_object,3);
	write_ppm("o4.ppm",ppm_object);
}

// if choice number is 5
// read ppm_file_name using function read_ppm
// create a new object which only contains red channel data of the file read. ue single_color
// function
// write the data of the new object to a file named "o5.ppm". use write_ppm function.
else if(choice_number==5) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	ppm_object=single_color(ppm_object,1);
	write_ppm("o5.ppm",ppm_object);
}

// if choice number is 6
// read ppm_file_name using function read_ppm
// create a new object which only contains green channel data of the file read. ue
// single_color function
// write the data of the new object to a file named "o6.ppm". use write_ppm function.
else if(choice_number==6) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	ppm_object=single_color(ppm_object,2);
	write_ppm("o6.ppm",ppm_object);
}

// if choice number is 7
// read ppm_file_name using function read_ppm
// create a new object which only contains blue channel data of the file read. ue single_color
// function
// write the data of the new object to a file named "o7.ppm". use write_ppm function.
else if(choice_number==7) {
	ppmImage ppm_object;
	read_ppm(ppm_file_name,ppm_object);
	ppm_object=single_color(ppm_object,3);
	write_ppm("o7.ppm",ppm_object);
}

}