#include <json.hpp>

using json = nlohmann::json;

int main()
{
    // create JSON values
    json j_object = {{"one", 1}, {"two", 2}};
    json j_array = {1, 2, 4, 8, 16};

    // call dump()
    std::cout << j_object.dump() << "\n\n";
    std::cout << j_object.dump(-1) << "\n\n";
    std::cout << j_object.dump(0) << "\n\n";
    std::cout << j_object.dump(4) << "\n\n";
    std::cout << j_array.dump() << "\n\n";
    std::cout << j_array.dump(-1) << "\n\n";
    std::cout << j_array.dump(0) << "\n\n";
    std::cout << j_array.dump(4) << "\n\n";

    // user-defined printer: do not separate array values with newlines
    json::printer my_printer = json::printer::pretty_printer();
    my_printer.array_comma = ", ";

    // call dump() with user-defined printer
    std::cout << j_object.dump(4, my_printer) << "\n\n";
    std::cout << j_array.dump(4, my_printer) << std::endl;
}
