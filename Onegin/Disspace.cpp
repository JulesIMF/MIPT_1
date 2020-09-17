#include <fstream>
#include <cassert>
#include <string>
#include <vector>
int main(int argc, char** argv)
{
	assert(argc == 2);
	std::ifstream fin(argv[1]);
	assert(fin.is_open());
	std::vector<std::string> vstr;
	std::string str;
	while(!fin.eof())
	{
		std::getline(fin, str);
		if(str != ""  && 
		   str != " " &&
		   str != "\t")
			vstr.push_back(str);
       	}
       	fin.close();
       	std::string newFile = "DISSPACED_";
       	newFile += argv[1];
       	std::ofstream fout(newFile);
       	for(auto& s : vstr)
       		fout << s << '\n';

       	fout.close();
}

