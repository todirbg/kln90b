#undef __STRICT_ANSI__
#include <iostream>
#include <cmath>
#include<fstream>
#include<iostream>
#include<istream>
#include<string>
#include<sstream>
#include<vector>
#include<iomanip>

using namespace std;

double rotationMatrix[4][4];
double inputMatrix[4][1] = {0.0, 0.0, 0.0, 0.0};
double outputMatrix[4][1] = {0.0, 0.0, 0.0, 0.0};

using namespace std;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out  <<  std::fixed << std::setprecision(n) << a_value;
    return out.str();
}

void multiplyMatrix()
{
    for(int i = 0; i < 3; i++ ){
        for(int j = 0; j < 1; j++){
            outputMatrix[i][j] = 0;
            for(int k = 0; k < 3; k++){
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

std::string createNewLine(std::vector<string> &words)
{
        stringstream new_line;
        for(auto it = words.begin(); it<words.end(); it++)
        {
            new_line<< *it<<" ";
           // cout<< *it<<" ";
            if(it==words.end()-1)
            {
                new_line<<"\n";
               // cout<<"\n";
            }
        }

        return new_line.str();
}

std::string offsetXYZ(std::string line, float x, float y, float z)
{
           // string new_line;
            string word;
            vector<string> words;
            stringstream numbersStream(line);
            while(numbersStream>>word)
            {
                words.push_back(word);
            }
            float fx, fy, fz;
            numbersStream.clear();
            numbersStream.seekg(1);
            numbersStream.ignore();
            numbersStream>>fy>>fz>>fx;
           // cout<<"read for x:"<<fx<<endl;
            //cout<<"read for y:"<<fy<<endl;
            //cout<<"read for z:"<<fz<<endl;

            fx=fx+x;
            fy=fy+y;
            fz=fz+z;

            //cout<<"changed to x:"<<fx<<endl;
            //cout<<"changed to y:"<<fy<<endl;
            //cout<<"changed to z:"<<fz<<endl;

            words[1]=to_string_with_precision(fy, 8);
            words[2]=to_string_with_precision(fz, 8);
            words[3]=to_string_with_precision(fx, 8);

            return createNewLine(words);

}

std::string offsetIDX(std::string &line, int offset)
{
            string word;
            vector<string> words;
            stringstream numbersStream(line);
            while(numbersStream>>word)
            {
                words.push_back(word);
            }
             for(auto it = words.begin()+1; it<words.end(); it++)
             {
                     *it = std::to_string(stoi(*it) + offset);
             }

            return createNewLine(words);

}

std::string offsetPoints(std::string &line, int offset_points,int offset_tris)
{
            string word;
            vector<string> words;
            stringstream numbersStream(line);
            while(numbersStream>>word)
            {
                words.push_back(word);
            }
             words[1] = std::to_string(stoi(words[1]) + offset_points);
             words[4] = std::to_string(stoi(words[4]) + offset_tris);

            return createNewLine(words);

}

std::string offsetTRIS(std::string &line, int offset)
{

        string word;
        vector<string> words;
        stringstream numbersStream(line);
        while(numbersStream>>word)
        {
            words.push_back(word);
        }

        words[1] = std::to_string(stoi(words[1]) + offset);


        return createNewLine(words);
}

std::string rotateXYZ(std::string &line, double anglex, double angley, double anglez)
{
            string word;
            vector<string> words;
            stringstream numbersStream(line);
            while(numbersStream>>word)
            {
                words.push_back(word);
            }
            double fx, fy, fz, nx, ny, nz;
            numbersStream.clear();
            numbersStream.seekg(1);
            numbersStream.ignore();
            numbersStream>>fx>>fy>>fz>>nx>>ny>>nz;
           // cout<<"read for x:"<<std::setprecision(8)<<fx<<endl;
            //cout<<"read for y:"<<fy<<endl;
            //cout<<"read for z:"<<fz<<endl;

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
            //cout<<"changed to x:"<<outputMatrix[2][0]<<endl;
            //cout<<"changed to y:"<<fy<<endl;
            //cout<<"changed to z:"<<fz<<endl;
           // cout<< std::setprecision(8) << outputMatrix[0][0] << endl;
           //cout<< std::setprecision(8) <<  std::fixed << fx << " " << fy << " " << fz << endl;
            words[1]=to_string_with_precision(fx, 8);
            words[2]=to_string_with_precision(fy, 8);
            words[3]=to_string_with_precision(fz, 8);
            words[4]=to_string_with_precision(nx, 8);
            words[5]=to_string_with_precision(ny, 8);
            words[6]=to_string_with_precision(nz, 8);
           // cout <<words[1] << endl;
            return createNewLine(words);

}

int main()
{
    string units;
    cout << "Enter units - f or m: ";
    cin >> units;

    float px,py,pz;
    cout << "panel offset for X: ";
    cin >> px;
    cout << "panel offset for Y: ";
    cin >> py;
    cout << "panel offset for Z: ";
    cin >> pz;

   cout << "click zones filename: ";
   string click_zones_file;
   cin >> click_zones_file;

    float x,y,z;
    cout << "offset for X: ";
    cin >> x;
    cout << "offset for Y: ";
    cin >> y;
    cout << "offset for Z: ";
    cin >> z;

    double ax,ay,az;
    cout << "angle for X: ";
    cin >> ax;
    cout << "angle for Y: ";
    cin >> ay;
    cout << "angle for Z: ";
    cin >> az;

    x = x - (px);
    y = y - (py);
    z = z - (pz);

    if(units == "f")
    {
        x = x*0.3048;
        y = y*0.3048;
        z = z*0.3048;
    }

    int offset_IDX;
    cout << "offset for IDX: ";
    cin >> offset_IDX;

     int offset_TRIS;
    cout << "offset for TRIS: ";
    cin >> offset_TRIS;

   ifstream inFile(click_zones_file.c_str() );

   if (! inFile)
   {
      cerr << "unable to open input file: "
	   << click_zones_file << " --exit! \n";
      return -1;
   }

    string line;
    ofstream output;
    output.open("offset.txt", std::ofstream::trunc);
   while (getline(inFile,line))
   {
        string word;
        istringstream first_word(line);
        first_word>>word;
        if(word == "POINT_COUNTS")
       {
        output<<offsetPoints(line, offset_IDX, offset_TRIS);
       }
       else if(word == "VT")
       {
        output<<offsetXYZ(rotateXYZ(line, ax, ay, az), x, y, z);
       }
       else if(word=="IDX" || word=="IDX10")
       {
           output<<offsetIDX(line, offset_IDX);
       }
        else if(word=="TRIS")
       {
           output<<offsetTRIS(line, offset_TRIS);
       }
       else output<<line<<"\n";

   }


    cout << "Done check offset.txt" << endl;
    system("pause");
    return 0;
}
