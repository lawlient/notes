#include <iostream>

#include "meta.pb.h"



int main() {
    Student s;
    s.set_name("xujiuan");
    s.set_age(29);
    s.set_gender(Student_Gender_Male);

    std::cout << s.DebugString() 
              << "gender: " << Student::Gender_Name(s.gender())
              << std::endl;

    std::string compress = s.SerializeAsString();

    std::cout << compress << std::endl;
    std::cout << compress.size() << std::endl;

    Student ss;
    ss.ParseFromString(compress);

    std::cout << ss.DebugString() 
              << "gender: " << Student::Gender_Name(ss.gender())
              << std::endl;

    return 0;
}
