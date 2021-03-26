#include <cstdio>
#include "xmlPrinter.cpp"

int main() {
  xmlPrinter printer;
  std::ifstream newfile ("xml.txt");
  if (newfile.is_open()){   //checking whether the file is open
    std::string tp;
    while(getline(newfile, tp)){ 
      std::cout << printer.createXML(tp) << "\n"; //print the data of the string
    }
    newfile.close(); //close the file object.
  }
  return 0;
}
