#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include<fstream>
#include<iostream>
#include<istream>
#include<sstream>
#include<iomanip>

#include <experimental/filesystem>



using namespace std;
namespace filesys = std::experimental::filesystem;

double rotationMatrix[4][4];
double inputMatrix[4][1] = { 0.0, 0.0, 0.0, 0.0 };
double outputMatrix[4][1] = { 0.0, 0.0, 0.0, 0.0 };

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::fixed << std::setprecision(n) << a_value;
	return out.str();
}

std::vector<std::string> split_line(std::string inputString) {
	std::vector<std::string> wordVector;
	std::stringstream stringStream(inputString);
	std::string line;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(" '/", prev)) != std::string::npos)
		{
			if (pos > prev)
				wordVector.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			wordVector.push_back(line.substr(prev, std::string::npos));
	}
	return wordVector;
}

void multiplyMatrix()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 1; j++) {
			outputMatrix[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				outputMatrix[i][j] += rotationMatrix[i][k] * inputMatrix[k][j];
			}
		}
	}
}

void rotateY(double angle)
{
	angle = angle * M_PI / 180.0; //converting to radian value

	rotationMatrix[0][0] = 1.0;
	rotationMatrix[0][1] = 0.0;
	rotationMatrix[0][2] = 0.0;

	rotationMatrix[1][0] = 0.0;
	rotationMatrix[1][1] = cos(angle);
	rotationMatrix[1][2] = -(sin(angle));

	rotationMatrix[2][0] = 0.0;
	rotationMatrix[2][1] = sin(angle);
	rotationMatrix[2][2] = cos(angle);

	multiplyMatrix();

}

void rotateX(double angle)
{
	angle = angle * M_PI / 180.0; //converting to radian value

	rotationMatrix[0][0] = cos(angle);
	rotationMatrix[0][1] = 0.0;
	rotationMatrix[0][2] = sin(angle);

	rotationMatrix[1][0] = 0.0;
	rotationMatrix[1][1] = 1.0;
	rotationMatrix[1][2] = 0.0;

	rotationMatrix[2][0] = -(sin(angle));
	rotationMatrix[2][1] = 0.0;
	rotationMatrix[2][2] = cos(angle);

	multiplyMatrix();

}

void rotateZ(double angle)
{
	angle = angle * M_PI / 180.0; //converting to radian value

	rotationMatrix[0][0] = cos(angle);
	rotationMatrix[0][1] = -(sin(angle));
	rotationMatrix[0][2] = 0.0;

	rotationMatrix[1][0] = sin(angle);
	rotationMatrix[1][1] = cos(angle);
	rotationMatrix[1][2] = 0.0;

	rotationMatrix[2][0] = 0.0;
	rotationMatrix[2][1] = 0.0;
	rotationMatrix[2][2] = 1.0;

	multiplyMatrix();

}

std::string createNewLine(const std::vector<string> &words)
{
	stringstream new_line;
	for (auto it = words.begin(); it < words.end(); it++)
	{
		new_line << *it << " ";
		if (it == words.end() - 1)
		{
			new_line << "\n";
		}
	}

	return new_line.str();
}

std::string offsetXYZ(const std::string line, float x, float y, float z)
{
	string word;
	vector<string> words;
	stringstream numbersStream(line);
	while (numbersStream >> word)
	{
		words.push_back(word);
	}
	float fx, fy, fz;
	numbersStream.clear();
	numbersStream.seekg(1);
	numbersStream.ignore();
	numbersStream >> fy >> fz >> fx;

	fx = fx + x;
	fy = fy + y;
	fz = fz + z;

	words[1] = to_string_with_precision(fy, 8);
	words[2] = to_string_with_precision(fz, 8);
	words[3] = to_string_with_precision(fx, 8);

	return createNewLine(words);

}

std::string offsetIDX(const std::string &line, int offset)
{
	// string new_line;
	string word;
	vector<string> words;
	stringstream numbersStream(line);
	while (numbersStream >> word)
	{
		words.push_back(word);
	}
	for (auto it = words.begin() + 1; it < words.end(); it++)
	{
		*it = std::to_string(stoi(*it) + offset);
	}

	return createNewLine(words);

}

std::string offsetPoints(const std::string &line, int offset_points, int offset_tris, int vline, int vlight)
{
	// string new_line;
	string word;
	vector<string> words;
	stringstream numbersStream(line);
	while (numbersStream >> word)
	{
		words.push_back(word);
	}
	words[1] = std::to_string(stoi(words[1]) + offset_points);
	words[2] = std::to_string(vline);
	words[3] = std::to_string(vlight);
	words[4] = std::to_string(stoi(words[4]) + offset_tris);

	return createNewLine(words);

}

std::string offsetTRIS(const std::string &line, int offset)
{

	string word;
	vector<string> words;
	stringstream numbersStream(line);
	while (numbersStream >> word)
	{
		words.push_back(word);
	}

	words[1] = std::to_string(stoi(words[1]) + offset);


	return createNewLine(words);
}

std::string rotateXYZ(const std::string &line, double anglex, double angley, double anglez)
{

	string word;
	vector<string> words;
	stringstream numbersStream(line);
	while (numbersStream >> word)
	{
		words.push_back(word);
	}
	double fx, fy, fz, nx, ny, nz;
	numbersStream.clear();
	numbersStream.seekg(1);
	numbersStream.ignore();
	numbersStream >> fx >> fy >> fz >> nx >> ny >> nz;

	inputMatrix[0][0] = fx;
	inputMatrix[1][0] = fy;
	inputMatrix[2][0] = fz;
	rotateZ(-anglez);
	fx = outputMatrix[0][0];
	fy = outputMatrix[1][0];
	fz = outputMatrix[2][0];
	inputMatrix[0][0] = nx;
	inputMatrix[1][0] = ny;
	inputMatrix[2][0] = nz;
	rotateZ(-anglez);
	nx = outputMatrix[0][0];
	ny = outputMatrix[1][0];
	nz = outputMatrix[2][0];

	inputMatrix[0][0] = fx;
	inputMatrix[1][0] = fy;
	inputMatrix[2][0] = fz;
	rotateY(angley);
	fx = outputMatrix[0][0];
	fy = outputMatrix[1][0];
	fz = outputMatrix[2][0];
	inputMatrix[0][0] = nx;
	inputMatrix[1][0] = ny;
	inputMatrix[2][0] = nz;
	rotateY(angley);
	nx = outputMatrix[0][0];
	ny = outputMatrix[1][0];
	nz = outputMatrix[2][0];

	inputMatrix[0][0] = fx;
	inputMatrix[1][0] = fy;
	inputMatrix[2][0] = fz;
	rotateX(-anglex);
	fx = outputMatrix[0][0];
	fy = outputMatrix[1][0];
	fz = outputMatrix[2][0];
	inputMatrix[0][0] = nx;
	inputMatrix[1][0] = ny;
	inputMatrix[2][0] = nz;
	rotateX(-anglex);
	nx = outputMatrix[0][0];
	ny = outputMatrix[1][0];
	nz = outputMatrix[2][0];

	words[1] = to_string_with_precision(fx, 8);
	words[2] = to_string_with_precision(fy, 8);
	words[3] = to_string_with_precision(fz, 8);
	words[4] = to_string_with_precision(nx, 8);
	words[5] = to_string_with_precision(ny, 8);
	words[6] = to_string_with_precision(nz, 8);

	return createNewLine(words);

}

std::vector<filesys::path> findFilesInDir(const std::string &dirPath, const std::string ext)
{

	std::vector<filesys::path> listOfFiles;
	try {

		if (filesys::exists(dirPath) && filesys::is_directory(dirPath))
		{

			filesys::directory_iterator iter(dirPath);

			filesys::directory_iterator end;

			while (iter != end)
			{

				if (iter->path().filename().extension() == ext)
				{
					listOfFiles.push_back(iter->path());
				}

				error_code ec;
				iter.increment(ec);
				if (ec) {
					std::cerr << "Error While Accessing : " << iter->path().string() << " :: " << ec.message() << '\n';
				}
			}
		}
	}
	catch (std::system_error & e)
	{
		std::cerr << "Exception :: " << e.what();
	}
	return listOfFiles;
}

int choseClickReg()
{
	int choise = 0;
	cout << "Chose object to install\n  1. KLN90B\n  2. MD41\n  3. Both\nEnter choise: ";
	cin >> choise;
	if (choise <= 3) return choise;


	return 0;
}

filesys::path choseFile(const std::string &dirPath, const std::string ext)
{
	int choise = 0;
	int i = 1;
	std::vector<filesys::path> listOfFiles = findFilesInDir(dirPath, ext);
	for (auto it = listOfFiles.begin(); it != listOfFiles.end(); it++)
	{

		std::cout << "  "<< i << ". " << it->filename().string() << std::endl;
		i++;
	}
	cout << "Enter choise: ";
	cin >> choise;

	if (choise < 1 || choise > i)
	{
		filesys::path empty;
		return empty;
	}
	return listOfFiles.at(choise - 1);
}

std::vector <std::vector<float> > parseAcf(const filesys::path &acfPath, string &MD41_mod, filesys::path &panel)
{
	std::vector <std::vector<float> > retv;
	std::vector<float> panel_offset;
	bool cockpit_found = false;
	std::string line;
	std::ifstream inFile(acfPath.string());
	bool found = false;
	while (getline(inFile, line))
	{

		if (line.find(panel.filename().stem().string()) != string::npos && found == false)
		{
			found = true;
			cockpit_found = true;
			std::vector<string> words = split_line(line);
			std::string obj_nr = words[2];
			while (words[2] == obj_nr)
			{
				if (words[3] == "_v10_att_phi_ref") {
					panel_offset.push_back(std::stof(words[4]));
				}
				else if (words[3] == "_v10_att_psi_ref") {
					panel_offset.push_back(std::stof(words[4]));
				}
				else if (words[3] == "_v10_att_the_ref") {
					panel_offset.push_back(std::stof(words[4]));
				}
				else if (words[3] == "_v10_att_x_acf_prt_ref") {
					panel_offset.push_back(std::stof(words[4]));
				}
				else if (words[3] == "_v10_att_y_acf_prt_ref") {
					panel_offset.push_back(std::stof(words[4]));
				}
				else if (words[3] == "_v10_att_z_acf_prt_ref") {
					panel_offset.push_back(std::stof(words[4]));
				}
				getline(inFile, line);
				words = split_line(line);
			}

		}
		else if (line.find("kln90b/kln90b.obj") != string::npos)
		{
			std::vector<float> values;
			if (cockpit_found == false)
			{
				cout << panel.filename().string() << " is not part of "<< acfPath.filename().string() << endl;
				#ifdef WIN
				system("pause");
				#endif
				exit(-1);
			}

			std::vector<string> words = split_line(line);
			std::string obj_nr = words[2];
			while (words[2] == obj_nr)
			{
				if (words[3] == "_v10_att_phi_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[0]));
				}
				else if (words[3] == "_v10_att_psi_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[1]));
				}
				else if (words[3] == "_v10_att_the_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[2]));
				}
				else if (words[3] == "_v10_att_x_acf_prt_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[3]));
				}
				else if (words[3] == "_v10_att_y_acf_prt_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[4]));
				}
				else if (words[3] == "_v10_att_z_acf_prt_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[5]));
				}
				getline(inFile, line);
				words = split_line(line);
			}
			retv.push_back(values);
		}
		else if (line.find("kln90b/MD41") != string::npos)
		{
			MD41_mod = line.substr(line.find("MD41"), 6);
			std::vector<float> values;
			std::vector<string> words = split_line(line);
			std::string obj_nr = words[2];
			while (words[2] == obj_nr)
			{
				if (words[3] == "_v10_att_phi_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[0]));
				}
				else if (words[3] == "_v10_att_psi_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[1]));
				}
				else if (words[3] == "_v10_att_the_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[2]));
				}
				else if (words[3] == "_v10_att_x_acf_prt_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[3]));
				}
				else if (words[3] == "_v10_att_y_acf_prt_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[4]));
				}
				else if (words[3] == "_v10_att_z_acf_prt_ref") {
					values.push_back(std::stof(words[4]) - (panel_offset[5]));
				}
				getline(inFile, line);
				words = split_line(line);
			}

			if(!retv.empty())
			retv.push_back(values);
		}

	}
	return retv;
}

std::vector<std::vector<string> > parseClickReg(const filesys::path &clickreg)
{
	std::vector<std::vector<string> > retv;
	std::vector<string> vtLine;
	std::vector<string> idxLine;
	std::vector<string> animLine;
	string line;
	std::ifstream inputClick(clickreg.string());
	while (getline(inputClick, line))
	{
		string word;
		istringstream words(line);
		words >> word;
		if (word == "POINT_COUNTS")
		{
			std::vector<string> temp;
			temp.push_back(line);
			retv.push_back(temp);
		}
		else if (word == "VT") break;
	}
	while (inputClick)
	{
		string word;
		istringstream words(line);
		words >> word;

		if (word == "VT")
		{
			vtLine.push_back(line);
		}
		else if (word == "IDX" || word == "IDX10")
		{
			idxLine.push_back(line);
		}
		else animLine.push_back(line);
		getline(inputClick, line);
	}
	retv.push_back(vtLine);
	retv.push_back(idxLine);
	retv.push_back(animLine);

	return retv;
}


filesys::path integrateClickReg(const filesys::path &clickreg,const filesys::path &panel, const std::vector<float> &offset)
{

	string newfile = panel.filename().string();
	newfile.replace(newfile.find(".obj"), newfile.length(), "_kln90b.obj");

	std::vector<std::vector<string> > clickReg = parseClickReg(clickreg);

	string line;
	ofstream output;
	output.open(newfile, std::ofstream::trunc);

	std::ifstream inputPanel(panel.string());

	int offset_IDX = 0;
	int offset_TRIS = 0;
	int temp_VLINE = 0;
	int temp_VLIGHT = 0;
	while (getline(inputPanel, line))
	{
		string word;
		istringstream words(line);
		words >> word;
		if (word == "POINT_COUNTS")
		{
			words >> offset_IDX >> temp_VLINE >> temp_VLIGHT >> offset_TRIS;
			output << offsetPoints(clickReg[0][0], offset_IDX, offset_TRIS, temp_VLINE, temp_VLIGHT);

		}
		else if (word == "VT")
		{


			while (inputPanel)
			{


				if (word != "VT") break;
				else output << line << "\n";
				getline(inputPanel, line);
				istringstream words(line);
				words >> word;
			}

			for (auto l : clickReg[1])
			{
				output << offsetXYZ(rotateXYZ(l, offset[1], offset[2], offset[0]), offset[5] * 0.3048, offset[3] * 0.3048, offset[4] * 0.3048);
			}

			output << line << "\n";
		}
		else if (word == "IDX" || word == "IDX10")
		{
			while (inputPanel)
			{
				if (word != "IDX10")
				{
					if (word != "IDX")	break;
					else output << line << "\n";
				}
				else output << line << "\n";
				getline(inputPanel, line);
				istringstream words(line);
				words >> word;
			}
			for (auto l : clickReg[2])
			{
				output << offsetIDX(l, offset_IDX);
			}
			output << line << "\n";
		}
		else output << line << "\n";
	}

	for (auto l : clickReg[3])
	{
		string word;
		istringstream tris(l);
		tris >> word;
		if (word == "TRIS")
		{
			output << offsetTRIS(l, offset_TRIS);
		}
		else output << l << "\n";
	}

	inputPanel.close();
	output.close();
	filesys::path retval(newfile);
	return retval;
}

int main()
{
	std::string dirPath = "../../";


	cout << "Chose ACF file:" << endl;
	filesys::path acfPath = choseFile(dirPath, ".acf");
	if (acfPath.empty())
	{
		cout << "Wrong choise! Try again...\nExiting" << endl;
		#ifdef WIN
		system("pause");
		#endif
		return -1;
	}

	cout << "Chose panel.obj file:" << endl;
	filesys::path panelPath = choseFile(dirPath, ".obj");
	if (panelPath.empty())
	{
		cout << "Wrong choise! Try again...\nExiting" << endl;
		#ifdef WIN
		system("pause");
		#endif
		return -1;
	}
	//cout << panelPath.filename().string() << endl;
	//cout << acfPath.string() << endl;
	string MD41_mod;
	std::vector<std::vector<float> > movrot = parseAcf(acfPath, MD41_mod, panelPath);
	if (movrot.empty())
	{
		cout << "KLN90B not found in " << acfPath.filename().string() << "...\nExiting!" << endl;
		#ifdef WIN
		system("pause");
		#endif
		return -1;
	}
	else
	{
		if (!movrot[0].empty())
		{
			cout << "\nFound KLN90B at position " << movrot[0][5] << " " << movrot[0][3] << " " << movrot[0][4] << " " << movrot[0][1] << " " << movrot[0][2] << " " << movrot[0][0] << " " << endl;
		}
		if (movrot.size() == 2)
		{
			cout << "\nFound MD41 at position " << movrot[1][5] << " " << movrot[1][3] << " " << movrot[1][4] << " " << movrot[1][1] << " " << movrot[1][2] << " " << movrot[1][0] << " " << endl;
		}
		else
		{
			cout << "\nMD41 not found." << endl;
		}

	}



	filesys::path newPanelPath = integrateClickReg("kln90b_click_regions.obj", panelPath, movrot[0]);
	if (movrot.size() == 2)
	{
		filesys::path newPath = integrateClickReg(MD41_mod+"_click_regions.obj", newPanelPath, movrot[1]);
		panelPath = newPanelPath;
		filesys::remove(newPanelPath);
		filesys::rename(newPath, panelPath);
	}

	std::cout << "****************************************************" << std::endl;




	cout << "Done!"<< endl;//\nGo to PlaneMaker and point panel to " << panelPath.filename().string()
	#ifdef IBM
	system("pause");
	#endif
	return 0;
}
